
//*****************************************************************************
//
// \file spansion_flash.h
//
// \brief Header file for spansion_flash.c
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
#ifndef SPANSION_FLASH_H
#define SPANSION_FLASH_H

#include <stdint.h>
#include "driverlib.h" 
#include "commDefines.h"

//*****************************************************************************
//
// Types
//
//*****************************************************************************
//*****************************************************************************
//
//! struct GPIO_PORT_PIN
//!
//! GPIO port configuration
//
//*****************************************************************************
struct GPIO_PORT_PIN{
    uint8_t selectedPort;
    uint16_t selectedPin;
};

//*****************************************************************************
//
//! \enum Spansion enumerates the available Spansion Serial Flash
//
//*****************************************************************************
typedef enum
{
    SPANSION_0 = 0,
    SPANSION_1 = 1,
    TOTAL_SPANSION_MEMORIES
}spansionMemNum_t;

//*****************************************************************************
//
//! struct SPANSION_MEMORY_SPI_CONFIG
//!
//! SPI Spansion Configuration
//
//*****************************************************************************
struct SPANSION_MEMORY_SPI_CONFIG{
    uint16_t baseAddress;
    uint8_t  selectClockSource;
    uint32_t desiredSpiClock;
    uint8_t  msbFirst;
    uint8_t  clockPhase;
    uint8_t  clockPolarity;
    struct   GPIO_PORT_PIN spiMemWPZ;
    struct   GPIO_PORT_PIN spiMemHOLDZ;
    struct   GPIO_PORT_PIN spiTx;
    struct   GPIO_PORT_PIN spiRx;
    struct   GPIO_PORT_PIN spiClk;
    struct   GPIO_PORT_PIN spiCs[TOTAL_SPANSION_MEMORIES];
};

//******************************************************************************
//
// Defines
//
//******************************************************************************

//*****************************************************************************
//
//! \def SPANSION_FLASH_STATUS_OK
//
//! \def SPANSION_FLASH_STATUS_ERROR
//
//*****************************************************************************
#define SPANSION_FLASH_STATUS_OK     0
#define SPANSION_FLASH_STATUS_ERROR  1

//*****************************************************************************
//
//! \def Spansion FLASH OPCODE COMMANDS
//
//*****************************************************************************
#define WREN    0x06
#define WRDI    0x04
#define RDSR1   0x05
#define RDSR2   0x07
#define WRSR    0x01
#define READ    0x03
#define PP      0x02
#define BE      0xC7
#define SE      0xD8
#define P4E     0x20
#define READ_ID 0x90
#define CLSR    0x30

//*****************************************************************************
//
//! \def Spansion SR1 BIT DEFINITION
//
//*****************************************************************************
#define WIP     BIT0
#define WEL     BIT1
#define E_ERR   BIT5
#define P_ERR   BIT6

//*****************************************************************************
//
//! \def SPANSION_MEMORY_LOCATIONS Locations available on SPANSION memory (16MB)
//
//! \def TOTAL_MEMORY_BANK Total memory locations
//
//! \def TOTAL_SIZE_KB Total memory available expressed in KBytes
//
//! \def SPANSION_BASE_ADDRESS Spansion Start Address
//
//*****************************************************************************
#define SPANSION_MEMORY_LOCATIONS  (uint32_t)(16777216)
#define TOTAL_MEMORY_BANK          (uint32_t)((SPANSION_MEMORY_LOCATIONS)* \
        (TOTAL_SPANSION_MEMORIES))
#define TOTAL_SIZE_KB	           (uint16_t)(TOTAL_MEMORY_BANK)/1024)
#define SPANSION_BASE_ADDRESS 	   (0x00000)

//*****************************************************************************
//
//! \var spansionSpiConfig stores the SPI and port configurations for the
//! Spansion memories
//
//*****************************************************************************
extern const struct SPANSION_MEMORY_SPI_CONFIG spansionSpiConfig;

//*****************************************************************************
//
//! \var g_spansionMemNumber Variable that holds the selected Spansion Memory
//
//*****************************************************************************
extern spansionMemNum_t g_SpansionMemSelected;

//*****************************************************************************
//
//! \var g_bErrorFlashErase Variable that holds the status if there was an
//!     error erasing the external flash
//
//*****************************************************************************
extern bool g_bErrorFlashErase;

//*****************************************************************************
//
//! \var writeBaseAddr Variable that holds the memory address when writing
//
//*****************************************************************************
extern uint32_t g_ui32WriteBaseAddr;

//*****************************************************************************
//
//! \var g_spansionMemNumber Variable that holds the virtual memory address
//!     when writing or reading
//
//*****************************************************************************
extern uint32_t g_ui32VirtualBaseAddr;

//*****************************************************************************
//
//! \var g_spansionMemNumber Variable that holds the memory address when reading
//
//*****************************************************************************
extern uint32_t g_ui32ReadBaseAddr;

