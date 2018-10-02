
//*****************************************************************************
//
// \file spansion_config_flash.c
//
// \brief Configuration of the pansion Flash Memory
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

//*****************************************************************************
//
// Constants
//
//*****************************************************************************
//*****************************************************************************
//
//! \var spansionSpiConfig stores the SPI and port configurations for the
//! Spansion memories
//
//*****************************************************************************
const struct SPANSION_MEMORY_SPI_CONFIG spansionSpiConfig = {
        .baseAddress = __MSP430_BASEADDRESS_USCI_B1__,
        .selectClockSource = USCI_SPI_CLOCKSOURCE_ACLK,
        .desiredSpiClock = SPICLK,
        .msbFirst = USCI_SPI_MSB_FIRST,
        .clockPhase = USCI_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,
        .clockPolarity = USCI_SPI_CLOCKPOLARITY_INACTIVITY_LOW,
        .spiMemWPZ.selectedPort  = GPIO_PORT_P2,
        .spiMemWPZ.selectedPin   = GPIO_PIN6,
        .spiMemHOLDZ.selectedPort  = GPIO_PORT_P2,
        .spiMemHOLDZ.selectedPin   = GPIO_PIN7,
        .spiTx.selectedPort  = GPIO_PORT_P4,
        .spiTx.selectedPin   = GPIO_PIN4,
        .spiRx.selectedPort  = GPIO_PORT_P4,
        .spiRx.selectedPin   = GPIO_PIN5,
        .spiClk.selectedPort = GPIO_PORT_P4,
        .spiClk.selectedPin  = GPIO_PIN6,
        .spiCs[SPANSION_0].selectedPort = GPIO_PORT_P6,
        .spiCs[SPANSION_0].selectedPin  = GPIO_PIN4,
        .spiCs[SPANSION_1].selectedPort = GPIO_PORT_P6,
        .spiCs[SPANSION_1].selectedPin  = GPIO_PIN5,
};

//*****************************************************************************
//
// Function Prototypes
//
//*****************************************************************************
