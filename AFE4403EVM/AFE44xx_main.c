#include <intrinsics.h>
#include <string.h>

#include "descriptors.h"
#include "device.h"
#include "types.h"               //Basic Type declarations
#include "usb.h"                 //USB-specific functions
#include "HAL_I2C.h"
#include "HAL_UCS.h"
#include "HAL_PMM.h"
#include "UsbCdc.h"
#include "usbConstructs.h"
#include "AFE44xx_main.h"
#include "AFE44xx.h"
#include "CalibrationRoutine.h"
#include "config_AFE44xx.h"
#include "spansion_flash.h"

//Function declarations
VOID Init_Ports (VOID);
VOID Init_Clock (VOID);
VOID Init_TimerA1 (VOID);
BYTE retInString (char* string);
unsigned char ascii2uint8 (unsigned char asciiVal);

//Global flags set by events
volatile BYTE bCDCDataReceived_event = FALSE;   //Indicates data has been received without an open rcv operation

#define MAX_STR_LENGTH 64
char wholeString[MAX_STR_LENGTH] = "";          //The entire input string from the last 'return'
unsigned int SlowToggle_Period = 20000 - 1;
unsigned int FastToggle_Period = 1000 - 1;
unsigned int readData_Period = 67 - 1;

unsigned long AFE44xx_SPO2_Data_buf[6];
unsigned char txString[MAX_STR_LENGTH] = "";
char startCaptureFlag = 0;

enum CAP_MODE {FINITE, CONTINUOUS};
char captureMode = FINITE;
char captureInProgressFlag = 0;

char sendDataFlag = 0;
char readDataFlag = 0;

unsigned long AFE44xxRegArr[49];
unsigned char AFE44xxRegAddr[49];

// Accelerometer data
volatile int16_t MPU9150_ACCEL_Data_buf[3];
uint8_t  acc[6];

unsigned char AFE44xxAddr;
unsigned long AFE44xxRegVal;
unsigned long totalCount;
unsigned long sampleCount;

char CALIBRATION_ENABLED = FALSE;

/* Variables to modify depending on the application */
const unsigned char ILED1_CURR_MAX_mA = 45;     // LED1 max current reqd. for application
const unsigned char ILED1_CURR_MIN_mA = 5;      // LED1 min current reqd. for application
const unsigned char ILED2_CURR_MAX_mA = 45;     // LED2 max current reqd. for application
const unsigned char ILED2_CURR_MIN_mA = 5;      // LED2 min current reqd. for application
const unsigned char LOW_THR_PERCENT = 10;       // Low Threshold Percent
const unsigned char HIGH_THR_PERCENT = 90;      // High Threshold percent
const unsigned char HYS_PERCENT = 3;            // Hysteresis percent
const unsigned char TARGET_THR_PERCENT = 50;    // Target Threshold percent
/* End of Variables to modify depending on the application */
calibRoutineParams calibParams;


//extern BYTE fInvokeBSL;
BYTE fInvokeBSL;
#define INT_PB1         BIT7
// Provide clock to AFE for Crucial Tek Board.
#define ACLK_MSP430		BIT0

/*  
 * ======== main ========
 */
