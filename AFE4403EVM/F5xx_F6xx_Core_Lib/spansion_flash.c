
//*****************************************************************************
//
// \file spansion_flash.c
//
// \brief Low Lever Driver for the Spansion Flash Memory
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
#include "msp430.h"
#include "spansion_flash.h"
#include "usci_spi.h"

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
spansionMemNum_t g_SpansionMemSelected;
bool     g_bErrorFlashErase;
uint32_t g_ui32WriteBaseAddr = SPANSION_BASE_ADDRESS;
uint32_t g_ui32VirtualBaseAddr = SPANSION_BASE_ADDRESS;
uint32_t g_ui32ReadBaseAddr = SPANSION_BASE_ADDRESS;

//*****************************************************************************
//
//! \var g_ui8spansionMfrID Variable to store the Manufacturer ID
//
//*****************************************************************************
uint8_t g_ui8SpansionMfrID;

//*****************************************************************************
//
//! \var g_ui8spansionDevID Variable to store the Device ID
//
//*****************************************************************************
uint8_t g_ui8SpansionDevID;

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
void waitForSpiToComplete (uint16_t baseAddress);

void spansionFlashInit (void)
{
    uint32_t locUCSGetClock = 0;
    spansionMemNum_t memoryIndex;

    // MEM_WPZ output and pulled high
    GPIO_setAsOutputPin(spansionSpiConfig.spiMemWPZ.selectedPort,
            spansionSpiConfig.spiMemWPZ.selectedPin);
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiMemWPZ.selectedPort,
            spansionSpiConfig.spiMemWPZ.selectedPin);

    // MEM_HOLDZ output and pulled high
    GPIO_setAsOutputPin(spansionSpiConfig.spiMemHOLDZ.selectedPort,
            spansionSpiConfig.spiMemHOLDZ.selectedPin);
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiMemHOLDZ.selectedPort,
            spansionSpiConfig.spiMemHOLDZ.selectedPin);

    // Configure SPI's Tx and CLK
    GPIO_setAsPeripheralModuleFunctionOutputPin(spansionSpiConfig.spiTx.selectedPort,
            spansionSpiConfig.spiTx.selectedPin);
    GPIO_setAsPeripheralModuleFunctionOutputPin(spansionSpiConfig.spiClk.selectedPort,
            spansionSpiConfig.spiClk.selectedPin);

    // Configure SPI's Rx
    GPIO_setAsPeripheralModuleFunctionOutputPin(spansionSpiConfig.spiRx.selectedPort,
            spansionSpiConfig.spiRx.selectedPin);

    // Configure SPI's CSx
    for (memoryIndex = SPANSION_0; memoryIndex < TOTAL_SPANSION_MEMORIES; memoryIndex++)
    {
        GPIO_setAsOutputPin(spansionSpiConfig.spiCs[memoryIndex].selectedPort,
                spansionSpiConfig.spiCs[memoryIndex].selectedPin);
        GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[memoryIndex].selectedPort,
                spansionSpiConfig.spiCs[memoryIndex].selectedPin);
    }

    switch(spansionSpiConfig.selectClockSource)
    {
        case USCI_SPI_CLOCKSOURCE_ACLK:
            locUCSGetClock = UCS_getACLK();
            break;

        case USCI_SPI_CLOCKSOURCE_SMCLK:
            locUCSGetClock = UCS_getSMCLK();
            break;

        default:
            locUCSGetClock = UCS_getACLK();
            break;
    }

    // Configure SPI
    USCI_SPI_masterInit(spansionSpiConfig.baseAddress,
            spansionSpiConfig.selectClockSource,
            locUCSGetClock,
            spansionSpiConfig.desiredSpiClock,
            spansionSpiConfig.msbFirst,
            spansionSpiConfig.clockPhase,
            spansionSpiConfig.clockPolarity);

    USCI_SPI_enable(spansionSpiConfig.baseAddress);

    // Default to SPANSION 0
    g_SpansionMemSelected = SPANSION_0;
    g_bErrorFlashErase = false;
}

