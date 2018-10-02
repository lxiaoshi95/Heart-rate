
//*****************************************************************************
//
// \file fuelGauge.h
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
#ifndef _FUELGAUGE_H_
#define _FUELGAUGE_H_

#include "driverlib.h" 
#include "usci_i2c.h"

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
//!  chargerOn  
//!
//!  \brief  Enables battery charging
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void chargerOn(void);

//*****************************************************************************
//
//!  getSOC
//!
//!  \brief Reads battery capacity of the device
//!
//!  \param none
//!
//!  \return Value with a range of 0 to 100%.
//
//***************************************************************************** 
extern int8_t getSOC(void);

//*****************************************************************************
//
//!  getAP
//!
//!  \brief Reads the battery Average Power
//!
//!  \param none
//!
//!  \return signed integer value of the average power during battery charging 
//!   and discharging. It is negative during discharge and positive during charge
//!   A value of 0 indicates that the battery is not being discharged. The value 
//!   is reported in units of mW.
//
//***************************************************************************** 
extern int8_t getAP(void);

//*****************************************************************************
//
//!  getAC
//!
//!  \brief Reads the battery Average Current
//!
//!  \param none
//!
//!  \return signed integer value of the average crrent
//
//*****************************************************************************
extern int8_t getAC(void);

//*****************************************************************************
//
//!  getVOLT
//!
//!  \brief Reads the battery voltage
//!
//!  \param none
//!
//!  \return unsigned integer value
//
//***************************************************************************** 
extern uint8_t getVOLT(void);

//*****************************************************************************
//
//!  readFuelGaugeCommand
//!
//!  \brief Reads value from a Command message
//!
//!  \param fuelGaugeMsg
//!
//!  \return none
//
//***************************************************************************** 
extern void readFuelGaugeCommand(uint8_t *fuelGaugeMsg);

//*****************************************************************************
//
//!  readFuelGaugeSubCommand
//!
//!  \brief Reads value from a SubCommand message
//!
//!  \param fuelGaugeMsg
//!
//!  \return none
//
//***************************************************************************** 
extern void readFuelGaugeSubCommand(uint8_t *fuelGaugeMsg);

//*****************************************************************************
//
//!  writeFuelGaugeSubCommand
//!
//!  \brief Send a SubCommand message
//!
//!  \param fuelGaugeMsg
//!
//!  \return none
//
//*****************************************************************************
extern void writeFuelGaugeSubCommand(uint8_t *fuelGaugeMsg);

//*****************************************************************************
//
//!  softResetFuelGauge
//!
//!  \brief Send the soft reset Message
//!
//!  \param none
//!
//!  \return none
//
//***************************************************************************** 
extern void softResetFuelGauge(void);

//*****************************************************************************
//
//!  batteryInsertFuelGauge
//!
//!  \brief Send the battery Insert Message
//!
//!  \param none
//!
//!  \return none
//
//***************************************************************************** 
extern void batteryInsertFuelGauge(void);

#endif /* _FUELGAUGE_H_ */
