#ifndef __AFE4400EVM_DRIVER_H__
#define __AFE4400EVM_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <libusb-1.0/libusb.h>

/********** Macros **********/
/***** Constants *****/
#define AFE4400EVM_VID              (0x2047)
#define AFE4400EVM_PID              (0x0300)
#define AFE4400EVM_BULK_SIZE        (64) /* USB 2.0 Full-speed */
#define AFE4400EVM_EP_IN_ADDR       (0x82)
#define AFE4400EVM_EP_OUT_ADDR      (0x02)
#define AFE4400EVM_ADC_MAX          (16777215) /* 2 ^ 24 - 1 */
#define AFE4400EVM_TIMEOUT          (20) /* ms */
#define AFE4400EVM_REG_LAST         (0x33)
#define AFE4400EVM_WAIT_TIME        (5) /* ms. A value too small can cause the firmware to hang (write) or throw the request away (read). */
#define AFE4400EVM_VREF_MV          (1200 << 2)
#define AFE4400EVM_MAX_TRANS_LEN    (30) /* Max transaction has length of 22. */
/***** Register addresses *****/
/* Dummies */
#define AFE4400EVM_REG_SPARE1       (0x1f)
#define AFE4400EVM_REG_SPARE2       (0x24)
#define AFE4400EVM_REG_SPARE3       (0x25)
#define AFE4400EVM_REG_SPARE4       (0x26)
#define AFE4400EVM_REG_RESERVED1    (0x27)
#define AFE4400EVM_REG_RESERVED2    (0x28)
/* Controls */
#define AFE4400EVM_REG_CONTROL0     (0x00)
#define AFE4400EVM_REG_CONTROL1     (0x1e)
#define AFE4400EVM_REG_CONTROL2     (0x23)
#define AFE4400EVM_REG_CONTROL3     (0x31)
#define AFE4400EVM_REG_LEDCNTRL     (0x22)
#define AFE4400EVM_REG_PRPCOUNT     (0x1d)
/* Values */
#define AFE4400EVM_REG_LED2VAL      (0x2a)
#define AFE4400EVM_REG_ALED2VAL     (0x2b)
#define AFE4400EVM_REG_LED1VAL      (0x2c)
#define AFE4400EVM_REG_ALED1VAL     (0x2d)
#define AFE4400EVM_REG_LED2NET      (0x2e)
#define AFE4400EVM_REG_LED1NET      (0x2f)
/* LED 1 timing */
#define AFE4400EVM_REG_LED1STC      (0x07)
#define AFE4400EVM_REG_LED1ENDC     (0x08)
#define AFE4400EVM_REG_LED1LEDSTC   (0x09)
#define AFE4400EVM_REG_LED1LEDENDC  (0x0a)
#define AFE4400EVM_REG_ALED1STC     (0x0b)
#define AFE4400EVM_REG_ALED1ENDC    (0x0c)
/* LED 2 timing */
#define AFE4400EVM_REG_LED2STC      (0x01)
#define AFE4400EVM_REG_LED2ENDC     (0x02)
#define AFE4400EVM_REG_LED2LEDSTC   (0x03)
#define AFE4400EVM_REG_LED2LEDENDC  (0x04)
#define AFE4400EVM_REG_ALED2STC     (0x05)
#define AFE4400EVM_REG_ALED2ENDC    (0x06)
/* LED 1 conversion timing */
#define AFE4400EVM_REG_LED1CONVST   (0x11)
#define AFE4400EVM_REG_LED1CONVEND  (0x12)
#define AFE4400EVM_REG_ALED1CONVST  (0x13)
#define AFE4400EVM_REG_ALED1CONVEND (0x14)
/* LED 2 conversion timing */
#define AFE4400EVM_REG_LED2CONVST   (0x0d)
#define AFE4400EVM_REG_LED2CONVEND  (0x0e)
#define AFE4400EVM_REG_ALED2CONVST  (0x0f)
#define AFE4400EVM_REG_ALED2CONVEND (0x10)
/* ADC timing */
#define AFE4400EVM_REG_ADCRSTCT0    (0x15)
#define AFE4400EVM_REG_ADCRENDCT0   (0x16)
#define AFE4400EVM_REG_ADCRSTCT1    (0x17)
#define AFE4400EVM_REG_ADCRENDCT1   (0x18)
#define AFE4400EVM_REG_ADCRSTCT2    (0x19)
#define AFE4400EVM_REG_ADCRENDCT2   (0x1a)
#define AFE4400EVM_REG_ADCRSTCT3    (0x1b)
#define AFE4400EVM_REG_ADCRENDCT3   (0x1c)
/* PD settings */
#define AFE4400EVM_REG_PDNCYCLESTC  (0x32)
#define AFE4400EVM_REG_PDNCYCLEENDC (0x33)
/* TIA settings */
#define AFE4400EVM_REG_TIAGAIN      (0x20)
#define AFE4400EVM_REG_TIA_AMB_GAIN (0x21)
/* Reporting */
#define AFE4400EVM_REG_ALARM        (0x29)
#define AFE4400EVM_REG_DIAG         (0x30)
/***** Register values *****/
/* Defaults (according to example in datasheet, not POR values) */
#define AFE4400EVM_CONTROL0_DEF     (0x000000)
#define AFE4400EVM_CONTROL1_DEF     (0x000101)
#define AFE4400EVM_CONTROL2_DEF     (0x000000)
#define AFE4400EVM_CONTROL3_DEF     (0x008000)
//#define AFE4400EVM_LEDCNTRL_DEF     (0x011414)
#define AFE4400EVM_LEDCNTRL_DEF     (0x012020)
#define AFE4400EVM_PRPCOUNT_DEF     (0x001f3f)

