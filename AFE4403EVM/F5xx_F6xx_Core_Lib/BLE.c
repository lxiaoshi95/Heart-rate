
//*****************************************************************************
//
// \file BLE.c
//						  
// \brief Handles communication with the BlueRadios BLE module 
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
#include "BLE.h"
#include "prg_info.h"
#include "AFE44xx.h"

#ifdef USING_BAUDRATE_CALCULATION
#include "usci_uartbaudrate.h"
#endif

//*****************************************************************************
//
// Types
//
//*****************************************************************************

//*****************************************************************************
//
//! \enum STREAM_STATUS enumerates values for STREAM state machine
//
//*****************************************************************************
enum STREAM_STATUS {START_STREAM, STOP_STREAM};

//*****************************************************************************
//
//! \var currentStreamStatus holds STREAM status
//
//*****************************************************************************
enum STREAM_STATUS currentStreamStatus = STOP_STREAM;

//*****************************************************************************
//
//! \enum SPI_STATUS enumerates values for SPI state machine
//
//*****************************************************************************
enum SPI_STATUS {IDLE, MEASURE, MEASURE_STREAM};

//*****************************************************************************
//
//! \var currentSpiStatus holds SPI status
//
//*****************************************************************************
enum SPI_STATUS currentSpiStatus = IDLE;

//*****************************************************************************
//
//! \enum UART_STATUS enumerates values for UART state machine
//
//*****************************************************************************
enum UART_STATUS {BLE_CONNECT,START_BYTE,MSG_LENGTH, DEVICE_CODE,OP_CODE,END_BYTE};

//*****************************************************************************
//
//! \var currentUartStatus holds UART status
//
//*****************************************************************************
enum UART_STATUS currentUartStatus = START_BYTE;

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
//! \var g_ui8UartReceive holds a received byte from the UART 
//! UART communication data holders
//
//*****************************************************************************
uint8_t g_ui8UartReceive = 0;

//*****************************************************************************
//
//! \var g_ui8UartMsgLength holds the length of the UART message
//! UART communication data holders
//
//*****************************************************************************
uint8_t g_ui8UartMsgLength = 0;

//*****************************************************************************
//
//! \var g_ui8UartOpCode holds UART OP code
//! UART communication data holders
//
//*****************************************************************************
uint8_t g_ui8UartOpCode = 0;

//Buffer to check if "DISCONNECT" was received
//*****************************************************************************
//
//! \var g_ui8bslCounter Buffer to check if "DISCONNECT" was received
//! UART communication data holders
//
//*****************************************************************************
uint8_t  g_ui8UartBuffer[10];

//*****************************************************************************
//
//! \var g_ui16UartCounter holds the UART message counter
//! UART communication data holders
//
//*****************************************************************************
uint16_t g_ui16UartCounter = 0;

//*****************************************************************************
//
//! \var g_ui8BufferEnable enables the search for the 'DISCONNECT' 
//! UART communication data holders
//
//*****************************************************************************
uint8_t  g_ui8BufferEnable = 0;

//*****************************************************************************
//
// Imported Data
//
//*****************************************************************************
// Battery capacity finction from fuelGauge.c
extern int8_t getSOC(void);

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
void setDataOff(void);

void initUart(void)
{
    USCI_UART_initParam locUARTInitParam = {0};

#ifdef USING_BAUDRATE_CALCULATION 
    uint8_t UCAxBR0_value = 0x00;
    uint8_t UCAxBR1_value = 0x00;
    uint16_t UCAxMCTL_value = 0x00;
#endif     

    //P3.3 = USCI A0 TX, P3.4 = USCI A0 RX
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
            GPIO_PIN3 + GPIO_PIN4
    );

    locUARTInitParam.selectClockSource = USCI_UART_CLOCKSOURCE_ACLK;

