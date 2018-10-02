
//*****************************************************************************
//
// \file prg_info.c
//
// \brief Handles the throughput measurement routines
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
#include <intrinsics.h>
#include "setMCLK.h"
#include "prg_info.h"

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
#define TIME_STAMP      (TA1R)
#define LOCAL_MIN_MAX   (0x100) // Define the number of base timer rollovers per "local minimum and maximum" period 

//*****************************************************************************
//
// Global Data
//
//*****************************************************************************
prgInfo_t g_prgInfo;

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

void initializePrgInfo(prgInfo_t *locPrgInfo)
{
    locPrgInfo->totalCumulative = 0;
    locPrgInfo->instantaneous   = 0;
    locPrgInfo->average         = 0;
    locPrgInfo->localMinimum    = 0xFF;
    locPrgInfo->localMaximum    = 0;
    locPrgInfo->globalMinimum   = 0xFF;
    locPrgInfo->globalMaximum   = 0;
    locPrgInfo->cumulativeTicks = 0;
    locPrgInfo->timerPeriods    = 0;
    locPrgInfo->startTicks      = 0;
    locPrgInfo->timeActive      = 0;
}

void startPrgInfoTimer(prgInfo_t *locPrgInfo)
{
    // Capture timer value
    locPrgInfo->timeActive = true;
    locPrgInfo->startTicks = TIME_STAMP;
}

void stopPrgInfoTimer(prgInfo_t *locPrgInfo)
{
    uint16_t delta_time;

    if (locPrgInfo->timeActive)
    {
        delta_time = TIME_STAMP - locPrgInfo->startTicks;

        // Cumulative time
        locPrgInfo->cumulativeTicks += delta_time;

        // Stop timer
        locPrgInfo->timeActive = false;
    }
}

void prgInfoTimerOverflowInterrupt(prgInfo_t *locPrgInfo)
{
    // Increment number of timer overflow periods
    locPrgInfo->timerPeriods++;

    // Only process if we did not roll over to 0
    if(locPrgInfo->timerPeriods)
    {
        // Instantaneous throughput (binary fraction, 8 bits)
        locPrgInfo->instantaneous = (locPrgInfo->cumulativeTicks + 0x0080) >> 8;

        // Add to total cumulative throughput
        locPrgInfo->totalCumulative += locPrgInfo->instantaneous;

        // Average throughput (binary fraction, 8 bits)
        locPrgInfo->average = locPrgInfo->totalCumulative / locPrgInfo->timerPeriods;

        // Reset
        locPrgInfo->cumulativeTicks = 0;

        // Calculate minima and maxima
        if(!(locPrgInfo->timerPeriods & (LOCAL_MIN_MAX - 1)))
        {
            locPrgInfo->localMinimum = locPrgInfo->instantaneous;
            locPrgInfo->localMaximum = locPrgInfo->instantaneous;
        }
        else
        {
            if(locPrgInfo->instantaneous < locPrgInfo->localMinimum)
            {
                locPrgInfo->localMinimum = locPrgInfo->instantaneous;
            }

            if(locPrgInfo->instantaneous > locPrgInfo->localMaximum)
            {
                locPrgInfo->localMaximum = locPrgInfo->instantaneous;
            }

            if(locPrgInfo->localMinimum < locPrgInfo->globalMinimum &&
                    locPrgInfo->localMinimum > 0)
            {
                locPrgInfo->globalMinimum = locPrgInfo->localMinimum;
            }

            if(locPrgInfo->localMaximum > locPrgInfo->globalMaximum)
            {
                locPrgInfo->globalMaximum = locPrgInfo->localMaximum;
            }
        }
    }
    else
    {
        initializePrgInfo(locPrgInfo);
    }
}

void setupPrgInfoTimer(void)
{
    uint32_t locUCSGetACLKClock = 0;

    //  ACLK/8, TACLK = ACLK, Up mode
    TA1CTL = ID_3 | TASSEL_1 | MC_1;             
    locUCSGetACLKClock = (UCS_getACLK() / 8);

    // (ACLK/8)/4
    TA1EX0 = TAIDEX_3;                           
    locUCSGetACLKClock /= 4;

    // 500ms
    TA1CCR0 = (locUCSGetACLKClock / 1000) * 500;
    TA1CCTL0 = CCIE;
}

//*****************************************************************************
//
//!
//!  \brief  Handles Timer1 A0 interrupt
//!          It is used to calculate the throughput of system
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
//#pragma vector=TIMER1_A0_VECTOR
//__interrupt void Timer1_A0 (void)
//{
//#ifdef MEASURED_THROUGHPUT
//    stopPrgInfoTimer(&g_prgInfo);
//    prgInfoTimerOverflowInterrupt(&g_prgInfo);
//    __bic_SR_register_on_exit(LPM0_bits);
//#endif
//}