#define AFE4400EVM_LED1STC_DEF      (0x000820)
#define AFE4400EVM_LED1ENDC_DEF     (0x000f9e)
#define AFE4400EVM_LED1LEDSTC_DEF   (0x0007d0)
#define AFE4400EVM_LED1LEDENDC_DEF  (0x000f9f)
#define AFE4400EVM_ALED1STC_DEF     (0x000ff0)
#define AFE4400EVM_ALED1ENDC_DEF    (0x00176e)

#define AFE4400EVM_LED2STC_DEF      (0x0017c0)
#define AFE4400EVM_LED2ENDC_DEF     (0x001f3e)
#define AFE4400EVM_LED2LEDSTC_DEF   (0x001770)
#define AFE4400EVM_LED2LEDENDC_DEF  (0x001f3f)
#define AFE4400EVM_ALED2STC_DEF     (0x000050)
#define AFE4400EVM_ALED2ENDC_DEF    (0x0007ce)

#define AFE4400EVM_LED1CONVST_DEF   (0x000fa6)
#define AFE4400EVM_LED1CONVEND_DEF  (0x00176f)
#define AFE4400EVM_ALED1CONVST_DEF  (0x001776)
#define AFE4400EVM_ALED1CONVEND_DEF (0x001f3f)

#define AFE4400EVM_LED2CONVST_DEF   (0x000006)
#define AFE4400EVM_LED2CONVEND_DEF  (0x0007cf)
#define AFE4400EVM_ALED2CONVST_DEF  (0x0007d6)
#define AFE4400EVM_ALED2CONVEND_DEF (0x000f9f)

#define AFE4400EVM_ADCRSTCT0_DEF    (0x000000)
#define AFE4400EVM_ADCRENDCT0_DEF   (0x000005)
#define AFE4400EVM_ADCRSTCT1_DEF    (0x0007d0)
#define AFE4400EVM_ADCRENDCT1_DEF   (0x0007d5)
#define AFE4400EVM_ADCRSTCT2_DEF    (0x000fa0)
#define AFE4400EVM_ADCRENDCT2_DEF   (0x000fa5)
#define AFE4400EVM_ADCRSTCT3_DEF    (0x001770)
#define AFE4400EVM_ADCRENDCT3_DEF   (0x001775)

