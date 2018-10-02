
//*****************************************************************************
//
// \file fuelGauge.c
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
#include <stdint.h>
#include "fuelGauge.h"
#include "HAL_I2C.h"

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
//*****************************************************************************
//
//! \def FG_DEFAULT_ADDR Fuel Gauge address
//
//! \def FG_SOC_REG_ADDR State of Charge register address
//
//*****************************************************************************
#define FG_DEFAULT_ADDR	0x55
#define FG_SOC_REG_ADDR 0x1C

//*****************************************************************************
//
// Global Data
//
//*****************************************************************************

//*****************************************************************************
//
// Function Prototypes
//
//*****************************************************************************

//*****************************************************************************
//
// Imported Data
//
//*****************************************************************************

//*****************************************************************************
//
// Constants
//
//*****************************************************************************

//void chargerOn(void)
//{
//    //HRGR_CE and CHRGR_EN1_EN2 set as output for charging circuitry
//    GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN0);
//
//    //HRGR_CE and CHRGR_EN1_EN2 set as output for charging circuitry
//    GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN1);
//
//    //Pull CHRGR_CE and CHRGR_EN1_EN2 low to enable battery charging
//    GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN0);
//
//    GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN1);
//}

int8_t getSOC(void)
{
    uint8_t stateOfCharge = 0;

    HAL_I2C_read(USCI_B0_BASE, FG_DEFAULT_ADDR, FG_SOC_REG_ADDR, &stateOfCharge, 1);
    return (stateOfCharge);
}

int8_t getAP(void)
{
    uint8_t averagePower = 0;

    HAL_I2C_read(USCI_B0_BASE, FG_DEFAULT_ADDR, 0x18, &averagePower, 1);
    return (averagePower);
}

int8_t getAC(void)
{
    uint8_t averageCurrent = 0;

    HAL_I2C_read(USCI_B0_BASE, FG_DEFAULT_ADDR, 0x10, &averageCurrent, 1);
    return (averageCurrent);
}

uint8_t getVOLT(void)
{
    uint8_t volt = 0;

    HAL_I2C_read(USCI_B0_BASE, FG_DEFAULT_ADDR, 0x04, &volt, 1);
    return (volt);
}

void readFuelGaugeCommand(uint8_t *fuelGaugeMsg)
{
//    HAL_I2C_write(USCI_B0_BASE, FG_DEFAULT_ADDR, fuelGaugeMsg[0], fuelGaugeMsg, 0);
//	__delay_cycles(10000);
    HAL_I2C_read(USCI_B0_BASE, FG_DEFAULT_ADDR, fuelGaugeMsg[0], (fuelGaugeMsg + 1), 2);
}

void readFuelGaugeSubCommand(uint8_t *fuelGaugeMsg)
{
    HAL_I2C_write(USCI_B0_BASE, FG_DEFAULT_ADDR, 0x00, fuelGaugeMsg, 2);
    __delay_cycles(10000);
    HAL_I2C_read(USCI_B0_BASE, FG_DEFAULT_ADDR, 0x00, (fuelGaugeMsg + 2), 2);
    __delay_cycles(2500);
}
 
void writeFuelGaugeSubCommand(uint8_t *fuelGaugeMsg)
{
    HAL_I2C_write(USCI_B0_BASE, FG_DEFAULT_ADDR, 0x00, fuelGaugeMsg, 2);
}

void softResetFuelGauge(void)
{
    uint8_t fuelGaugeMsg[2] = {0x42,0x00};

    HAL_I2C_write(USCI_B0_BASE, FG_DEFAULT_ADDR, 0x00, fuelGaugeMsg, 2);
}

void batteryInsertFuelGauge(void)
{
    uint8_t fuelGaugeMsg[2] = {0x0C,0x00};

    HAL_I2C_write(USCI_B0_BASE, FG_DEFAULT_ADDR, 0x00, fuelGaugeMsg, 2);
}

//*****************************************************************************
//
//!  getTEMP
//!
//!  \brief Reads the battery Temperature
//!
//!  \param none
//!
//!  \return unsigned integer value
//
//*****************************************************************************
uint8_t getTEMP(void)
{
    uint8_t tempBuffer[2] = {0,0};

    HAL_I2C_read(USCI_B0_BASE, FG_DEFAULT_ADDR, 0x02, &tempBuffer[0], 2);
    return (tempBuffer[0]);
}
