
//*****************************************************************************
//
// \file HAL_I2C.c
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
#ifndef _HAL_I2C_H
#define _HAL_I2C_H

#include "setMCLK.h"

//*****************************************************************************
//
// Types
//
//*****************************************************************************

//*****************************************************************************
//
//! \typedef USCI_I2C_initPortParam
//!
//! I2C port configuration
//
//*****************************************************************************
struct USCI_I2C_initPortParam{
    uint8_t  selectedPort;
    uint16_t selectedPinSDA;
    uint16_t selectedPinSCL;
};

//*****************************************************************************
//
//! \typedef USCI_I2C_Config
//!
//! I2C configuration
//
//*****************************************************************************
struct USCI_I2C_Config{
    uint16_t baseAddress;
    uint8_t  selectClockSource;
    uint32_t desiredi2cClock;
    uint8_t  i2cAddress;
    struct USCI_I2C_initPortParam i2cPortConfig;
};

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************

//*****************************************************************************
//
// Exported variables
//
//*****************************************************************************

//*****************************************************************************
//
//! \var I2C_USCI_B0_Config constant varaiable that holds the USCI B0 
//!      Configuration
//
//*****************************************************************************
extern const struct USCI_I2C_Config I2C_USCI_B0_Config;

//*****************************************************************************
//
// Exported prototypes
//
//*****************************************************************************

//*****************************************************************************
//
//!  initMPU9150ACCInterrupt  
//!
//!  \brief Initialize the MPU9150 ACC Interrupt
//!
//!  \param none
//!
//!  \return none
//
//***************************************************************************** 
extern void initMPU9150ACCInterrupt (void);

//*****************************************************************************
//
//!  HAL_I2C_init  
//!
//!  \brief I2C Initialization
//!
//!  \param paramConfig I2C configuration structure
//!
//!  \return none
//
//***************************************************************************** 
extern void HAL_I2C_init (struct USCI_I2C_Config);

//*****************************************************************************
//
//!  HAL_I2C_write  
//!
//!  \brief multi byte transmission from Master to Slave
//!
//!  \param baseAddress is the base address of the USCI I2C module.
//!
//!  \param slave_addr 7-bit slave address
//!
//!  \param reg Is the register byte to be transmitted
//!
//!  \param data is the data byte to be transmitted
//!
//!  \param byteCount number bytes to be transmitted
//!
//!  \return status of the transmission process.
//
//***************************************************************************** 
extern int8_t HAL_I2C_write(uint16_t baseAddress, uint8_t slave_addr, uint8_t reg, 
        uint8_t *data, uint8_t byteCount);

//*****************************************************************************
//
//!  HAL_I2C_read
//!
//!  \brief byte transmission and multi byte reception from Master to Slave
//!
//!  \param baseAddress is the base address of the USCI I2C module.
//!
//!  \param slave_addr 7-bit slave address
//!
//!  \param reg Is the register byte to be transmitted
//!
//!  \param data is a pointer to the location to store the received byte at
//!   master end
//!
//!  \param byteCount number bytes to be received
//!
//!  \return status of the reception process.
//
//***************************************************************************** 
extern int8_t HAL_I2C_read(uint16_t baseAddress, uint8_t slave_addr, uint8_t reg, 
        uint8_t *data, uint8_t byteCount);

//*****************************************************************************
//
//!  HAL_I2C_read_only
//!
//!  multi byte reception from Master to Slave
//!
//!  \param baseAddress is the base address of the USCI I2C module.
//!
//!  \param slave_addr 7-bit slave address
//!
//!  \param reg Is the register byte to be transmitted
//!
//!  \param data is a pointer to the location to store the received byte at
//!   master end
//!
//!  \param byteCount number bytes to be received
//!
//!  \return status of the reception process.
//
//***************************************************************************** 
extern int8_t HAL_I2C_read_only(uint16_t baseAddress, uint8_t slave_addr, 
        uint8_t reg, uint8_t *data, uint8_t byteCount);

#endif /* _HAL_I2C_H_ */