VOID main (VOID)
{
    unsigned char Reg_Init_i;
    uint8_t locIndex;
    
    WDTCTL = WDTPW + WDTHOLD;                                   //Stop watchdog timer

    SetVCore(3);
    Init_Clock();                                               //Init clocks
    
    // Provide clock to AFE for Crucial Tek Board.
    P1SEL |= ACLK_MSP430;
    P1DIR |= ACLK_MSP430;

    AFE44xx_PowerOn_Init();
    
    // Configure LEDs
    P1DIR |= BIT3 | BIT4;       // Set DIR for P1.3 and P1.4 (LED1 and LED2)
    P1OUT &= ~BIT4;              // Turn on LED2 to indicate start of program
    
    // Configure P1.7 for PB_INT
    P1DIR &= ~INT_PB1;
    P1REN |= INT_PB1;                                   // Enable P1.7 internal resistance
    P1OUT |= INT_PB1;                                   // Set P1.7 as pull-Up resistance
    P1IES |= INT_PB1;                                   // P1.7 Hi/Lo edge
    P1IFG &= ~INT_PB1;                           	// P1.7 IFG cleared
    P1IE |= INT_PB1;                             	// P1.7 interrupt enabled

    fInvokeBSL = 0;
    
    chargerOn(); //Turn on battery charging circuit

    // Initialize UART
    initUart();

    // Initialize SPI or I2C interfaces depending on the AFE44xx
    initAFE44xxCommInterface();

    // Init and Erase serial flash
    spansionFlashInit();
    if ( spansionFlashBulkErase() == SPANSION_FLASH_STATUS_ERROR )
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);

        // Flash LEDs to indicate a erro when erasing the flash
        for(locIndex = 0; locIndex < 11; locIndex++)
        {
            GPIO_toggleOutputOnPin(GPIO_PORT_P1,
                    GPIO_PIN3 + GPIO_PIN4);

            __delay_cycles(6000000);
        }

        // Disable flash logging
        g_bErrorFlashErase = true;
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3 + GPIO_PIN4);
    }

    USB_init();                 //Init USB
       
    //Enable various USB event handling routines
    USB_setEnabledEvents(
        kUSB_VbusOnEvent + kUSB_VbusOffEvent + kUSB_receiveCompletedEvent
        + kUSB_dataReceivedEvent + kUSB_UsbSuspendEvent + kUSB_UsbResumeEvent +
        kUSB_UsbResetEvent);

    //See if we're already attached physically to USB, and if so, connect to it
    //Normally applications don't invoke the event handlers, but this is an exception.
    if (USB_connectionInfo() & kUSB_vbusPresent){
        USB_handleVbusOnEvent();
    }

    // init I2C
    HAL_I2C_init(I2C_USCI_B0_Config);
    initMPU9150();

    __enable_interrupt();                           //Enable interrupts globally
    while (1)
    {   
        if(fInvokeBSL)
        {
          USB_disable();   				// Disable CDC connection
          __delay_cycles(200);	        // Delay
          ((void (*)())0x1000)(); // This sends execution to the BSL. When execution
                                  // returns to the user app, it will be via the reset
                                  // vector, meaning execution will re-start.
        }
        if (readDataFlag)
        {
        	readDataFlag = 0;
            AFE44xx_SPO2_Data_buf[0] = AFE44xx_Reg_Read(42);  //read RED Data
            AFE44xx_SPO2_Data_buf[1] = AFE44xx_Reg_Read(43);  //read Ambient data
            AFE44xx_SPO2_Data_buf[2] = AFE44xx_Reg_Read(44);  //read IR Data
            AFE44xx_SPO2_Data_buf[3] = AFE44xx_Reg_Read(45);  //read Ambient Data
            AFE44xx_SPO2_Data_buf[4] = AFE44xx_Reg_Read(46);  //read RED - Ambient Data
            AFE44xx_SPO2_Data_buf[5] = AFE44xx_Reg_Read(47);  //read IR - Ambient Data
            // read accelerometer data
            readAccelerometerData(acc);

			if (CALIBRATION_ENABLED == TRUE)
			{
				calibrationLED1(AFE44xx_SPO2_Data_buf[2]);
				calibrationLED2(AFE44xx_SPO2_Data_buf[0]);
			}
	  
            if (captureMode == FINITE)
            {
              sampleCount++;
              if (sampleCount == totalCount)
              {
                sampleCount = 0;
                totalCount = 1;
                Disable_AFE44xx_DRDY_Interrupt();
                captureMode = FINITE;
                captureInProgressFlag = 0;
                //P5OUT &= ~BIT0;                                                        //Turn off LED P5.0 (Green)
              }
            }
            sendDataFlag = 1;
        }
        if (sendDataFlag)
        {
          MPU9150_ACCEL_Data_buf[0] = (((int16_t) acc[0]) << 8) | acc[1];
          MPU9150_ACCEL_Data_buf[1] = (((int16_t) acc[2]) << 8) | acc[3];
          MPU9150_ACCEL_Data_buf[2] = (((int16_t) acc[4]) << 8) | acc[5];

          // read PPG data
          sendDataFlag = 0;
          txString[0] = (unsigned char) START_READ_ADC_REG_CMD;
          txString[1] = (unsigned char) SOT;
          txString[2] = (unsigned char)(AFE44xx_SPO2_Data_buf[0] & 0x000000FF);
          txString[3] = (unsigned char)((AFE44xx_SPO2_Data_buf[0] & 0x0000FF00) >> 8);
          txString[4] = (unsigned char)((AFE44xx_SPO2_Data_buf[0] & 0x00FF0000) >> 16);
          txString[5] = (unsigned char)(AFE44xx_SPO2_Data_buf[1] & 0x000000FF);
          txString[6] = (unsigned char)((AFE44xx_SPO2_Data_buf[1] & 0x0000FF00) >> 8);
          txString[7] = (unsigned char)((AFE44xx_SPO2_Data_buf[1] & 0x00FF0000) >> 16);
          txString[8] = (unsigned char)(AFE44xx_SPO2_Data_buf[2] & 0x000000FF);
          txString[9] = (unsigned char)((AFE44xx_SPO2_Data_buf[2] & 0x0000FF00) >> 8);
          txString[10] = (unsigned char)((AFE44xx_SPO2_Data_buf[2] & 0x00FF0000) >> 16);
          txString[11] = (unsigned char)(MPU9150_ACCEL_Data_buf[0] & 0x000000FF);
          txString[12] = (unsigned char)((MPU9150_ACCEL_Data_buf[0] & 0x0000FF00) >> 8);
          txString[13] = (unsigned char)((AFE44xx_SPO2_Data_buf[3] & 0x00000000) >> 16);
          txString[14] = (unsigned char)(MPU9150_ACCEL_Data_buf[1] & 0x000000FF);
          txString[15] = (unsigned char)((MPU9150_ACCEL_Data_buf[1] & 0x0000FF00) >> 8);
          txString[16] = (unsigned char)((AFE44xx_SPO2_Data_buf[4] & 0x00000000) >> 16);
          txString[17] = (unsigned char)(MPU9150_ACCEL_Data_buf[2] & 0x000000FF);
          txString[18] = (unsigned char)((MPU9150_ACCEL_Data_buf[2] & 0x0000FF00) >> 8);
          txString[19] = (unsigned char)((AFE44xx_SPO2_Data_buf[5] & 0x00000000) >> 16);
          
          txString[20] = (unsigned char) EOT;
          txString[21] = (unsigned char) CR;
          cdcSendDataInBackground((BYTE*)txString,22,CDC0_INTFNUM,0);           // Send the response over USB
          P1OUT ^= BIT3;                                                       //Toggle LED P1.3
        }
        BYTE i;
        //Check the USB state and directly main loop accordingly
        switch (USB_connectionState())
        {
            case ST_USB_DISCONNECTED:
                //__bis_SR_register(LPM3_bits + GIE);                                 //Enter LPM3 w/ interrupts enabled
                _NOP();                                                             //For Debugger
                break;

            case ST_USB_CONNECTED_NO_ENUM:
                break;

            case ST_ENUM_ACTIVE:
                //__bis_SR_register(LPM0_bits + GIE);                                 //Enter LPM0 (can't do LPM3 when active)
                _NOP();                                                             //For Debugger
                                                                                    //Exit LPM on USB receive and perform a receive
                                                                                    //operation
                if (bCDCDataReceived_event){                                        //Some data is in the buffer; begin receiving a
                                                                                    //command
                    char pieceOfString[MAX_STR_LENGTH] = "";                        //Holds the new addition to the string
                    //char outString[MAX_STR_LENGTH] = "";                            //Holds the outgoing string

                                                                                    //Add bytes in USB buffer to theCommand
                    cdcReceiveDataInBuffer((BYTE*)pieceOfString,
                        MAX_STR_LENGTH,
                        CDC0_INTFNUM);                                                         //Get the next piece of the string
                    strcat(wholeString,pieceOfString);
                    //cdcSendDataInBackground((BYTE*)pieceOfString,
                    //    strlen(pieceOfString),CDC0_INTFNUM,0);                      //Echoes back the characters received (needed
                                                                                    //for Hyperterm)

                    if (retInString(wholeString)){                                  //Has the user pressed return yet?
                        if (wholeString[0] == WRITE_REG_CMD) // AFE44xx Write Operation
                        {
                        	if (CALIBRATION_ENABLED == FALSE)
                        	{
                        		AFE44xxAddr = (ascii2uint8 (wholeString[1]) << 4) | ascii2uint8 (wholeString[2]);
                        		unsigned long AFE44xxRegData[3];
                        		AFE44xxRegData[0] = (ascii2uint8 (wholeString[3]) << 4) | ascii2uint8 (wholeString[4]);
                        		AFE44xxRegData[1] = (ascii2uint8 (wholeString[5]) << 4) | ascii2uint8 (wholeString[6]);
                        		AFE44xxRegData[2] = (ascii2uint8 (wholeString[7]) << 4) | ascii2uint8 (wholeString[8]);
                        		AFE44xxRegVal = (AFE44xxRegData[0]<<16)| (AFE44xxRegData[1]<<8) | (AFE44xxRegData[2]);

                        		// Reg_Addr != 0; Disable READ; Write value to REG; Enable READ
                        		if (AFE44xxAddr)
                        		{
                        			Disable_AFE44xx_SPI_Read();
                        			AFE44xx_Reg_Write(AFE44xxAddr, AFE44xxRegVal);
                        			//AFE44xx_Reg_Write(49, 0x008000);
                        			Enable_AFE44xx_SPI_Read();
                        		}
                        		// Reg_Addr == 0; Write value to REG;
                        		else
                        			AFE44xx_Reg_Write(AFE44xxAddr, AFE44xxRegVal);
							}
                        }
                        else if (wholeString[0] == READ_REG_CMD) // AFE44xx Read Operation
                        {
                          AFE44xxAddr = (ascii2uint8 (wholeString[1]) << 4) | ascii2uint8 (wholeString[2]);
                          // Reg_Addr != 0; Read REG;
                          if (AFE44xxAddr)
                          {
                        	  Enable_AFE44xx_SPI_Read();
                        	  AFE44xxRegVal = AFE44xx_Reg_Read(AFE44xxAddr);
                          }
                          else
                        	  AFE44xxRegVal = 0;
                          //AFE44xxRegVal = AFE44xx_Reg_Read(AFE44xxAddr);
                          txString[0] = (unsigned char) READ_REG_CMD;
                          txString[1] = (unsigned char) SOT;
                          txString[2] = (unsigned char)(AFE44xxRegVal & 0x000000FF);
                          txString[3] = (unsigned char)((AFE44xxRegVal & 0x0000FF00) >> 8);
                          txString[4] = (unsigned char)((AFE44xxRegVal & 0x00FF0000) >> 16);
                          txString[5] = (unsigned char) EOT;
                          txString[6] = (unsigned char) CR;
                          cdcSendDataInBackground((BYTE*)txString,7,CDC0_INTFNUM,0);          // Send the response over USB
                        }
                        else if (wholeString[0] == START_READ_ADC_REG_CMD) // Start AFE44x0 ADC Reg Read Command
                        {
                        	if (captureInProgressFlag == 0)
                        	{
                        		captureInProgressFlag = 1;
                        		totalCount = 1;

								sampleCount = (unsigned long)wholeString[5];
								sampleCount += (unsigned long)wholeString[4] * 256;
								sampleCount += (unsigned long)wholeString[3] * 65536;
								sampleCount += (unsigned long)wholeString[2] * 16777216;
//
								if (sampleCount)
								{
									captureMode = FINITE;
									totalCount = sampleCount;
								}
								else
									captureMode = CONTINUOUS;
								sampleCount = 0;
                        		Enable_AFE44xx_DRDY_Interrupt();			// Enable DRDY interrupt

                        		sendDataFlag = 0;
                        		AFE44xx_SPO2_Data_buf[0] = 0;
                        		AFE44xx_SPO2_Data_buf[1] = 0;
                        		AFE44xx_SPO2_Data_buf[2] = 0;
                        		AFE44xx_SPO2_Data_buf[3] = 0;
                        		AFE44xx_SPO2_Data_buf[4] = 0;
                        		AFE44xx_SPO2_Data_buf[5] = 0;
                        	}

                        }
                        else if (wholeString[0] == STOP_READ_ADC_REG_CMD) // Stop AFE44x0 ADC Reg Read Command
                        {
                        	Disable_AFE44xx_DRDY_Interrupt();
                        	sampleCount = 0;
                        	totalCount = 1;
                        	readDataFlag = 0;
                        	sendDataFlag = 0;
                        	captureMode = FINITE;
                        	captureInProgressFlag = 0;
                        }
                        else if (wholeString[0] == FW_UPGRADE_CMD) // Firmware Upgrade Command
                        {
                          USB_disable();   				// Disable CDC connection
                          TA1CTL &= ~MC_1;                              // Turn off Timer
                          Disable_AFE44xx_DRDY_Interrupt();             // Disable interrupt
                          sendDataFlag = 0;                             // Disable Recv Data flag
                          __delay_cycles(200);				// Delay
                          ((void (*)())0x1000)();			// Go to BSL
                        }
                        else if (wholeString[0] == DEV_ID_CMD) // Dev ID Command
                        {
                          txString[0] = (unsigned char)DEV_ID_CMD;
                          txString[1] = (unsigned char)SOT;
                          txString[2] = 0x34;
                          txString[3] = 0x34;
#ifdef __AFE4400__
                          txString[4] = 0x30;
                          txString[5] = 0x30;
#endif
#ifdef __AFE4490__
                          txString[4] = 0x39;
                          txString[5] = 0x30;
#endif
#ifdef __AFE4403__
                          txString[4] = 0x30;
                          txString[5] = 0x33;
#endif

                          txString[6] = (unsigned char)EOT;
                          txString[7] = (unsigned char)CR;
                          cdcSendDataInBackground((BYTE*)txString,8,CDC0_INTFNUM,0);          // Send the response over USB
                        }
                        else if (wholeString[0] == FW_VERSION_CMD) // FW Version Command
                        {
                          txString[0] = (unsigned char)FW_VERSION_CMD;
                          txString[1] = (unsigned char)SOT;
                          txString[2] = (unsigned char)AFE44x0_Major_Number;
                          txString[3] = (unsigned char)AFE44x0_Minor_Number;
                          txString[4] = (unsigned char)EOT;
                          txString[5] = (unsigned char)CR;
                          cdcSendDataInBackground((BYTE*)txString,6,CDC0_INTFNUM,0);          // Send the response over USB
                        }
						else if (wholeString[0] == CALIBRATION_CMD) // Calibration Code Command
						{
							if (wholeString[1] == CALIBRATION_TRUE)
							{
								CALIBRATION_ENABLED = TRUE;
								AFE44xx_Default_Reg_Init();
								calibParams.ILED1_CURR_MAX_mA = ILED1_CURR_MAX_mA;        // LED1 max current
								calibParams.ILED1_CURR_MIN_mA = ILED1_CURR_MIN_mA;        // LED1 min current
								calibParams.ILED2_CURR_MAX_mA = ILED2_CURR_MAX_mA;        // LED2 max current
								calibParams.ILED2_CURR_MIN_mA = ILED2_CURR_MIN_mA;        // LED2 min current
								calibParams.LOW_THR_PERCENT = LOW_THR_PERCENT;            // Low Threshold Percent
								calibParams.HIGH_THR_PERCENT = HIGH_THR_PERCENT;          // High Threshold percent
								calibParams.HYS_PERCENT = HYS_PERCENT;                    // Hysteresis percent
								calibParams.TARGET_THR_PERCENT = TARGET_THR_PERCENT;      // Target Threshold percent
								initCalibrationRoutine(&calibParams);
							}
							else if (wholeString[1] == CALIBRATION_FALSE)
							{
								CALIBRATION_ENABLED = FALSE;
							}
						}
                        for (i = 0; i < MAX_STR_LENGTH; i++){                       //Clear the string in preparation for the next
                                                                                    //one
                            wholeString[i] = 0x00;
                        }
                    }
                    bCDCDataReceived_event = FALSE;
                }
                break;

            case ST_ENUM_SUSPENDED:
                P1OUT &= ~BIT3;                                                     //When suspended, turn off LED
                //__bis_SR_register(LPM3_bits + GIE);                                 //Enter LPM3 w/ interrupts
                _NOP();
                break;

            case ST_ENUM_IN_PROGRESS:
                break;

            case ST_NOENUM_SUSPENDED:
                P1OUT &= ~BIT3;
                //__bis_SR_register(LPM3_bits + GIE);
                _NOP();
                break;

            case ST_ERROR:
                _NOP();
                break;

            default:;
        }
    }  //while(1)
} //main()

