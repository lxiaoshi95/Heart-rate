#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "driver.h"
#include "driver_internals.h"
#include "uart2trans.h"
#include "util.h"


/* Convenience macro */
#define WRITE_CMD_AND_WAIT(dev, cmd) { \
  struct timespec ts = { \
    .tv_sec = 0, \
    .tv_nsec = AFE4400EVM_WAIT_TIME * 1000000L, \
  }; \
  nanosleep(&ts, NULL); \
  TRY_RETURN_ERRNO(afe4400evm_uart_write(dev, &cmd, sizeof(cmd))); \
}

#define REASM_24B(array) ((array[0]) | (array[1] << 8) | (array[2] << 16))

#define POLL_FOR_RESPONSE(dev, response, msg_type) { \
  register int res; \
  /* TODO: set a retry count? */ \
  do { \
    res = afe4400evm_poll_incoming_transactions(dev); \
    if (res != EXIT_SUCCESS && res != -ETIMEDOUT) { \
      return res; \
    } \
  } while (NULL == (response = afe4400evm_get_transaction_by_type(dev, msg_type))); \
}


char* register_names[AFE4400EVM_REG_LAST + 1] = {
  "CONTROL0",
  "LED2STC",
  "LED2ENDC",
  "LED2LEDSTC",
  "LED2LEDENDC",
  "ALED2STC",
  "ALED2ENDC",
  "LED1STC",
  "LED1ENDC",
  "LED1LEDSTC",
  "LED1LEDENDC",
  "ALED1STC",
  "ALED1ENDC",
  "LED2CONVST",
  "LED2CONVEND",
  "ALED2CONVST",
  "ALED2CONVEND",
  "LED1CONVST",
  "LED1CONVEND",
  "ALED1CONVST",
  "ALED1CONVEND",
  "ADCRSTSTCT0",
  "ADCRSTENDCT0",
  "ADCRSTSTCT1",
  "ADCRSTENDCT1",
  "ADCRSTSTCT2",
  "ADCRSTENDCT2",
  "ADCRSTSTCT3",
  "ADCRSTENDCT3",
  "PRPCOUNT",
  "CONTROL1",
  "SPARE1",
  "TIAGAIN",
  "TIA_AMB_GAIN",
  "LEDCNTRL",
  "CONTROL2",
  "SPARE2",
  "SPARE3",
  "SPARE4",
  "RESERVED1",
  "RESERVED2",
  "ALARM",
  "LED2VAL",
  "ALED2VAL",
  "LED1VAL",
  "ALED1VAL",
  "LED2",
  "LED1NET",
  "DIAG",
  "CONTROL3",
  "PDNCYCLESTC",
  "PDNCYCLEENDC",
};

static char u2c(uint8_t u) {
  if (u <= 9) {
    return u + 0x30;
  }

  return u + 0x41 - 10;
}

static void u82c(uint8_t u, char c[2]) {
  c[0] = u2c(u >> 4);
  c[1] = u2c(u & 0x0f);
}

static void u242c(uint32_t u, char c[6]) {
  u82c((u >> 16) & 0xff, c);
  u82c((u >> 8) & 0xff, c + 2);
  u82c(u & 0xff, c + 4);
}

/* Open and setup serial. Returns pointer to device context, or NULL on error with errno set. */
afe4400_t* afe4400evm_init(void) {
  afe4400_t* dev;

  dev = malloc(sizeof(*dev));
  if (dev == NULL) {
    errno = ENOMEM;
    return NULL;
  }
  dev->adc_enabled = false;
  dev->devh = NULL;
  dev->rxq = NULL;
  dev->rxq_tail = NULL;

  if (afe4400evm_uart_setup(dev) != EXIT_SUCCESS) {
    afe4400evm_cleanup(dev);
    return NULL;
  }

  return dev;
}

int afe4400evm_cleanup(afe4400_t* dev) {
  if (dev == NULL) {
    return -EFAULT;
  }

  if (dev->adc_enabled) {
    TRY_RETURN_RES(afe4400evm_stop_adc(dev));
  }

  TRY_RETURN_RES(afe4400evm_uart_cleanup(dev));
  TRY_RETURN_RES(afe4400evm_destory_transactions(dev));

  free(dev);
  return EXIT_SUCCESS;
}