#ifndef USING_BAUDRATE_CALCULATION  
    // Using:
    // http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
    // ACLK = 4MHZ BAUDRATE 115200
    locUARTInitParam.clockPrescalar = 2;
    locUARTInitParam.firstModReg = 2;
    locUARTInitParam.secondModReg = 3;
    locUARTInitParam.overSampling = 1;
#endif  

    locUARTInitParam.parity = USCI_UART_NO_PARITY;
    locUARTInitParam.msborLsbFirst = USCI_UART_LSB_FIRST;
    locUARTInitParam.numberofStopBits = USCI_UART_ONE_STOP_BIT;
    locUARTInitParam.uartMode = USCI_UART_MODE;

    //115200 Baud rate, no parity, one stop bit
    USCI_UART_init(__MSP430_BASEADDRESS_USCI_A0__,
            &locUARTInitParam);

#ifdef USING_BAUDRATE_CALCULATION             
    //Calculate Baud rate divider values for Modulation control registers
    USCI_UARTBAUDRATE_calculateBaudDividers(UCS_getACLK(),
            BAUD_RATE,
            &UCAxBR0_value,
            &UCAxBR1_value,
            &UCAxMCTL_value,
            USCI_UART_LOW_FREQUENCY_BAUDRATE_GENERATION);

    //Modulation Control Registers
    HWREG8(__MSP430_BASEADDRESS_USCI_A0__ + OFS_UCAxBR0 ) = UCAxBR0_value;
    HWREG8(__MSP430_BASEADDRESS_USCI_A0__ + OFS_UCAxBR1)  = UCAxBR1_value;
    HWREG8(__MSP430_BASEADDRESS_USCI_A0__ + OFS_UCAxMCTL) = UCAxMCTL_value;
#endif             

    USCI_UART_enable(__MSP430_BASEADDRESS_USCI_A0__);

    USCI_UART_enableInterrupt(__MSP430_BASEADDRESS_USCI_A0__,
            USCI_UART_RECEIVE_INTERRUPT);

}

void sendPulseOx(uint16_t pulseOx)
{
    uint8_t pulseOxHigh = pulseOx >> 8;
    uint8_t pulseOxLow = pulseOx;
    int16_t locIndex;
    uint8_t buffer[7];

    buffer[0] = 0x0C; //Start byte
    buffer[1] = 0x35; //Message length
    buffer[2] = deviceCode; //Device code
    buffer[3] = 0x43; //PulseOx opcode
    buffer[4] = pulseOxHigh; //PulseOx high byte
    buffer[5] = pulseOxLow; //PulseOx low byte
    buffer[6] = 0x0D; //End byte

    for(locIndex = 0; locIndex < 7; locIndex++)
    {
        USCI_UART_transmitData (__MSP430_BASEADDRESS_USCI_A0__, buffer[locIndex]);
        __delay_cycles(2500);
    }
}

void sendHeartRate(uint16_t heartRate)
{
    uint8_t heartRateHigh = heartRate >> 8;
    uint8_t heartRateLow = heartRate;
    int16_t locIndex;
    uint8_t buffer[7];

    buffer[0] = 0x0C; //Start byte
    buffer[1] = 0x35; //Message length
    buffer[2] = deviceCode; //Device code
    buffer[3] = 0x44; //Heart rate opcode
    buffer[4] = heartRateHigh; //Heart rate high byte
    buffer[5] = heartRateLow; //Heart rate low byte
    buffer[6] = 0x0D; //End byte

    for(locIndex = 0; locIndex < 7; locIndex++)
    {
        USCI_UART_transmitData (__MSP430_BASEADDRESS_USCI_A0__, buffer[locIndex]);
        __delay_cycles(2500);
    }
}

void sendDisconnect(void)
{
    __delay_cycles(2500000);
    sendAtCommand("+++\r\0",100);
    //__delay_cycles(1600000);
    sendAtCommand("atdh,0\r\0",100);
}

