
//*****************************************************************************
//
// \file setMCLK.h
//
// \brief Header file for setMCLK.c
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
#ifndef SETMCLK_H
#define SETMCLK_H

#include "driverlib.h" 

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
#define _16MHZ_  16000
#define _25MHZ_  25000

//*****************************************************************************
//
//! \def USE_AUX_CLK Configures timers using in the system to use the Aux
//!     clock. Might find it useful when using LPM3
//
//*****************************************************************************
// #define USE_AUX_CLK

//*****************************************************************************
//
//! \def UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ defines Target frequency for MCLK in
//!     kHz
//!        Valid values are:
//!        - \b _16MHZ_
//!        - \b _25MHZ_
//
//*****************************************************************************
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ  (_25MHZ_)

#if (UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ == _25MHZ_)
//*****************************************************************************
//
//! \def UCS_MCLK_FLLREF_RATIO defines FLL Ratio
//!     For 25Mhz/32.768kHz = 763
//!     For 16MHz/32.768kHz = 488
//
//*****************************************************************************
#define UCS_MCLK_FLLREF_RATIO   763

#elif (UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ == _16MHZ_)

#define UCS_MCLK_FLLREF_RATIO   488

#else

#error "Incorrect Frequency"

#endif

//*****************************************************************************
//
// Exported variables
//
//*****************************************************************************

//*****************************************************************************
//
// Exported prototypes
//
//*****************************************************************************

//*****************************************************************************
//
//!  SetSystemCLKs
//!
//!  \brief Configure MSP clocks (MCLK, SMCLK and ACLK)
//!
//!  \param ucs_MCLK_desired_freq_KHZ
//!        Valid values are:
//!        - \b UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ
//!  \param ucc_MCLK_fllref_ratio
//!        Valid values are:
//!        - \b UCS_MCLK_FLLREF_RATIO
//!  \return none
//
//*****************************************************************************
extern void SetSystemCLKs(uint16_t ucs_MCLK_desired_freq_KHZ, 
        uint16_t ucc_MCLK_fllref_ratio);

#endif