int afe4400evm_start_adc(afe4400_t* dev, bool continuous, int count) {
  struct afe4400evm_cmd_adc_start cmd = {
    .command  = AFE4400EVM_CMD_ADC_START,
    .dummy    = AFE4400EVM_ADC_DUMMY,
    .splitter = AFE4400EVM_CR,
  };

  if (dev == NULL) {
    return -EFAULT;
  }

  if (continuous) {
    memset(cmd.count, 0, 4);
  } else {
    cmd.count[0] = (count >> 24) & 0xff;
    cmd.count[1] = (count >> 16) & 0xff;
    cmd.count[2] = (count >> 8) & 0xff;
    cmd.count[3] = count & 0xff;
  }
  WRITE_CMD_AND_WAIT(dev, cmd);

  dev->adc_enabled = true;
  return EXIT_SUCCESS;
}

int afe4400evm_stop_adc(afe4400_t* dev) {
  struct afe4400evm_cmd_adc_stop cmd = {
    .command  = AFE4400EVM_CMD_ADC_STOP,
    .splitter = AFE4400EVM_CR,
  };

  if (dev == NULL) {
    return -EFAULT;
  }

  WRITE_CMD_AND_WAIT(dev, cmd);

  dev->adc_enabled = false;
  printf("1");
  return EXIT_SUCCESS;
}

/* This function will block */
int afe4400evm_poll_adc(afe4400_t* dev, afe4400_result_t* result) {
  struct afe4400evm_resp_adc *response;

  if ((dev == NULL) || (result == NULL)) {
    return -EFAULT;
  }
/*
  if (!(dev->adc_enabled)) {
    return -ENODATA;
  }
*/
  POLL_FOR_RESPONSE(dev, response, AFE4400EVM_CMD_ADC_START);

  result->red         = REASM_24B(response->red);
  result->red_ambient = REASM_24B(response->red_ambient);
  result->ir          = REASM_24B(response->ir);
  result->ir_ambient  = REASM_24B(response->ir_ambient);
  result->red_net     = REASM_24B(response->red_net);
  result->ir_net      = REASM_24B(response->ir_net);

  free(response);
  return EXIT_SUCCESS;
}

/* Since this function requires reading responses, need to make sure ADC is not running. */
int afe4400evm_read_register(afe4400_t* dev, uint8_t reg_addr, uint32_t *reg_data) {
  struct afe4400evm_resp_reg_read* response;
  struct afe4400evm_cmd_reg_read cmd = {
    .command  = AFE4400EVM_CMD_REG_READ,
    .splitter = AFE4400EVM_CR,
  };

  if ((dev == NULL) || (reg_data == NULL)) {
    return -EFAULT;
  }

  if (dev->adc_enabled) {
    return -EBUSY;
  }

  if (reg_addr > AFE4400EVM_REG_LAST) {
    return -ERANGE;
  }

  u82c(reg_addr, cmd.reg_addr);
  WRITE_CMD_AND_WAIT(dev, cmd);
  POLL_FOR_RESPONSE(dev, response, AFE4400EVM_CMD_REG_READ);

  *reg_data = REASM_24B(response->reg_data);
  free(response);
  return EXIT_SUCCESS;
}

int afe4400evm_write_register(afe4400_t* dev, uint8_t reg_addr, uint32_t reg_data) {
  struct afe4400evm_cmd_reg_write cmd = {
    .command  = AFE4400EVM_CMD_REG_WRITE,
    .splitter = AFE4400EVM_CR,
  };

  if (dev == NULL) {
    return -EFAULT;
  }

  if (reg_addr > AFE4400EVM_REG_LAST) {
    return -ERANGE;
  }

  u82c(reg_addr, cmd.reg_addr);
  u242c(reg_data, cmd.reg_data);
  WRITE_CMD_AND_WAIT(dev, cmd);

  return EXIT_SUCCESS;
}

int afe4400evm_set_calibration(afe4400_t* dev, bool enabled) {
  struct afe4400evm_cmd_cal cmd = {
    .command    = AFE4400EVM_CMD_CAL,
    .cal_enable = enabled ? AFE4400EVM_CAL_TRUE : AFE4400EVM_CAL_FALSE,
    .splitter   = AFE4400EVM_CR,
  };

  if (dev == NULL) {
    return -EFAULT;
  }

  WRITE_CMD_AND_WAIT(dev, cmd);

  return EXIT_SUCCESS;
}