void spansionFlashReadID (spansionMemNum_t selectedMemory)
{
    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, READ_ID);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Send 3 bytes of address to be written
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Send 2 dummy bytes to read back
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);
    g_ui8SpansionMfrID = USCI_SPI_receiveData(spansionSpiConfig.baseAddress);

    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);
    g_ui8SpansionDevID = USCI_SPI_receiveData(spansionSpiConfig.baseAddress);

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);
}

uint8_t spansionFlashReadSR1 (spansionMemNum_t selectedMemory)
{
    uint8_t statusReg;

    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, RDSR1);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Send 1 dummy byte to read back
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);
    statusReg = USCI_SPI_receiveData(spansionSpiConfig.baseAddress);

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    return (statusReg);
}

uint8_t spansionFlashReadSR2 (spansionMemNum_t selectedMemory)
{
    uint8_t statusReg;

    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, RDSR2);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Send 1 dummy byte to read back
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);
    statusReg = USCI_SPI_receiveData(spansionSpiConfig.baseAddress);

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    return (statusReg);
}

uint8_t spansionFlashReadReg (spansionMemNum_t selectedMemory, uint8_t readReg, uint8_t readPos)
{
    uint8_t statusReg;
    uint8_t locIndex;

    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, readReg);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    for (locIndex = 0; locIndex < readPos; locIndex++)
    {
        // Send 1 dummy byte to read back
        USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
        waitForSpiToComplete(spansionSpiConfig.baseAddress);
        statusReg = USCI_SPI_receiveData(spansionSpiConfig.baseAddress);
    }

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    return (statusReg);
}

void spansionFlashWriteEnable (spansionMemNum_t selectedMemory)
{
    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, WREN);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);
}

void spansionFlashWriteDisable (spansionMemNum_t selectedMemory)
{
    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, WRDI);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);
}

void spansionFlashClearSR (spansionMemNum_t selectedMemory)
{
    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, CLSR);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);
}

uint8_t spansionFlashBulkErase (void)
{
    uint8_t readSR1;
    uint8_t returnStatus = SPANSION_FLASH_STATUS_OK;
    spansionMemNum_t memoryIndex;

    for (memoryIndex = SPANSION_0; memoryIndex < TOTAL_SPANSION_MEMORIES;
            memoryIndex++)
    {
        spansionFlashWriteEnable(memoryIndex);

        // Pull CS low
        GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[memoryIndex].selectedPort,
                spansionSpiConfig.spiCs[memoryIndex].selectedPin);

        // Specify command
        USCI_SPI_transmitData(spansionSpiConfig.baseAddress, BE);
        waitForSpiToComplete(spansionSpiConfig.baseAddress);

        // Pull CS High
        GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[memoryIndex].selectedPort,
                spansionSpiConfig.spiCs[memoryIndex].selectedPin);
    }

    for (memoryIndex = SPANSION_0; memoryIndex < TOTAL_SPANSION_MEMORIES;
            memoryIndex++)
    {
        while( (readSR1 = spansionFlashReadSR1(memoryIndex)) & WIP )
        {
            if (readSR1 & E_ERR)
            {
                returnStatus = SPANSION_FLASH_STATUS_ERROR;
                break;
            }
        }

        if(returnStatus == SPANSION_FLASH_STATUS_ERROR )
        {
            break;
        }
    }
    return (returnStatus);
}

uint8_t spansionFlashSectorErase (spansionMemNum_t selectedMemory)
{
    uint8_t locIndex;
    uint8_t readSR1;
    uint8_t returnStatus = SPANSION_FLASH_STATUS_OK;

    for (locIndex = 0; locIndex < 16; locIndex++)
    {
        spansionFlashWriteEnable(selectedMemory);

        // Pull CS low
        GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
                spansionSpiConfig.spiCs[selectedMemory].selectedPin);

        // Specify command
        USCI_SPI_transmitData(spansionSpiConfig.baseAddress, SE);
        waitForSpiToComplete(spansionSpiConfig.baseAddress);

        // Send 3 bytes of address to be written
        USCI_SPI_transmitData(spansionSpiConfig.baseAddress, locIndex);
        waitForSpiToComplete(spansionSpiConfig.baseAddress);

        USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
        waitForSpiToComplete(spansionSpiConfig.baseAddress);

        USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
        waitForSpiToComplete(spansionSpiConfig.baseAddress);

        // Pull CS High
        GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
                spansionSpiConfig.spiCs[selectedMemory].selectedPin);

        while( (readSR1 = spansionFlashReadSR1(selectedMemory)) & WIP )
        {
            if (readSR1 & E_ERR)
            {
                returnStatus = SPANSION_FLASH_STATUS_ERROR;
                break;
            }
        }
    }
    return (returnStatus);
}