void sendStreamOn(void)
{
    uint8_t buffer[5];
    int16_t locIndex;
    buffer[0] = 0x0C; //Start byte
    buffer[1] = 0x33; //Message length
    buffer[2] = deviceCode; //Device code
    buffer[3] = 0x41; //Stream on opcode
    buffer[4] = 0x0D; //End byte

    for(locIndex = 0; locIndex < 5; locIndex++)
    {
        USCI_UART_transmitData (__MSP430_BASEADDRESS_USCI_A0__, buffer[locIndex]);
        __delay_cycles(2500);
    }
}

void sendStreamOff(void)
{
    uint8_t buffer[5];
    int16_t locIndex;

    buffer[0] = 0x0C; //Start byte
    buffer[1] = 0x33; //Message length
    buffer[2] = deviceCode; //Device code
    buffer[3] = 0x42; //Stream Off opcode
    buffer[4] = 0x0D; //End byte

    for(locIndex = 0; locIndex < 5; locIndex++)
    {
        USCI_UART_transmitData (__MSP430_BASEADDRESS_USCI_A0__, buffer[locIndex]);
        __delay_cycles(2500);
    }
}

void sendDataOff(void)
{
    uint8_t buffer[5];
    int16_t locIndex;

    buffer[0] = 0x0C; //Start byte
    buffer[1] = 0x33; //Message length
    buffer[2] = deviceCode; //Device code
    buffer[3] = 0x46; //Data off opcode
    buffer[4] = 0x0D; //End byte

    for(locIndex = 0; locIndex < 5; locIndex++)
    {
        USCI_UART_transmitData (__MSP430_BASEADDRESS_USCI_A0__, buffer[locIndex]);
        __delay_cycles(2500);
    }

    sendDisconnect();
}

void sendEcho(void)
{
    uint8_t buffer[5];
    int16_t locIndex;

    buffer[0] = 0x0C; //Start byte
    buffer[1] = 0x33; //Message length
    buffer[2] = deviceCode; //Device code
    buffer[3] = 0x47; //Echo opcode
    buffer[4] = 0x0D; //End byte

    for(locIndex = 0; locIndex < 5; locIndex++)
    {
        USCI_UART_transmitData (__MSP430_BASEADDRESS_USCI_A0__, buffer[locIndex]);
        __delay_cycles(2500);
    }
}

void sendBattery(uint8_t capacity)
{
    uint8_t buffer[6];
    int16_t locIndex;

    //Scale down battery capacity to 1 of 4 values
    if(capacity >= 75) 
    {
        capacity = 3;
    }
    else if(capacity >= 50)
    { 
        capacity = 2;
    }
    else if(capacity >= 25)
    {
        capacity = 1;
    }
    else 
    {
        capacity = 0;
    }

    buffer[0] = 0x0C; //Start byte
    buffer[1] = 0x34; //Message length
    buffer[2] = deviceCode; //Device code
    buffer[3] = 0x4B; //Battery capacity opcode
    buffer[4] = capacity; //Battery capacity value
    buffer[5] = 0x0D; //End byte

    for(locIndex = 0; locIndex < 6; locIndex++)
    {
        USCI_UART_transmitData (__MSP430_BASEADDRESS_USCI_A0__, buffer[locIndex]);
        __delay_cycles(2500);
    }
}

void sendAtCommand(uint8_t* command,int32_t ms)
{
    int16_t locIndex;

    for(locIndex = 0; command[locIndex] != '\0'; locIndex++) 
    {
        USCI_UART_transmitData (__MSP430_BASEADDRESS_USCI_A0__,command[locIndex]);
        __delay_cycles(2500);
    }
    for (locIndex = 0 ; locIndex < ms ; locIndex++)
    {
        __delay_cycles(25000);
    }
}