/*  
 * ======== Init_Clock ========
 */
VOID Init_Clock (VOID)
{
    //Initialization of clock module
    if (USB_PLL_XT == 2){
		#if defined (__MSP430F552x) || defined (__MSP430F550x)
			P5SEL |= 0x0C;                                      //enable XT2 pins for F5529
		#elif defined (__MSP430F563x_F663x)
			P7SEL |= 0x0C;
		#endif

        //use REFO for FLL and ACLK
        UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | (SELREF__REFOCLK);
        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__DCOCLKDIV);

        //MCLK will be driven by the FLL (not by XT2), referenced to the REFO
        Init_FLL_Settle(USB_MCLK_FREQ / 1000, USB_MCLK_FREQ / 32768);   //Start the FLL, at the freq indicated by the config
                                                                        //constant USB_MCLK_FREQ
        XT2_Start(XT2DRIVE_0);                                          //Start the "USB crystal"
    } 
	else {
		#if defined (__MSP430F552x) || defined (__MSP430F550x)
			P5SEL |= 0x10;                                      //enable XT1 pins
		#endif
        //Use the REFO oscillator to source the FLL and ACLK
        UCSCTL3 = SELREF__REFOCLK;
        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);

        //MCLK will be driven by the FLL (not by XT2), referenced to the REFO
        Init_FLL_Settle(USB_MCLK_FREQ / 1000, USB_MCLK_FREQ / 32768);   //set FLL (DCOCLK)

        XT1_Start(XT1DRIVE_0);                                          //Start the "USB crystal"
    }
}