#define AFE4400EVM_PDNCYCLESTC_DEF  (0x000000)
#define AFE4400EVM_PDNCYCLEENDC_DEF (0x000000)

//#define AFE4400EVM_TIAGAIN_DEF      (0x000000)
#define AFE4400EVM_TIAGAIN_DEF      (0x00803b) /* 10k, 195pF 0x0000f5?? 5 is right but 195 is wrong  100k 100pf 7a*/
#define AFE4400EVM_TIA_AMB_GAIN_DEF (0x060002) /*raw: 0x000002, 250pF, R: None*/

#define AFE4400EVM_ALARM_DEF        (0x000000)

/* Bit fields */
#define AFE4400EVM_CONTROL0_SW_RST  (1 << 3)
#define AFE4400EVM_CONTROL0_DIAG_EN (1 << 2)
#define AFE4400EVM_CONTROL0_TIM_COUNT_RST (1 << 1)
#define AFE4400EVM_CONTROL0_SPI_READ (1 << 0) /* Not used by upper layer */
#define AFE4400EVM_TIA_AMB_GAIN_FSEL (1 << 15) /* Filter corner frequency selection */
#define AFE4400EVM_TIA_AMB_GAIN_S2EN (1 << 14) /* Stage 2 enable */

// TODO: MIN/MAX/POR/BITs/MASKs

/********** Types **********/
/* NOTE: These are for single-threaded use only. */
typedef struct afe4400evm_transcation {
  uint64_t id; /* Takes 200 years to overflow at 3GHz addition rate */
  uint8_t type;
  size_t len;
  void* record;
  struct afe4400evm_transcation* next;
} afe4400evm_transcation_t;

typedef struct {
  struct libusb_device_handle* devh;
  bool adc_enabled;
  afe4400evm_transcation_t* rxq;
  afe4400evm_transcation_t* rxq_tail;
} afe4400_t;

typedef struct {
  uint32_t red;
  uint32_t red_ambient;
  uint32_t ir;
  uint32_t ir_ambient;
  uint32_t red_net;
  uint32_t ir_net;
} afe4400_result_t;

/********** Fields **********/
extern char* register_names[AFE4400EVM_REG_LAST + 1]; /* Converts register address to name. */

/********** Functions **********/
/* Basics */
afe4400_t* afe4400evm_init(void);
int        afe4400evm_cleanup(afe4400_t* dev);
int        afe4400evm_start_adc(afe4400_t* dev, bool continuous, int count);
int        afe4400evm_stop_adc(afe4400_t* dev);
int        afe4400evm_poll_adc(afe4400_t* dev, afe4400_result_t* result);
double     afe4400evm_adc_to_mv(uint32_t adc);
int        afe4400evm_set_calibration(afe4400_t* dev, bool enabled);
int        afe4400evm_enter_bootloader(afe4400_t* dev);
/* Low-level */
int        afe4400evm_read_register(afe4400_t* dev, uint8_t reg_addr, uint32_t *reg_data);
int        afe4400evm_write_register(afe4400_t* dev, uint8_t reg_addr, uint32_t reg_data);
int        afe4400evm_get_id(afe4400_t* dev, char id[4]);
int        afe4400evm_get_fw_ver(afe4400_t* dev, uint8_t *major, uint8_t *minor);
int        afe4400evm_write_register_and_verify(afe4400_t* dev, uint8_t reg_addr, uint32_t reg_data);
/* High-level */
int        afe4400evm_board_name(afe4400_t* dev, char* buf, size_t len);
int        afe4400evm_reset(afe4400_t* dev);

#endif /* __AFE4400EVM_DRIVER_H__ */
