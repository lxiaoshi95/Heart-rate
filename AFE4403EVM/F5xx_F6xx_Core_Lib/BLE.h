
//*****************************************************************************
//
// \file BLE.h
//
// \brief Header file for BLE.c
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
#ifndef BLE_H
#define BLE_H

#include "driverlib.h" 
#include "commDefines.h"
#include "string.h"
#include "stdio.h"
#include "msp430_math.h"

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
//! \def USING_BAUDRATE_CALCULATION when enable, it uses the usci_uartbaudrate
//
//*****************************************************************************
#define USING_BAUDRATE_CALCULATION

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
//!  initUart
//!
//!  \brief Initializes the UART interface. Calls uart.c driverlib functions to 
//!         initialize clocks, ports and the UART interface
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void initUart(void);

//*****************************************************************************
//
//!  sendHeartRate
//!
//!  \brief Sends a message with the user's heart rate value
//!
//!  \param heartRate an uint8_t
//!
//!  \return none
//
//*****************************************************************************
extern void sendHeartRate(uint16_t heartRate);

//*****************************************************************************
//
//!  sendPulseOx
//!
//!  \brief Sends a message with the user's blood oxygen value
//!
//!  \param pulseOx an uint8_t
//!
//!  \return none
//
//*****************************************************************************
extern void sendPulseOx(uint16_t pulseOx);

//*****************************************************************************
//
//!  sendStreamOn
//!
//!  \brief Sends a confirmation for Stream On message
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void sendStreamOn(void);

//*****************************************************************************
//
//!  sendStreamOff
//!
//!  \brief Sends a confirmation for Stream Off message
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void sendStreamOff(void);

//*****************************************************************************
//
//!  sendDataOff
//!
//!  \brief Sends a confirmation for Data Off message
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void sendDataOff(void);

//*****************************************************************************
//
//!  sendEcho
//!
//!  \brief Sends an echo for handshake
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void sendEcho(void);

//*****************************************************************************
//
//!  sendAtCommand
//!
//!  \brief Sends an AT command over BLE
//!
//!  \param command an uint8_t*
//!
//!  \param ms a int32_t
//!
//!  \return none
//
//*****************************************************************************
extern void sendAtCommand(uint8_t* command, int32_t ms);

//*****************************************************************************
//
//!  sendBattery
//!
//!  \brief Returns battery capacity
//!
//!  \param capacity an uint8_t
//!
//!  \return none
//
//*****************************************************************************
extern void sendBattery(uint8_t capacity);

//*****************************************************************************
//
//!  checkDisconnect
//!
//!  \brief Checks if disconnect was received
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void checkDisconnect(void);

//*****************************************************************************
//
//!  sendDisconnect
//!
//!  \brief Sends AT commands to disconnect BLE
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void sendDisconnect(void);

#endif /* end of include guard: COMMHANDLER_6CO2ZKIY */