uint8_t spansionFlashSectorErase4K (spansionMemNum_t selectedMemory, uint32_t baseAddress)
{
    uint8_t readSR1;
    uint8_t returnStatus = SPANSION_FLASH_STATUS_OK;
    uint8_t firstByte = (uint8_t)(baseAddress >> 16);

    spansionFlashWriteEnable(selectedMemory);

    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, P4E);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Send 3 bytes of address to be written
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, firstByte);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    while( (readSR1 = spansionFlashReadSR1(selectedMemory)) & WIP )
    {
        if (readSR1 & E_ERR)
        {
            returnStatus = SPANSION_FLASH_STATUS_ERROR;
            break;
        }
    }
    return (returnStatus);
}

uint8_t spansionFlashPageProgram (spansionMemNum_t selectedMemory, uint32_t baseAddress,
        uint8_t *pointerToBuffer, int16_t bufferSize)
{
    int16_t locIndex;
    uint8_t readSR1;
    uint8_t returnStatus = SPANSION_FLASH_STATUS_OK;
    uint8_t firstByte = (uint8_t)(baseAddress >> 16);
    uint8_t secondByte = (uint8_t)(baseAddress >> 8);
    uint8_t thirdByte = (uint8_t)baseAddress;

    spansionFlashWriteEnable(selectedMemory);

    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, PP);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Send 3 bytes of address to be written
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, firstByte);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, secondByte);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, thirdByte);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    for (locIndex = 0; locIndex < bufferSize; locIndex++)
    {
        USCI_SPI_transmitData(spansionSpiConfig.baseAddress, pointerToBuffer[locIndex]);
        waitForSpiToComplete(spansionSpiConfig.baseAddress);
    }

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    while( (readSR1 = spansionFlashReadSR1(selectedMemory)) & WIP )
    {
        if (readSR1 & P_ERR)
        {
            returnStatus = SPANSION_FLASH_STATUS_ERROR;
            break;
        }
    }
    return (returnStatus);
}

void spansionFlashRead (spansionMemNum_t selectedMemory, uint32_t baseAddress,
        uint8_t *pointerToBuffer, int16_t bufferSize)
{
    int16_t locIndex;
    uint8_t firstByte = (uint8_t)(baseAddress >> 16);
    uint8_t secondByte = (uint8_t)(baseAddress >> 8);
    uint8_t thirdByte = (uint8_t)baseAddress;

    spansionFlashWriteEnable(selectedMemory);

    // Pull CS low
    GPIO_setOutputLowOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);

    // Specify command
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, READ);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    // Send 3 bytes of address to be written
    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, firstByte);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, secondByte);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    USCI_SPI_transmitData(spansionSpiConfig.baseAddress, thirdByte);
    waitForSpiToComplete(spansionSpiConfig.baseAddress);

    for (locIndex = 0; locIndex < bufferSize; locIndex++)
    {
        USCI_SPI_transmitData(spansionSpiConfig.baseAddress, 0x00);
        waitForSpiToComplete(spansionSpiConfig.baseAddress);
        pointerToBuffer[locIndex] = USCI_SPI_receiveData(spansionSpiConfig.baseAddress);
    }

    // Pull CS High
    GPIO_setOutputHighOnPin(spansionSpiConfig.spiCs[selectedMemory].selectedPort,
            spansionSpiConfig.spiCs[selectedMemory].selectedPin);
}

//*****************************************************************************
//
//!  waitForSpiToComplete
//!
//!  \brief Waits for the SPI to complete TX/RX
//!
//!  \param baseAddress
//!        - \b Memory address
//!  \return none
//
//*****************************************************************************
void waitForSpiToComplete (uint16_t baseAddress)
{
    uint16_t timeOut = 0x01;

    while( (USCI_SPI_isBusy(baseAddress) == UCBUSY) &&
            ( timeOut++ ) );
}