/*
 * ======== UNMI_ISR ========
 */
#pragma vector = UNMI_VECTOR
__interrupt VOID UNMI_ISR (VOID)
{
    switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG))
    {
        case SYSUNIV_NONE:
            __no_operation();
            break;
        case SYSUNIV_NMIIFG:
            __no_operation();
            break;
        case SYSUNIV_OFIFG:
            UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG); //Clear OSC flaut Flags fault flags
            SFRIFG1 &= ~OFIFG;                          //Clear OFIFG fault flag
            break;
        case SYSUNIV_ACCVIFG:
            __no_operation();
            break;
        case SYSUNIV_BUSIFG:

                                                                //If bus error occured - the cleaning of flag and re-initializing of
                                                                //USB is required.
            SYSBERRIV = 0;                                      //clear bus error flag
            USB_disable();                                      //Disable
    }
}

/*  
 * ======== Init_TimerA1 ========
 */
VOID Init_TimerA1 (VOID)
{
    // TA1CCTL0 = CCIE;                                            //CCR0 interrupt enabled
    // TA1CTL = TASSEL_1 + TACLR;                                  //ACLK, clear TAR
}

/*  
 * ======== retInString ========
 */
//This function returns true if there's an 0x0D character in the string; and if so,
//it trims the 0x0D and anything that had followed it.
BYTE retInString (char* string)
{
    BYTE retPos = 0,i,len;
    char tempStr[MAX_STR_LENGTH] = "";

    strncpy(tempStr,string,strlen(string));                     //Make a copy of the string
    len = strlen(tempStr);
    while ((tempStr[retPos] != 0x0A) && (tempStr[retPos] != 0x0D) &&
           (retPos++ < len)) ;                                  //Find 0x0D; if not found, retPos ends up at len

    if ((retPos < len) && (tempStr[retPos] == 0x0D)){           //If 0x0D was actually found...
        for (i = 0; i < MAX_STR_LENGTH; i++){                   //Empty the buffer
            string[i] = 0x00;
        }
        strncpy(string,tempStr,retPos);                         //...trim the input string to just before 0x0D
        return ( TRUE) ;                                        //...and tell the calling function that we did so
    } else if ((retPos < len) && (tempStr[retPos] == 0x0A)){    //If 0x0D was actually found...
        for (i = 0; i < MAX_STR_LENGTH; i++){                   //Empty the buffer
            string[i] = 0x00;
        }
        strncpy(string,tempStr,retPos);                         //...trim the input string to just before 0x0D
        return ( TRUE) ;                                        //...and tell the calling function that we did so
    } else if (tempStr[retPos] == 0x0D){
        for (i = 0; i < MAX_STR_LENGTH; i++){                   //Empty the buffer
            string[i] = 0x00;
        }
        strncpy(string,tempStr,retPos);                         //...trim the input string to just before 0x0D
        return ( TRUE) ;                                        //...and tell the calling function that we did so
    } else if (retPos < len){
        for (i = 0; i < MAX_STR_LENGTH; i++){                   //Empty the buffer
            string[i] = 0x00;
        }
        strncpy(string,tempStr,retPos);                         //...trim the input string to just before 0x0D
        return ( TRUE) ;                                        //...and tell the calling function that we did so
    }

    return ( FALSE) ;                                           //Otherwise, it wasn't found
}

