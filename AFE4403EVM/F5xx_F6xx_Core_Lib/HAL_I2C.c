
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
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib.h"
#include "MPU9150.h"
#include "HAL_I2C.h"

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
//*****************************************************************************
//
//! \def USCI_I2C_STATUS_SLAVE_NACK
//
//*****************************************************************************
#define USCI_I2C_STATUS_SLAVE_NACK    1

//*****************************************************************************
//
// Global Data
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
const struct USCI_I2C_Config I2C_USCI_B0_Config = {
        .baseAddress = __MSP430_BASEADDRESS_USCI_B0__,
        .selectClockSource = USCI_I2C_CLOCKSOURCE_SMCLK,
        .desiredi2cClock = USCI_B_I2C_SET_DATA_RATE_400KBPS,
        .i2cAddress = 0,
        .i2cPortConfig.selectedPort = GPIO_PORT_P3,
        .i2cPortConfig.selectedPinSDA = GPIO_PIN0,
        .i2cPortConfig.selectedPinSCL = GPIO_PIN1,
};

//*****************************************************************************
//
// Function Prototypes
//
//*****************************************************************************

void initMPU9150ACCInterrupt (void)
{
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN1);           // P2.1 as input                                
    //    GPIO_setAsInputPinWithPullUpresistor(GPIO_PORT_P2,
    //                                         GPIO_PIN1);       // Enable P2.1 internal resistance
    //    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);      // Set P2.1 as pull-Up resistance
    GPIO_interruptEdgeSelect(GPIO_PORT_P2,GPIO_PIN1,
            GPIO_LOW_TO_HIGH_TRANSITION); // P2.1 Lo/Hi edge
    GPIO_clearInterruptFlag(GPIO_PORT_P2,GPIO_PIN1);       // P2.1 IFG cleared
    //    GPIO_enableInterrupt(GPIO_PORT_P2,GPIO_PIN1);          // P2.1 interrupt enabled

}

void HAL_I2C_init (struct USCI_I2C_Config paramConfig)
{
    USCI_I2C_initMasterParam param = {0};
    uint32_t locUCSGetClock = 0;

    switch(paramConfig.selectClockSource)
    {
        case USCI_I2C_CLOCKSOURCE_ACLK:
            locUCSGetClock = UCS_getACLK();
            break;

        case USCI_I2C_CLOCKSOURCE_SMCLK:
            locUCSGetClock = UCS_getSMCLK();
            break;

        default:
            locUCSGetClock = UCS_getACLK();
            break;
    }

    param.selectClockSource = paramConfig.selectClockSource;
    param.i2cClk = locUCSGetClock;
    param.dataRate = paramConfig.desiredi2cClock;

    GPIO_setAsPeripheralModuleFunctionInputPin( paramConfig.i2cPortConfig.selectedPort, 
            paramConfig.i2cPortConfig.selectedPinSDA +
            paramConfig.i2cPortConfig.selectedPinSCL);

    USCI_I2C_initMaster(paramConfig.baseAddress, &param);
    USCI_I2C_enable(paramConfig.baseAddress);
}

int8_t HAL_I2C_write(uint16_t baseAddress, uint8_t slave_addr, uint8_t reg, uint8_t *data, uint8_t byteCount)
{
    uint8_t comres = 0;

    // WAIT FOR PREVIOUS TRAFFIC TO CLEAR
    while(HWREG8(baseAddress + OFS_UCBxSTAT) & UCBBUSY);

    // LOAD THE DEVICE SLAVE ADDRESS
    HWREG16(baseAddress + OFS_UCBxI2CSA) = (slave_addr);

    // ENABLE TRANSMIT, GENERATE START BIT
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTR + UCTXSTT;

    // WAIT FOR FIRST INT
    while(!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG));

    // LOAD THE REGISTER ADDRESS
    HWREG8(baseAddress + OFS_UCBxTXBUF) = reg;

    // NOW WAIT FOR START BIT TO COMPLETE
    while(HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTT);

    // CHECK IF SLAVE ACK/NACK
    if((HWREG8(baseAddress + OFS_UCBxIFG) & UCNACKIFG))
    {
        // IF NACK, SET STOP BIT AND EXIT
        HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;
        return(USCI_I2C_STATUS_SLAVE_NACK);
    }

    // NOW WRITE ONE OR MORE DATA BYTES
    while(1)
    {
        // WAIT FOR NEXT INT AFTER REGISTER BYTE
        while(!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG));

        // IF NOT DATA TO FOLLOW, THEN WE ARE DONE
        if(byteCount == 0 )
        {
            HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;

            return(comres);
        }
        // IF MORE, SEND THE NEXT BYTE
        else
        {
            HWREG8(baseAddress + OFS_UCBxTXBUF) = *data++;
        }
        byteCount--;
    }
}