int afe4400evm_enter_bootloader(afe4400_t* dev) {
  struct afe4400evm_cmd_fw_update cmd = {
    .command    = AFE4400EVM_CMD_FW_UPG,
    .splitter   = AFE4400EVM_CR,
  };

  if (dev == NULL) {
    return -EFAULT;
  }

  WRITE_CMD_AND_WAIT(dev, cmd);

  return EXIT_SUCCESS;
}

int afe4400evm_get_id(afe4400_t* dev, char id[4]) {
  struct afe4400evm_resp_rdid* response;
  struct afe4400evm_cmd_rdid cmd = {
    .command    = AFE4400EVM_CMD_RDID,
    .splitter   = AFE4400EVM_CR,
  };

  if ((dev == NULL) || (id == NULL)) {
    return -EFAULT;
  }

  if (dev->adc_enabled) {
    return -EBUSY;
  }

  WRITE_CMD_AND_WAIT(dev, cmd);
  POLL_FOR_RESPONSE(dev, response, AFE4400EVM_CMD_RDID);

  memcpy(id, response->dev_id, 4);
  free(response);
  return EXIT_SUCCESS;
}

int afe4400evm_get_fw_ver(afe4400_t* dev, uint8_t *major, uint8_t *minor) {
  struct afe4400evm_resp_fw_version* response;
  struct afe4400evm_cmd_fw_version cmd = {
    .command    = AFE4400EVM_CMD_FW_VER,
    .splitter   = AFE4400EVM_CR,
  };

  if ((dev == NULL) || (major == NULL) || (minor == NULL)) {
    return -EFAULT;
  }

  if (dev->adc_enabled) {
    return -EBUSY;
  }

  WRITE_CMD_AND_WAIT(dev, cmd);
  POLL_FOR_RESPONSE(dev, response, AFE4400EVM_CMD_FW_VER);

  *major = response->major;
  *minor = response->minor;
  free(response);
  return EXIT_SUCCESS;
}

int afe4400evm_board_name(afe4400_t* dev, char* buf, size_t len) {
  char id[4];
  uint8_t major;
  uint8_t minor;
  int res;

  if ((dev == NULL) || (buf == NULL) || (len == 0)) {
    return -EFAULT;
  }

  TRY_RETURN_RES(afe4400evm_get_id(dev, id));
  TRY_RETURN_RES(afe4400evm_get_fw_ver(dev, &major, &minor));

  res = snprintf(buf, len, "AFE%c%c%c%cEVM FW v%d.%d", id[0], id[1], id[2], id[3], major, minor);
  buf[len - 1] = '\0';
  if (res >= len) {
    return -ENOSPC;
  }

  return EXIT_SUCCESS;
}

#define AFE4400EVM_ADC_BITS     (24) /* Depends on setting */
#define AFE4400EVM_ADC_CODE_FS  (1 << AFE4400EVM_ADC_BITS)
#define AFE4400EVM_ADC_CODE_SGN (1 << (AFE4400EVM_ADC_BITS - 1))
double afe4400evm_adc_to_mv(uint32_t adc) {
  int32_t i = adc & (AFE4400EVM_ADC_CODE_FS - 1);

  /* Check sign and turn it negative if needed */
  if ((i & AFE4400EVM_ADC_CODE_SGN) == AFE4400EVM_ADC_CODE_SGN) {
    i -= AFE4400EVM_ADC_CODE_FS;
  }

  return AFE4400EVM_VREF_MV * (double)i / (float)(AFE4400EVM_ADC_CODE_FS / 2);
}

int afe4400evm_write_register_and_verify(afe4400_t* dev, uint8_t reg_addr, uint32_t reg_data) {
  uint32_t reg;

  TRY_RETURN_RES(afe4400evm_write_register(dev, reg_addr, reg_data));
  TRY_RETURN_RES(afe4400evm_read_register(dev, reg_addr, &reg));
  if (reg != reg_data) {
    fprintf(stderr, "%s: reg %s mismatch, want 0x%06x, got 0x%06x\n", __func__, register_names[reg_addr], reg_data, reg);
    return -EROFS;
  }

  return EXIT_SUCCESS;
}