void checkDisconnect(void)
{
    if(g_ui8UartBuffer[0] == 'D' &&
       g_ui8UartBuffer[1] == 'I' &&
       g_ui8UartBuffer[2] == 'S' &&
       g_ui8UartBuffer[3] == 'C' &&
       g_ui8UartBuffer[4] == 'O' &&
       g_ui8UartBuffer[5] == 'N' &&
       g_ui8UartBuffer[6] == 'N' &&
       g_ui8UartBuffer[7] == 'E' &&
       g_ui8UartBuffer[8] == 'C' &&
       g_ui8UartBuffer[9] == 'T')
    {
        setDataOff(); //If valid disconnect, set data off
    }

    g_ui8BufferEnable = 0;
}

//*****************************************************************************
//
//!  setStreamOn  
//!
//!  \brief Sets the device to stream data
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void setStreamOn(void)
{
    currentStreamStatus = START_STREAM;
    currentSpiStatus = MEASURE_STREAM;
#ifndef USE_LED_FOR_DEBUG
    //LED to indicate stream on message was received
    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN3);
#endif
}
//*****************************************************************************
//
//!  setStreamOff  
//!
//!  \brief Sets the device to stop streaming
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void setStreamOff(void)
{
    currentStreamStatus = STOP_STREAM; //Stop sending graphing data
    currentSpiStatus = MEASURE;
#ifndef USE_LED_FOR_DEBUG
    //Turn off LED to indicate stream off was received
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN3);
#endif
}

//*****************************************************************************
//
//!  setDataOff  
//!
//!  \brief Sets the device to stop sending any data
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void setDataOff(void)
{
    currentStreamStatus = STOP_STREAM;
    currentSpiStatus = IDLE; //Stop sending all data
#ifndef USE_LED_FOR_DEBUG
    //Turn off both LEDs
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN3);

    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN4);
#endif
}

//*****************************************************************************
//
//!  processStartByte  
//!
//!  \brief Processes the start byte received in the UART
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void processStartByte(void)
{
    if(g_ui8UartReceive == 0x0C) 
    {
        currentUartStatus = MSG_LENGTH; //If start byte is correct, progress state machine
    }
}

//*****************************************************************************
//
//!  processMsgLength  
//!
//!  \brief Processes the message length byte received in the UART
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void processMsgLength(void)
{
    if((g_ui8UartReceive | 0x0F) == 0x3F)
    {
        g_ui8UartMsgLength = g_ui8UartReceive & 0x0F; //Store message length
        if((g_ui8UartMsgLength > 2) && (g_ui8UartMsgLength < 7))
        {
            currentUartStatus = DEVICE_CODE; //If valid message length, progress in state machine
            g_ui8UartMsgLength--; //Update message length as we process each byte
        }
        else 
        {
            currentUartStatus = START_BYTE; //If invalid message length, return state machine to initial state
        }
    }
    else
    {
        currentUartStatus = START_BYTE; //If invalid message, return state machine to initial state
    }
}

//*****************************************************************************
//
//!  processDeviceCode  
//!
//!  \brief Processes the device code byte received in the UART
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void processDeviceCode(void)
{
    if(g_ui8UartReceive == deviceCode) //If valid device code
    {
        currentUartStatus = OP_CODE; //Progress state machine
        g_ui8UartMsgLength--; //Update message length
    }
    else
    {
        currentUartStatus = START_BYTE; //If invalid message, return state machine to initial state
    }
}

//*****************************************************************************
//
//!  processOpCode  
//!
//!  \brief Processes the opcode byte received in the UART
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void processOpCode(void)
{
    if((g_ui8UartReceive | 0x0F) == 0x4F)
    {
        g_ui8UartOpCode = g_ui8UartReceive; //Store the received opcode
        g_ui8UartMsgLength--; //Update message length
        if(g_ui8UartMsgLength == 0) currentUartStatus = END_BYTE; //If message length = 0, there was no data; check for end byte
        else currentUartStatus = START_BYTE; //Revert to start byte
    }
    else
    {
        currentUartStatus = START_BYTE; //If invalid message, return state machine to initial state
    }
}