int8_t HAL_I2C_read(uint16_t baseAddress, uint8_t slave_addr, uint8_t reg, uint8_t *data, uint8_t byteCount)
{
    uint8_t comres = 0;

    //WAIT FOR PREVIOUS TRAFFIC TO CLEAR
    while(HWREG8(baseAddress + OFS_UCBxSTAT) & UCBBUSY);
    // No Problem
    // LOAD THE DEVICE SLAVE ADDRESS
    HWREG16(baseAddress + OFS_UCBxI2CSA) = (slave_addr);
    // No Problem
    // ENABLE TRANSMIT, GENERATE START BIT
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTR + UCTXSTT;
    // No Problem
    // WAIT FOR FIRST INT
    while(!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG));
    // No Problem
    // LOAD THE REGISTER ADDRESS
    HWREG8(baseAddress + OFS_UCBxTXBUF) = reg;
    // No Problem
    // NOW WAIT FOR START BIT TO COMPLETE
    while(HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTT);
    P1OUT |= BIT4;
    // CHECK IF SLAVE ACK/NACK
    if((HWREG8(baseAddress + OFS_UCBxIFG) & UCNACKIFG))
    {
        // IF NACK, SET STOP BIT AND EXIT
        HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;
        return(USCI_I2C_STATUS_SLAVE_NACK);
    }

    // NOW PREPARE TO READ DATA FROM SLAVE
    // TURN OFF TRANSMIT (ENABLE RECEIVE)
    HWREG8(baseAddress + OFS_UCBxCTL1) &= ~UCTR;
//=========================================================================================================//
    // GENERATE (RE-)START BIT
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTT;

    // WAIT FOR START BIT TO COMPLETE
    while(HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTT);

    // CHECK IF SLAVE ACK/NACK
    if((HWREG8(baseAddress + OFS_UCBxIFG) & UCNACKIFG))
    {
        // IF NACK, SET STOP BIT AND EXIT
        HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;
        return(USCI_I2C_STATUS_SLAVE_NACK);
    }
//=========================================================================================================//
    // NOW READ ONE OR MORE DATA BYTES
    while(byteCount)
    {
        // IF READING 1 BYTE (OR LAST BYTE), GENERATE STOP BIT NOW TO MEET SPEC
        if(byteCount-- == 1)
        {
            HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;
        }

        // WAIT FOR NEXT RX INT
        while(!(HWREG8(baseAddress + OFS_UCBxIFG) & UCRXIFG));

        // READ THE BYTE
        *data++ = HWREG8(baseAddress + OFS_UCBxRXBUF);
    }

    return(comres);
}

int8_t HAL_I2C_read_only(uint16_t baseAddress, uint8_t slave_addr, uint8_t reg, uint8_t *data, uint8_t byteCount)
{
    uint8_t comres = 0;

    //WAIT FOR PREVIOUS TRAFFIC TO CLEAR
    while(HWREG8(baseAddress + OFS_UCBxSTAT) & UCBBUSY);

    // LOAD THE DEVICE SLAVE ADDRESS
    HWREG16(baseAddress + OFS_UCBxI2CSA) = (slave_addr);

    // TURN OFF TRANSMIT (ENABLE RECEIVE)
    HWREG8(baseAddress + OFS_UCBxCTL1) &= ~UCTR;

    // GENERATE START BIT
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTT;

    // WAIT FOR START BIT TO COMPLETE
    while(HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTT);

    // CHECK IF SLAVE ACK/NACK
    if((HWREG8(baseAddress + OFS_UCBxIFG) & UCNACKIFG))
    {
        // IF NACK, SET STOP BIT AND EXIT
        HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;
        return(USCI_I2C_STATUS_SLAVE_NACK);
    }

    // NOW READ ONE OR MORE DATA BYTES
    while(byteCount)
    {
        // IF READING 1 BYTE (OR LAST BYTE), GENERATE STOP BIT NOW TO MEET SPEC
        if(byteCount-- == 1)
        {
            HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;
        }

        // WAIT FOR NEXT RX INT
        while(!(HWREG8(baseAddress + OFS_UCBxIFG) & UCRXIFG));

        // READ THE BYTE
        *data++ = HWREG8(baseAddress + OFS_UCBxRXBUF);
    }

    return(comres);
}