//*****************************************************************************
//
//!  spansionFlashInit
//!
//!  \brief Initialize Spansion Flash Memory
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
extern void spansionFlashInit (void);

//*****************************************************************************
//
//!  spansionFlashReadID
//!
//!  \brief Reads Spansion Flash ID
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!
//!  \return none
//
//*****************************************************************************
extern void spansionFlashReadID (spansionMemNum_t selectedMemory);

//*****************************************************************************
//
//!  spansionFlashReadSR1
//!
//!  \brief Reads Spansion Flash SR1
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!
//!  \return uint8_t - Status Register 1 value
//
//*****************************************************************************
extern uint8_t spansionFlashReadSR1 (spansionMemNum_t selectedMemory);

//*****************************************************************************
//
//!  spansionFlashReadSR2
//!
//!  \brief Reads Spansion Flash SR2
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!
//!  \return uint8_t - Status Register 2 value
//
//*****************************************************************************
extern uint8_t spansionFlashReadSR2 (spansionMemNum_t selectedMemory);

//*****************************************************************************
//
//!  spansionFlashReadReg
//!
//!  \brief Write Enable to the Spansion Flash
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!  \param readReg
//!        - \b Register number
//!  \param readPos
//!        - \b Register index
//!
//!  \return uint8_t - Status Register value
//
//*****************************************************************************
extern uint8_t spansionFlashReadReg (spansionMemNum_t selectedMemory, uint8_t readReg,
        uint8_t readPos);

//*****************************************************************************
//
//!  spansionFlashWriteEnable
//!
//!  \brief Write Enable to the Spansion Flash
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!
//!  \return none
//
//*****************************************************************************
extern void spansionFlashWriteEnable (spansionMemNum_t selectedMemory);

//*****************************************************************************
//
//!  spansionFlashWriteDisable
//!
//!  \brief Write Disable to the Spansion Flash
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!
//!  \return none
//
//*****************************************************************************
extern void spansionFlashWriteDisable (spansionMemNum_t selectedMemory);

//*****************************************************************************
//
//!  spansionFlashClearSR
//!
//!  \brief Clears Spansion Flash SR
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!
//!  \return none
//
//*****************************************************************************
extern void spansionFlashClearSR (spansionMemNum_t selectedMemory);

//*****************************************************************************
//
//!  spansionFlashBulkErase
//!
//!  \brief Bluk Erase the Spansion Flash
//!
//!  \param none
//!
//!  \return uint8_t
//!        Valid values are:
//!        - \b SPANSION_FLASH_STATUS_OK
//!        - \b SPANSION_FLASH_STATUS_ERROR
//
//*****************************************************************************
extern uint8_t spansionFlashBulkErase (void);

//*****************************************************************************
//
//!  spansionFlashSectorErase
//!
//!  \brief Erase all sectors of the Spansion Flash
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!
//!  \return uint8_t
//!        Valid values are:
//!        - \b SPANSION_FLASH_STATUS_OK
//!        - \b SPANSION_FLASH_STATUS_ERROR
//
//*****************************************************************************
extern uint8_t spansionFlashSectorErase (spansionMemNum_t selectedMemory);

//*****************************************************************************
//
//!  spansionFlashSectorErase4K
//!
//!  \brief Erase a single 4KB sector of the Spansion Flash
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!  \param baseAddress
//!        - \b Memory address
//!
//!  \return uint8_t
//!        Valid values are:
//!        - \b SPANSION_FLASH_STATUS_OK
//!        - \b SPANSION_FLASH_STATUS_ERROR
//
//*****************************************************************************
extern uint8_t spansionFlashSectorErase4K (spansionMemNum_t selectedMemory, uint32_t baseAddress);

//*****************************************************************************
//
//!  spansionFlashPageProgram
//!
//!  \brief Program a Spansion Flash Page
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!  \param baseAddress
//!        - \b Memory address
//!  \param pointerToBuffer
//!        - \b Buffer with the data to program
//!  \param bufferSize
//!        - \b Buffer Size
//!  \return uint8_t
//!        Valid values are:
//!        - \b SPANSION_FLASH_STATUS_OK
//!        - \b SPANSION_FLASH_STATUS_ERROR
//
//*****************************************************************************
extern uint8_t spansionFlashPageProgram (spansionMemNum_t selectedMemory, uint32_t baseAddress,
        uint8_t *pointerToBuffer, int16_t bufferSize);

//*****************************************************************************
//
//!  spansionFlashRead
//!
//!  \brief Reads the Spansion Flash
//!
//!  \param selectedMemory
//!        - \b The selected Memory
//!  \param baseAddress
//!        - \b Memory address
//!  \param pointerToBuffer
//!        - \b Buffer where the data will be stored
//!  \param bufferSize
//!        - \b Buffer Size
//!  \return none
//
//*****************************************************************************
extern void spansionFlashRead (spansionMemNum_t selectedMemory, uint32_t baseAddress,
        uint8_t *pointerToBuffer, int16_t bufferSize);

#endif /* SPANSION_FLASH_H */
