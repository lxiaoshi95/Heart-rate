
//*****************************************************************************
//
// \file MPU9150.c
//
// \brief Provides Functions to initialize the MPU9150
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
#include "MPU9150.h"
#include "HAL_I2C.h"

//*****************************************************************************
//
// Types
//
//*****************************************************************************

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************

//*****************************************************************************
//
// Global Data
//
//*****************************************************************************

//*****************************************************************************
//
// Constants
//
//*****************************************************************************

//*****************************************************************************
//
// Function Prototypes
//
//*****************************************************************************

void initMPU9150 (void)	
{
    uint8_t configData [1];

    HAL_I2C_read(USCI_B0_BASE, MPU6050_DEFAULT_ADDRESS, MPU6050_RA_WHO_AM_I, 
            configData, 1);
    
    // wake up sensor
    configData [0]=0x01; //0x04
    HAL_I2C_write(USCI_B0_BASE, MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_1, 
            configData, 1);
    
    //set acc sensitivity to 8G
    configData [0]=0x10;
    HAL_I2C_write(USCI_B0_BASE, MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 
            configData, 1);
    
    //set DLPF to 21 Hz
    configData [0]=0x04;
    HAL_I2C_write(USCI_B0_BASE, MPU6050_DEFAULT_ADDRESS, MPU6050_RA_CONFIG, 
            configData, 1);
    
    //set sampling to 1kHZ
    configData [0]=0x00;
    HAL_I2C_write(USCI_B0_BASE, MPU6050_DEFAULT_ADDRESS, MPU6050_RA_SMPLRT_DIV, 
            configData, 1);
    
    //Configure int
    configData [0]=0x30;
    HAL_I2C_write(USCI_B0_BASE, MPU6050_DEFAULT_ADDRESS, MPU6050_RA_INT_PIN_CFG, 
            configData, 1);
    
    //Enable data ready int
//    configData [0]=0x01;
//    HAL_I2C_write(USCI_B0_BASE, MPU6050_DEFAULT_ADDRESS, MPU6050_RA_INT_ENABLE, 
//            configData, 1);
}

