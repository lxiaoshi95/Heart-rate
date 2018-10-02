
//*****************************************************************************
//
// \file setMCLK.c
//
// \brief Handles the clock configurations
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
#include "setMCLK.h"

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
//! \var g_i8status Variable that holds thes status of Oscillator fault flags
//
//*****************************************************************************
int16_t g_i8status;

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

void SetSystemCLKs(uint16_t ucs_MCLK_desired_freq_KHZ, uint16_t ucc_MCLK_fllref_ratio)
{
    //Activate external 4MHz XT2 oscillator pins
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
            GPIO_PIN2 + GPIO_PIN3
    );

    //Set VCore
    PMM_setVCore(
#if (UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ == _25MHZ_)
            PMM_CORE_LEVEL_3
#elif (UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ == _16MHZ_)
            PMM_CORE_LEVEL_2
#else
            PMM_CORE_LEVEL_3
#endif
    );

    //Set up crystal oscillator to run at 4 MHz
    UCS_setExternalClockSource(0,4000000);
    UCS_XT2Start(UCS_XT2DRIVE_4MHZ_8MHZ);

    //Set up ACLK to run off oscillator at 4 MHz
    UCS_clockSignalInit(UCS_ACLK,
            UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);

    //Set DCO FLL reference = REFO
    UCS_clockSignalInit(
            UCS_FLLREF,
            UCS_REFOCLK_SELECT,
            UCS_CLOCK_DIVIDER_1
    );

    //Set Ratio and Desired MCLK Frequency  and initialize DCO
    UCS_initFLLSettle(
            ucs_MCLK_desired_freq_KHZ,
            ucc_MCLK_fllref_ratio
    );

    // Enable global oscillator fault flag
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
}

//*****************************************************************************
//
//!  NMI_ISR
//!
//!  \brief Handles NMI interrupt
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
//#pragma vector=UNMI_VECTOR
//__interrupt void NMI_ISR(void)
//{
//    do {
//        // If it still can't clear the oscillator fault flags after the timeout,
//        // trap and wait here.
//        g_i8status = UCS_clearAllOscFlagsWithTimeout(1000);
//    } while(g_i8status != 0);
//}
