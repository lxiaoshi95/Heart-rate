
//*****************************************************************************
//
// \file prg_info.h
//
// \brief Header file for prg_info.c
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
#ifndef PRG_INFO_H
#define PRG_INFO_H

//*****************************************************************************
//
// Types
//
//*****************************************************************************
//*****************************************************************************
//
//! \typedef prgInfo_t
//!
//! Program Information when calculating the throughput
//
//*****************************************************************************
typedef struct
{
    uint32_t totalCumulative;  /* Total cumulative throughput */
    uint16_t instantaneous;    /* Instantaneous throughput in the last timer period */
    uint16_t average;          /* Average throughput */
    uint16_t localMinimum;     /* local Minimum throughput */
    uint16_t localMaximum;     /* local Maximum throughput */
    uint16_t globalMinimum;    /* global Mimimum throughput*/
    uint16_t globalMaximum;    /* global Maximum throughput */
    uint16_t cumulativeTicks;  /* Throughput Ticks in the last timer period */
    uint16_t timerPeriods;     /* Number of timer periods */
    uint16_t startTicks;       /* Start Ticks for measured event */
    uint8_t  timeActive;       /* flag to stop the timer after idle */
} prgInfo_t;

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
//*****************************************************************************
//
//! \def MEASURED_THROUGHPUT Enables/disables the throughput measurement on the
//!     system
//
//*****************************************************************************
// #define MEASURED_THROUGHPUT

//*****************************************************************************
//
// Exported variables
//
//*****************************************************************************
//*****************************************************************************
//
//! \var g_prgInfo Variable that holds program information 
//
//*****************************************************************************
extern prgInfo_t g_prgInfo;

//*****************************************************************************
//
// Exported prototypes
//
//*****************************************************************************

//*****************************************************************************
//
//!  initializePrgInfo
//!
//!  \brief  Initialize program information
//!
//!  \param prgInfo_t
//!
//!  \return none
//
//*****************************************************************************
extern void initializePrgInfo(prgInfo_t *locPrgInfo);

//*****************************************************************************
//
//!  startPrgInfoTimer
//!
//!  \brief  Start timer to calculte program information
//!
//!  \param prgInfo_t
//!
//!  \return none
//
//*****************************************************************************
extern void startPrgInfoTimer(prgInfo_t *locPrgInfo);

//*****************************************************************************
//
//!  stopPrgInfoTimer
//!
//!  \brief  Stop program information timer
//!
//!  \param prgInfo_t
//!
//!  \return none
//
//*****************************************************************************
extern void stopPrgInfoTimer(prgInfo_t *profile);

//*****************************************************************************
//
//!  prgInfoTimerOverflowInterrupt
//!
//!  \brief  Keep the system measurements 
//!          Average = (((uint16_t)(0xFF - prg_info->average) * 100 ) / 0xFF)
//!
//!  \param prg info type
//!
//!  \return none
//
//*****************************************************************************
extern void prgInfoTimerOverflowInterrupt(prgInfo_t *profile);

//*****************************************************************************
//
//!  setupPrgInfoTimer
//!
//!  \brief  Setup timer to be used to calculate the prg information
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void setupPrgInfoTimer(void);
#endif
