#ifndef __AFE4400EVM_DRIVER_INTERNALS_H__
#define __AFE4400EVM_DRIVER_INTERNALS_H__

#include <stdint.h>

/* Basic protocal */
#define AFE4400EVM_SOT           0x02
#define AFE4400EVM_EOT           0x03
#define AFE4400EVM_CR            0x0d
#define AFE4400EVM_LF            0x0a
#define AFE4400EVM_CAL_TRUE      0x54 /* 'T' */
#define AFE4400EVM_CAL_FALSE     0x46 /* 'F' */
#define AFE4400EVM_ADC_DUMMY     0x2a

/* Commands */
#define AFE4400EVM_CMD_REG_WRITE 0x02
#define AFE4400EVM_CMD_REG_READ  0x03
#define AFE4400EVM_CMD_ADC_START 0x01
#define AFE4400EVM_CMD_ADC_STOP  0x06
#define AFE4400EVM_CMD_FW_UPG    0x05
#define AFE4400EVM_CMD_FW_VER    0x07
#define AFE4400EVM_CMD_RDID      0x04
#define AFE4400EVM_CMD_CAL       0x08


/* Command and response formats */
/* Most data are 24-bit since the ADC is 20/22 bit. */
struct __attribute__ ((__packed__)) afe4400evm_cmd_reg_write {
  uint8_t  command;         /* AFE4400EVM_CMD_REG_WRITE */
  char     reg_addr[2];     /* ASCII, MSB first */
  char     reg_data[6];     /* ASCII, MSB first */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_cmd_reg_read {
  uint8_t  command;         /* AFE4400EVM_CMD_REG_READ */
  char     reg_addr[2];     /* ASCII, MSB first */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_resp_reg_read {
  uint8_t  command;         /* AFE4400EVM_CMD_REG_READ */
  uint8_t  sot;             /* AFE4400EVM_SOT */
  uint8_t  reg_data[3];     /* LSB first */
  uint8_t  eot;             /* AFE4400EVM_EOT */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_cmd_adc_start {
  uint8_t  command;         /* AFE4400EVM_CMD_ADC_START */
  uint8_t  dummy;           /* Must be 0x2a according to "7357.Message Communication Protocol v3.0 - AFE4403EVM.pdf" posted on support forum. */
  uint8_t  count[4];        /* MSB first, 0 = continuous mode */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_cmd_adc_stop {
  uint8_t  command;         /* AFE4400EVM_CMD_ADC_STOP */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_resp_adc {
  uint8_t  command;         /* AFE4400EVM_CMD_ADC_START */
  uint8_t  sot;             /* AFE4400EVM_SOT */
  uint8_t  red[3];          /* LSB first */
  uint8_t  red_ambient[3];  /* LSB first */
  uint8_t  ir[3];           /* LSB first */
  uint8_t  ir_ambient[3];   /* LSB first */
  uint8_t  red_net[3];      /* LSB first, red - red_ambient */
  uint8_t  ir_net[3];       /* LSB first, ir - ir_ambient */
  uint8_t  eot;             /* AFE4400EVM_EOT */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_cmd_fw_update {
  uint8_t  command;         /* AFE4400EVM_CMD_FW_UPG */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_cmd_fw_version {
  uint8_t  command;         /* AFE4400EVM_CMD_FW_VER */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_resp_fw_version {
  uint8_t  command;         /* AFE4400EVM_CMD_FW_VER */
  uint8_t  sot;             /* AFE4400EVM_SOT */
  uint8_t  major;
  uint8_t  minor;
  uint8_t  eot;             /* AFE4400EVM_EOT */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_cmd_rdid {
  uint8_t  command;         /* AFE4400EVM_CMD_RDID */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_resp_rdid {
  uint8_t  command;         /* AFE4400EVM_CMD_RDID */
  uint8_t  sot;             /* AFE4400EVM_SOT */
  char     dev_id[4];       /* 4-digit ASCII */
  uint8_t  eot;             /* AFE4400EVM_EOT */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

struct __attribute__ ((__packed__)) afe4400evm_cmd_cal {
  uint8_t  command;         /* AFE4400EVM_CMD_RDID */
  uint8_t  cal_enable;      /* AFE4400EVM_CAL_TRUE / AFE4400EVM_CAL_FALSE */
  uint8_t  splitter;        /* AFE4400EVM_CR */
};

#endif /* __AFE4400EVM_DRIVER_INTERNALS_H__ */