/* Host software reset: set all registers to defaults (not POR values), stop ADC, flush serial, and reset context. */
int afe4400evm_reset(afe4400_t* dev) {
  int res;

  /* Clear buffer stuck at MCU end, so it can respond to new commands. */
  res = afe4400evm_poll_incoming_transactions(dev);
  if ((res != EXIT_SUCCESS) && (res != -ETIMEDOUT)) {
    return res;
  }
  TRY_RETURN_RES(afe4400evm_stop_adc(dev));
  /* Empty once again just in case */
  while ((res = afe4400evm_poll_incoming_transactions(dev)) == EXIT_SUCCESS);
  if (res != -ETIMEDOUT) {
    return res;
  }
  /* Clear meassage queue since anything in it is from previous run. */
  TRY_RETURN_RES(afe4400evm_destory_transactions(dev));

  TRY_RETURN_RES(afe4400evm_write_register(dev, AFE4400EVM_REG_CONTROL0, AFE4400EVM_CONTROL0_SW_RST | AFE4400EVM_CONTROL0_DIAG_EN)); /* Software reset */
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LEDCNTRL,     0x000000)); /* Turn off LED before applying settings */

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_PRPCOUNT,     AFE4400EVM_PRPCOUNT_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED2STC,      AFE4400EVM_LED2STC_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED2ENDC,     AFE4400EVM_LED2ENDC_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ALED2STC,     AFE4400EVM_ALED2STC_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ALED2ENDC,    AFE4400EVM_ALED2ENDC_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED1STC,      AFE4400EVM_LED1STC_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED1ENDC,     AFE4400EVM_LED1ENDC_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ALED1STC,     AFE4400EVM_ALED1STC_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ALED1ENDC,    AFE4400EVM_ALED1ENDC_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED2LEDSTC,   AFE4400EVM_LED2LEDSTC_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED2LEDENDC,  AFE4400EVM_LED2LEDENDC_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED1LEDSTC,   AFE4400EVM_LED1LEDSTC_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED1LEDENDC,  AFE4400EVM_LED1LEDENDC_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ADCRSTCT0,    AFE4400EVM_ADCRSTCT0_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ADCRENDCT0,   AFE4400EVM_ADCRENDCT0_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED2CONVST,   AFE4400EVM_LED2CONVST_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED2CONVEND,  AFE4400EVM_LED2CONVEND_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ADCRSTCT1,    AFE4400EVM_ADCRSTCT1_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ADCRENDCT1,   AFE4400EVM_ADCRENDCT1_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ALED2CONVST,  AFE4400EVM_ALED2CONVST_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ALED2CONVEND, AFE4400EVM_ALED2CONVEND_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ADCRSTCT2,    AFE4400EVM_ADCRSTCT2_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ADCRENDCT2,   AFE4400EVM_ADCRENDCT2_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED1CONVST,   AFE4400EVM_LED1CONVST_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LED1CONVEND,  AFE4400EVM_LED1CONVEND_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ADCRSTCT3,    AFE4400EVM_ADCRSTCT3_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ADCRENDCT3,   AFE4400EVM_ADCRENDCT3_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ALED1CONVST,  AFE4400EVM_ALED1CONVST_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_ALED1CONVEND, AFE4400EVM_ALED1CONVEND_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_CONTROL1,     AFE4400EVM_CONTROL1_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_LEDCNTRL,     AFE4400EVM_LEDCNTRL_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_TIAGAIN,      AFE4400EVM_TIAGAIN_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_TIA_AMB_GAIN, AFE4400EVM_TIA_AMB_GAIN_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_CONTROL2,     AFE4400EVM_CONTROL2_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_CONTROL3,     AFE4400EVM_CONTROL3_DEF));

  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_PDNCYCLESTC,  AFE4400EVM_PDNCYCLESTC_DEF));
  TRY_RETURN_RES(afe4400evm_write_register_and_verify(dev, AFE4400EVM_REG_PDNCYCLEENDC, AFE4400EVM_PDNCYCLEENDC_DEF));

  return EXIT_SUCCESS;
}