//*****************************************************************************
//
//!  sendBLEBuffer  
//!
//!  \brief Sends a framed buffer of 20 bytes of graphing data
//!
//!  \param buffer an uint8_t[]
//!
//!  \return none
//
//*****************************************************************************
void sendBLEBuffer(uint8_t buffer[])
{
    uint8_t tempBuffer[25];
    int16_t locIndex;

    tempBuffer[0] = 0x0C; //Start byte
    tempBuffer[1] = 0x47; //Message length
    tempBuffer[2] = deviceCode; //Device code
    tempBuffer[3] = 0x49; //Stream opcode

    for(locIndex = 0; locIndex < 20; locIndex ++) //Copy 20 bytes of data
    {
        tempBuffer[locIndex+4] = buffer[locIndex];
    }

    tempBuffer[24] = 0x0D; //End byte

    for(locIndex = 0; locIndex < 25; locIndex++)
    {
        USCI_UART_transmitData (__MSP430_BASEADDRESS_USCI_A0__, tempBuffer[locIndex]);
        __delay_cycles(25*100);
    }
}

//*****************************************************************************
//
//!  processEndByte  
//!
//!  \brief Processes the end byte received in the UART
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
void processEndByte(void)
{
    if(g_ui8UartReceive == 0x0D) //If valid end byte
    {
        if(g_ui8UartOpCode == 0x41)// Set stream on
        {
            setStreamOn();
            sendStreamOn(); //Send Confirmation message
        }
        else if(g_ui8UartOpCode == 0x42) //Set stream off
        {
            setStreamOff();
            sendStreamOff(); //Send Confirmation message
            //sendBattery(getSOC());
        }

        else if(g_ui8UartOpCode == 0x46) //Set data off
        {
            setDataOff();
            sendDataOff(); //Send Confirmation message
        }

        else if(g_ui8UartOpCode == 0x47) //Echo message
        {
            sendEcho();
        }

        else if(g_ui8UartOpCode == 0x4B) //Battery capacity query
        {
            sendBattery(getSOC()); //Call fuelGauge function to get capacity and then send over BLE
        }

    }
    currentUartStatus = START_BYTE; //Reset state machine
}

//*****************************************************************************
//
//!  USCI_A0_ISR  
//!
//!  \brief ART interrupt handler called when data is received on UART  RX line.
//!
//!  \param none
//!
//!  \return none
//
//*****************************************************************************
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
#ifdef MEASURED_THROUGHPUT
    stopPrgInfoTimer(&g_prgInfo);
#endif

    switch (__even_in_range(UCA0IV,4)){
        //Vector 2 - RXIFG
        case 2:
            //Receive data
            g_ui8UartReceive = USCI_UART_receiveData(__MSP430_BASEADDRESS_USCI_A0__);

            //If a 'D' was received start looking for 'DISCONNECT'
            if(g_ui8UartReceive == 'D')
            {
                g_ui8BufferEnable = 1;
                g_ui16UartCounter = 0;
            }

            if(g_ui8BufferEnable)
            {
                g_ui8UartBuffer[g_ui16UartCounter++] = g_ui8UartReceive;
                if(g_ui16UartCounter == 10) 
                {
                    checkDisconnect(); //Check if saved 10-byte message was 'DISCONNECT'
                }
            }
            //Take action based on current status of UART state machine
            if(currentUartStatus == START_BYTE)
            {
                processStartByte();
            }
            else if(currentUartStatus == MSG_LENGTH)
            {
                processMsgLength();
            }
            else if(currentUartStatus == DEVICE_CODE)
            {
                processDeviceCode();
            }
            else if(currentUartStatus == OP_CODE)
            {
                processOpCode();
            }
            else if(currentUartStatus == END_BYTE)
            {
                processEndByte();
            }
            break;
        default: 
            break;
    }
    __bic_SR_register_on_exit(LPM0_bits);
}

