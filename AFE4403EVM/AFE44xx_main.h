#ifndef AFE44xx_MAIN_H_
#define AFE44xx_MAIN_H_

#define SOT     0x02
#define EOT     0x03
#define CR      0x0D
#define LF      0x0A
#define CALIBRATION_TRUE        0x54
#define CALIBRATION_FALSE       0x46

#define WRITE_REG_CMD                   0x02
#define READ_REG_CMD                    0x03
#define START_READ_ADC_REG_CMD          0x01
#define STOP_READ_ADC_REG_CMD           0x06
#define DEV_ID_CMD                      0x04
#define FW_UPGRADE_CMD                  0x05
#define FW_VERSION_CMD                  0x07
#define CALIBRATION_CMD                 0x08

#define  AFE44x0_Major_Number 0x02
#define  AFE44x0_Minor_Number 0x02

/**
* @brief Enables battery charging
*
* @param  None
*
* @return  None
*/

void chargerOn();

#endif /*AFE44xx_MAIN_H_*/
