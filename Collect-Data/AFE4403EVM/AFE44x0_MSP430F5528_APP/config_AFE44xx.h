
//*****************************************************************************
//
// \file config_AFE44xx.h
//
// \brief 
//
// Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
//*****************************************************************************
#ifndef CONFIG_AFE44XX_H_
#define CONFIG_AFE44XX_H_

//*****************************************************************************
//
// Types
//
//*****************************************************************************
struct AFE44xx_GPIO{
  uint8_t selectedPort;
  uint16_t selectedPin;
};

struct AFE44xx_PORT_ISR{
  uint8_t selectedPort;
  uint16_t selectedPin;
};

struct AFE44xx_SPI{
  uint16_t baseAddress;
  uint8_t  selectClockSource;
  uint32_t desiredSpiClock;
  uint8_t  msbFirst;
  uint8_t  clockPhase;
  uint8_t  clockPolarity;
  struct   AFE44xx_GPIO SPI_Tx;
  struct   AFE44xx_GPIO SPI_Rx;
  struct   AFE44xx_GPIO SPI_Clk;
  struct   AFE44xx_GPIO SPI_CS;
};

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
#define DATA_LOGGER_PACKET_SIZE             14 //Number of bytes per HRM packet: 6 AFE, 6 MPU9150 , 1 HR
#define MPU6050_RA_ACCEL_XOUT_H     		0x3B
#define MPU6050_ADDRESS_AD0_LOW        		0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_DEFAULT_ADDRESS        		MPU6050_ADDRESS_AD0_LOW


//
// Events passed back to the application
//
#define CONTROL0 0x00
#define REDSTARTCOUNT 0x01
#define REDENDCOUNT 0x02
#define REDLEDSTARTCOUNT 0x03
#define REDLEDENDCOUNT 0x04
#define AMBREDSTARTCOUNT 0x05
#define AMBREDENDCOUNT 0x06
#define IRSTARTCOUNT 0x07
#define IRENDCOUNT 0x08
#define IRLEDSTARTCOUNT 0x09
#define IRLEDENDCOUNT 0x0A
#define AMBIRSTARTCOUNT 0x0B
#define AMBIRENDCOUNT 0x0C
#define REDCONVSTART 0x0D
#define REDCONVEND 0x0E
#define AMBREDCONVSTART 0x0F
#define AMBREDCONVEND 0x10
#define IRCONVSTART 0x11
#define IRCONVEND 0x12
#define AMBIRCONVSTART 0x13
#define AMBIRCONVEND 0x14
#define ADCRESETSTCOUNT0 0x15
#define ADCRESETENDCOUNT0 0x16
#define ADCRESETSTCOUNT1 0x17
#define ADCRESETENDCOUNT1 0x18
#define ADCRESETSTCOUNT2 0x19
#define ADCRESETENDCOUNT2 0x1A
#define ADCRESETSTCOUNT3 0x1B
#define ADCRESETENDCOUNT3 0x1C
#define PRPCOUNT 0x1D
#define CONTROL1 0x1E
#define TIAGAIN 0x20
#define TIA_AMB_GAIN 0x21
#define LEDCNTRL 0x22
#define CONTROL2 0x23
#define MODE 0x25
#define ALARM 0x29
#define REDVALUE 0x2A
#define AMBREDVALUE 0x2B
#define IRVALUE 0x2C
#define AMBIRVALUE 0x2D
#define REDMINUSAMBREDVALUE 0x2E
#define IRMINUSAMBIRVALUE 0x2F
#define DIAGNOSTICS 0x30
#define CONTROL3 0x31

//*****************************************************************************
//
// Exported variables
//
//*****************************************************************************
extern const uint8_t P4mapping[];

//*****************************************************************************
//
// Exported prototypes
//
//*****************************************************************************
extern void readAccelerometerData(uint8_t *);
extern void initAFE44xxCommInterface (void);

#endif
