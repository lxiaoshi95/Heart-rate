
//*****************************************************************************
//
// \file config_AFE4403.c
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
#include "commDefines.h"
#include "driverlib.h"
#include "AFE44xx.h" 
#include "HAL_I2C.h"
#include "config_AFE44xx.h"

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

/* AFE GPIO Configuration */
const struct AFE44xx_GPIO AFE44xx_RESET = {
    .selectedPort = GPIO_PORT_P2,
    .selectedPin = GPIO_PIN3
};

const struct AFE44xx_GPIO AFE44xx_PDNZ = {
    .selectedPort = GPIO_PORT_P2,
    .selectedPin = GPIO_PIN4
};

const struct AFE44xx_GPIO AFE44xx_DIAG_END = {
    .selectedPort = GPIO_PORT_P2,
    .selectedPin = GPIO_PIN2
};

/* LED GPIO Configuration */
const struct AFE44xx_GPIO AFE44xx_STREAM_STATUS_LED = {
    .selectedPort = GPIO_PORT_P1,
    .selectedPin = GPIO_PIN3
};

const struct AFE44xx_GPIO AFE44xx_PACKET_TX_STATUS_LED = {
    .selectedPort = GPIO_PORT_P1,
    .selectedPin = GPIO_PIN4
};

const struct AFE44xx_PORT_ISR AFE44xx_DRDY_PORT_ISR = {
    .selectedPort = GPIO_PORT_P2,
    .selectedPin = GPIO_PIN5
};

/* AFE SPI Configuration */
const struct AFE44xx_SPI AFE44xx_SPI_Config = {
    .baseAddress = __MSP430_BASEADDRESS_USCI_A1__,
    .selectClockSource = USCI_SPI_CLOCKSOURCE_ACLK,
    .desiredSpiClock = SPICLK,
    .msbFirst = USCI_SPI_MSB_FIRST,
    .clockPhase = USCI_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,
    .clockPolarity = USCI_SPI_CLOCKPOLARITY_INACTIVITY_LOW, 
    .SPI_Tx.selectedPort  = GPIO_PORT_P4,
    .SPI_Tx.selectedPin   = GPIO_PIN1,
    .SPI_Rx.selectedPort  = GPIO_PORT_P4,
    .SPI_Rx.selectedPin   = GPIO_PIN2,
    .SPI_Clk.selectedPort = GPIO_PORT_P4,
    .SPI_Clk.selectedPin  = GPIO_PIN3,
    .SPI_CS.selectedPort  = GPIO_PORT_P4,
    .SPI_CS.selectedPin   = GPIO_PIN0,
};

// For Spansion External Flash Memory on UCB1
const uint8_t P4mapping[8] =
{
		PM_NONE, //PM_UCA1STE, //PM_UCB1STE,
		PM_UCA1SIMO, //PM_UCB1SIMO,
		PM_UCA1SOMI, //PM_UCB1SOMI,
		PM_UCA1CLK, //PM_UCB1CLK,
		PM_UCB1SIMO, //PM_UCB1SCL, //PM_UCA1TXD,
		PM_UCB1SOMI, //PM_UCB1SDA, //PM_UCA1RXD,
		PM_UCB1CLK, //PM_NONE,
		PM_NONE
};

//*****************************************************************************
//
// Function Prototypes
//
//*****************************************************************************

//*****************************************************************************
//
//!  readAccelerometerData  
//!
//!  \brief  Callback functions to read the accelerometer
//!
//!  \param pointer to the acc buffer
//!
//!  \return none
//
//*****************************************************************************
void readAccelerometerData (uint8_t *data)
{
   HAL_I2C_read(USCI_B0_BASE, MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, data, 6);
}

//*****************************************************************************
//
//!  initSpi  
//!
//!  \brief  Calls functions in driverlib to initialize and enable SPI master 
//!          and enable interrupts
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void initSpi(void)
{
  uint32_t locUCSGetClock = 0;

	//Set Tx and CLK pins as outputs for SPI
	GPIO_setAsPeripheralModuleFunctionOutputPin(AFE44xx_SPI_Config.SPI_Tx.selectedPort,
			AFE44xx_SPI_Config.SPI_Tx.selectedPin
	);

	GPIO_setAsPeripheralModuleFunctionOutputPin(AFE44xx_SPI_Config.SPI_Clk.selectedPort,
			AFE44xx_SPI_Config.SPI_Clk.selectedPin
	);

	//Set Rx pin as input for SPI
	GPIO_setAsPeripheralModuleFunctionInputPin(AFE44xx_SPI_Config.SPI_Rx.selectedPort,
			AFE44xx_SPI_Config.SPI_Rx.selectedPin
	);

	//AFE_RESET
	GPIO_setAsOutputPin(AFE44xx_RESET.selectedPort,
			AFE44xx_RESET.selectedPin
	);

	//AFE_RESET is pulled high
	GPIO_setOutputHighOnPin(AFE44xx_RESET.selectedPort,
			AFE44xx_RESET.selectedPin
	);

	//Set CS pin as output for SPI
	GPIO_setAsOutputPin(AFE44xx_SPI_Config.SPI_CS.selectedPort,
			AFE44xx_SPI_Config.SPI_CS.selectedPin
	);

	//Pull CS low to enable communication
	GPIO_setOutputLowOnPin(AFE44xx_SPI_Config.SPI_CS.selectedPort,
			AFE44xx_SPI_Config.SPI_CS.selectedPin
	);

	//Set AFE4300 DRDY and is set as Input
	GPIO_setAsInputPinWithPullUpResistor(AFE44xx_DRDY_PORT_ISR.selectedPort,
			AFE44xx_DRDY_PORT_ISR.selectedPin
	);

    switch(AFE44xx_SPI_Config.selectClockSource)
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
 
	//Configure SPI
	USCI_SPI_masterInit(AFE44xx_SPI_Config.baseAddress,
			AFE44xx_SPI_Config.selectClockSource,
			locUCSGetClock,
			AFE44xx_SPI_Config.desiredSpiClock,
			AFE44xx_SPI_Config.msbFirst,
			AFE44xx_SPI_Config.clockPhase,
			AFE44xx_SPI_Config.clockPolarity);

	USCI_SPI_enable(AFE44xx_SPI_Config.baseAddress);
}

//*****************************************************************************
//
//!  initAFE44xxCommInterface  
//!
//!  \brief  Configures the AFE44xx communication interface
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void initAFE44xxCommInterface (void)
{
  initSpi();
}

