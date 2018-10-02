
//*****************************************************************************
//
// \file commDefines.h
//
// \brief This file defines the communication constants
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
#ifndef commDefines_H
#define commDefines_H

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
//! \def SPICLK defines the Frequency of SPI Clock
//
//*****************************************************************************
#define SPICLK              (2000000)

//*****************************************************************************
//
//! \def BAUD_RATE defines the Baud rate for UART
//
//*****************************************************************************
#define BAUD_RATE           (115200)

//*****************************************************************************
//
//! \def I2CCLK defines the Frequency of I2C Clock
//
//*****************************************************************************
#define I2CCLK			    (400000)

//*****************************************************************************
//
//! \def AFE4404_I2C_ADDRESS defines the AFE4404 I2C address
//
//*****************************************************************************
#define AFE4404_I2C_ADDRESS (0x58)

//*****************************************************************************
//
//! \def HRM Enables HRM ble profile
//
//! \def PULSE_OX Enables TI ble
//
//*****************************************************************************
#define HRM
//#define PULSE_OX

#ifdef PULSE_OX
#define deviceCode 0x32
#endif

#ifdef HRM
#define deviceCode 0x33
#endif

//*****************************************************************************
//
//! \def DATA_LOGGER_PACKET_SIZE defines the data logger packet size
//!     Number of bytes per HRM packet: 6 AFE, 6 MPU9150 , 1 HR
//
//*****************************************************************************
#define DATA_LOGGER_PACKET_SIZE             14

//*****************************************************************************
//
//! AFE44xx Register Address Definitions
//
//*****************************************************************************
#define CONTROL0 0x00
#define REDSTARTCOUNT 0x01
#define REDENDCOUNT 0x02
#define REDLEDSTARTCOUNT 0x03
#define REDLEDENDCOUNT 0x04
#define AMBREDSTARTCOUNT 0x05
#define AMBREDENDCOUNT 0x06
#define IRSTARTCOUNT 0x07
#define IRENDCOUNT 0x08
#define IRLEDSTARTCOUNT 0x09
#define IRLEDENDCOUNT 0x0A
#define AMBIRSTARTCOUNT 0x0B
#define AMBIRENDCOUNT 0x0C
#define REDCONVSTART 0x0D
#define REDCONVEND 0x0E
#define AMBREDCONVSTART 0x0F
#define AMBREDCONVEND 0x10
#define IRCONVSTART 0x11
#define IRCONVEND 0x12
#define AMBIRCONVSTART 0x13
#define AMBIRCONVEND 0x14
#define ADCRESETSTCOUNT0 0x15
#define ADCRESETENDCOUNT0 0x16
#define ADCRESETSTCOUNT1 0x17
#define ADCRESETENDCOUNT1 0x18
#define ADCRESETSTCOUNT2 0x19
#define ADCRESETENDCOUNT2 0x1A
#define ADCRESETSTCOUNT3 0x1B
#define ADCRESETENDCOUNT3 0x1C
#define PRPCOUNT 0x1D
#define CONTROL1 0x1E
#define TIAGAIN 0x20
#define TIA_AMB_GAIN 0x21
#define LEDCNTRL 0x22
#define CONTROL2 0x23
#define MODE 0x25
#define ALARM 0x29
#define REDVALUE 0x2A
#define AMBREDVALUE 0x2B
#define IRVALUE 0x2C
#define AMBIRVALUE 0x2D
#define REDMINUSAMBREDVALUE 0x2E
#define IRMINUSAMBIRVALUE 0x2F
#define DIAGNOSTICS 0x30
#define CONTROL3 0x31

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

#endif