/*  
 * ======== TIMER1_A0_ISR ========
 */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
{
    // P1OUT ^= BIT4;                                              //Toggle LED P1.4
//    if (startCaptureFlag)
//    {
//      AFE44xx_SPO2_Data_buf[0]++;
//      if (AFE44xx_SPO2_Data_buf[0] == 501)
//        AFE44xx_SPO2_Data_buf[0] = 0;
//      AFE44xx_SPO2_Data_buf[1]++;
//      if (AFE44xx_SPO2_Data_buf[1] == 501)
//        AFE44xx_SPO2_Data_buf[1] = 0;
//      AFE44xx_SPO2_Data_buf[2]++;
//      if (AFE44xx_SPO2_Data_buf[2] == 501)
//        AFE44xx_SPO2_Data_buf[2] = 0;
//      AFE44xx_SPO2_Data_buf[3]++;
//      if (AFE44xx_SPO2_Data_buf[3] == 501)
//        AFE44xx_SPO2_Data_buf[3] = 0;
//      AFE44xx_SPO2_Data_buf[4]++;
//      if (AFE44xx_SPO2_Data_buf[4] == 501)
//        AFE44xx_SPO2_Data_buf[4] = 0;
//      AFE44xx_SPO2_Data_buf[5]++;
//      if (AFE44xx_SPO2_Data_buf[5] == 501)
//        AFE44xx_SPO2_Data_buf[5] = 0;
//      sendDataFlag = 1;
//    }     
}


// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR  //DRDY interrupt
__interrupt void Port_2(void)
{
  switch (P2IV)
  {  
  case P2IV_P2IFG5:
	  P2IFG &= ~BIT5;                     // Clear P2.5 IFG i.e Data RDY interrupt status
    readDataFlag = 1;                                 // Set Flag to read AFE44x0 ADC REG data
    break;
    
  case P2IV_NONE:
    break;
  }
}

unsigned char ascii2uint8 (unsigned char asciiVal)
{
	unsigned char uint8Val;
	
	if (asciiVal > 0x60 && asciiVal < 0x67)				// 'a' to 'f'
		uint8Val = ((asciiVal - 0x61) + 0x0A);
  	else if (asciiVal > 0x40 && asciiVal < 0x47)		// 'A' to 'F'
  		uint8Val = ((asciiVal - 0x41) + 0x0A);
  	else if (asciiVal > 0x2F && asciiVal < 0x3A)		// '0' to '9'
  		uint8Val = (asciiVal - 0x30);
  	else												// others
  		uint8Val = 0;
  		
  	return uint8Val;
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR  //Push Button interrupt
__interrupt void Port_1(void)
{
  switch (P1IV)
  {  
  case P1IV_P1IFG7:
    P1IFG &= ~INT_PB1;                 // Clear P1.7 IFG i.e INT_PB1
    fInvokeBSL = 1;
    break;
    
  case P1IV_NONE:
    break;
  }
}

void chargerOn()
{
  //P6.0, P6.1 set as output for charging circuitry
        P6DIR |= BIT0;
        P6DIR |= BIT1;

   //Pull CHRGR_CE and CHRGR_EN1_EN2 low to enable battery charging
        P6OUT &= ~BIT0;
        P6OUT &= ~BIT1;
}

