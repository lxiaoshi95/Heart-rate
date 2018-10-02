
//------------------------------------------------------------------------------
//******************************************************************************
//  Praveen Aroul
//  HealthTech, MHR
//  (C) Texas Instruments Inc., 2014
//  All Rights Reserved.
//  Built with IAR Workbench 6.10.1
//******************************************************************************

#include "CalibrationRoutine.h"
#include "AFE44xx.h"

/*----------------------------------------------------------------------------+
| Constant Definition                                                         |
+----------------------------------------------------------------------------*/
const unsigned char BLOCK_SIZE = 16;            // Samples Window size for measuring DC value
unsigned int TFREEZE = PRF;                     // Duration (Number of PRFs) to freeze the calibration routine
unsigned int TIMEWAIT = PRF / 50;               // Duration (Number of PRFs) to wait after a configuration change
const unsigned long ADC_FULL_SCALE = 2097152;   // ADC Full scale

/*----------------------------------------------------------------------------+
| Globals Definition                                                         |
+----------------------------------------------------------------------------*/
unsigned long LOW_THR_HYS_LOW;
unsigned long LOW_THR_HYS_HIGH;
unsigned long HIGH_THR_HYS_LOW;
unsigned long HIGH_THR_HYS_HIGH;
unsigned long TARGET_THR;

unsigned char ILED1_CURR_MAX_Code;
unsigned char ILED1_CURR_MIN_Code;
unsigned char ILED2_CURR_MAX_Code;
unsigned char ILED2_CURR_MIN_Code;
unsigned int maxLEDCurrent;

typedef enum {s10K, s25K, s50K, s100K, s250K, s500K, s1M} RF_VALUES;
RF_VALUES LED1RFValue = s1M;
RF_VALUES LED2RFValue = s1M;

typedef enum {sInit, sWaitTime1, sBlock1, sWaitTime2, sFreezeTime, sBlock2} CALIBRATION_STATES;
CALIBRATION_STATES LED1CalibrationState = sInit;
CALIBRATION_STATES LED2CalibrationState = sInit;

unsigned char LED1CalibrationInProgress = FALSE;        // Global variable that indicates the status of calibration
unsigned char LED2CalibrationInProgress = FALSE;        // Global variable that indicates the status of calibration

unsigned char LED1_DISABLE_HIGH_CHECK = FALSE;  // These 2 flags are essential to cover the fail modes when the measured DC 
unsigned char LED1_DISABLE_LOW_CHECK = FALSE;   // value is 
// a) lower than LOW_THR with the highest Gain setting and Max LED1 current or
// b) higher than HIGH_THR with the lowest Gain setting and Min LED1 current

unsigned char LED2_DISABLE_HIGH_CHECK = FALSE;  // These 2 flags are essential to cover the fail modes when the measured DC 
unsigned char LED2_DISABLE_LOW_CHECK = FALSE;   // value is 
// a) lower than LOW_THR with the highest Gain setting and Max LED2 current or
// b) higher than HIGH_THR with the lowest Gain setting and Min LED2 current

extern unsigned long AFE44xx_Current_Register_Settings[36];     // Array that holds the latest AFE register settings
// For any AFE register change - update the appropriate element 
// in the array corresponding to the register
/* Function Declaration */

unsigned char ledCurrent2ledCode(const unsigned char ledCurrent);

/*********************************************************************
* @fn      		initCalibrationRoutine
*
* @brief		Initializes Calibration routine variables
*
* @param   		void
*
* @return  		void
*
*********************************************************************/
void initCalibrationRoutine(calibRoutineParams *pCalibParams)
{
  LED1CalibrationState = sInit;
  LED2CalibrationState = sInit;
  
  LED1RFValue = s1M;
  LED2RFValue = s1M;
  
  LED1CalibrationInProgress = FALSE;        // Global variable that indicates the status of calibration
  LED2CalibrationInProgress = FALSE;        // Global variable that indicates the status of calibration
  
  LED1_DISABLE_HIGH_CHECK = FALSE;  // These 2 flags are essential to cover the fail modes when the measured DC 
  LED1_DISABLE_LOW_CHECK = FALSE;   // value is 
  // a) lower than LOW_THR with the highest Gain setting and Max LED1 current or
  // b) higher than HIGH_THR with the lowest Gain setting and Min LED1 current
  
  LED2_DISABLE_HIGH_CHECK = FALSE;  // These 2 flags are essential to cover the fail modes when the measured DC 
  LED2_DISABLE_LOW_CHECK = FALSE;
  
#ifdef __AFE4403__
  // Based on TX_REF and LED_RANGE set, set the MAX LED Current supported by the device.
  if ((AFE44xx_Current_Register_Settings[35] & TX_REF_3) == TX_REF_0)
  {
    if (((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_0) || ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_2))
    {
      maxLEDCurrent = 50;
    }
    else if ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_1)
    {
      maxLEDCurrent = 25;
    }
  }
  else if ((AFE44xx_Current_Register_Settings[35] & TX_REF_3) == TX_REF_1)
  {
    if (((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_0) || ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_2))
    {
      maxLEDCurrent = 100;
    }
    else if ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_1)
    {
      maxLEDCurrent = 50;
    }
  }
  else if ((AFE44xx_Current_Register_Settings[35] & TX_REF_3) == TX_REF_2)
  {
    maxLEDCurrent = 100;
  }
  else if ((AFE44xx_Current_Register_Settings[35] & TX_REF_3) == TX_REF_3)
  {
    maxLEDCurrent = 75;
  }
#else
  // Based on TX_REF and LED_RANGE set, set the MAX LED Current supported by the device.
  if (((AFE44xx_Current_Register_Settings[35] & TX_REF_3) == TX_REF_0) || ((AFE44xx_Current_Register_Settings[35] & TX_REF_3) == TX_REF_3))
  {
    if (((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_0) || ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_2))
    {
      maxLEDCurrent = 150;
    }
    else if ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_1)
    {
      maxLEDCurrent = 75;
    }
  }
  else if ((AFE44xx_Current_Register_Settings[35] & TX_REF_3) == TX_REF_1)
  {
    if (((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_0) || ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_2))
    {
      maxLEDCurrent = 100;
    }
    else if ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_1)
    {
      maxLEDCurrent = 50;
    }
  }
  else if ((AFE44xx_Current_Register_Settings[35] & TX_REF_3) == TX_REF_2)
  {
    if (((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_0) || ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_2))
    {
      maxLEDCurrent = 200;
    }
    else if ((AFE44xx_Current_Register_Settings[34] & LED_RANGE_3) == LED_RANGE_1)
    {
      maxLEDCurrent = 100;
    }
  }
#endif
  
  ILED1_CURR_MAX_Code = ledCurrent2ledCode(pCalibParams->ILED1_CURR_MAX_mA);
  ILED1_CURR_MIN_Code = ledCurrent2ledCode(pCalibParams->ILED1_CURR_MIN_mA);
  ILED2_CURR_MAX_Code = ledCurrent2ledCode(pCalibParams->ILED2_CURR_MAX_mA);
  ILED2_CURR_MIN_Code = ledCurrent2ledCode(pCalibParams->ILED2_CURR_MIN_mA);
  
  LOW_THR_HYS_LOW = ((pCalibParams->LOW_THR_PERCENT - pCalibParams->HYS_PERCENT) * ADC_FULL_SCALE)/100;
  LOW_THR_HYS_HIGH = ((pCalibParams->LOW_THR_PERCENT + pCalibParams->HYS_PERCENT) * ADC_FULL_SCALE)/100;
  HIGH_THR_HYS_LOW = ((pCalibParams->HIGH_THR_PERCENT - pCalibParams->HYS_PERCENT) * ADC_FULL_SCALE)/100;
  HIGH_THR_HYS_HIGH = ((pCalibParams->HIGH_THR_PERCENT + pCalibParams->HYS_PERCENT) * ADC_FULL_SCALE)/100;
  TARGET_THR = ((pCalibParams->TARGET_THR_PERCENT * ADC_FULL_SCALE)/100);
}

/*********************************************************************
* @fn      		calibrationLED1
*
* @brief		LED1 Calibration routine
*
* @param   		unsigned long
*
* @return  		void
*
*********************************************************************/
void calibrationLED1(unsigned long LED1data)
{
  // Variable declaration  
  static unsigned int LED1_waitTimeCount = 0;
  static unsigned int LED1_blockSizeCount = 0;
  static unsigned int LED1_freezeTimeCount = 0;
  static unsigned long LED1_Meas_DC = 0;
  static unsigned long LED1_Meas_DC_Est = 0;
  static unsigned long ILED1Code;
  unsigned long LED1InterimCode;
  
  // LED1 Calibration State machine
  switch (LED1CalibrationState)
  {
    // Init State - This state disable stage 2, sets LED1 Current to min value 
    // and resets Flags "DISABLE_LOW_CHECK" and "DISABLE_HIGH_CHECK".
    // These 2 flags are essential to cover the fail modes when the measured DC 
    // value is 
    // a) lower than LOW_THR with the highest Gain setting and Max LED current or
    // b) higher than HIGH_THR with the lowest Gain setting and Min LED current
    // During these fail modes, currently the algorithm keeps trying to measure
    // the DC value after a FREEZE period.
    // FREEZE period is defined as the minimum duration when the calibration routine
    // stays idle without processing the PPG data.
  case (sInit) :
    LED1CalibrationInProgress = TRUE;
    LED1InterimCode = (ILED1_CURR_MIN_Code<<8);
    AFE44xx_Current_Register_Settings[34] = (AFE44xx_Current_Register_Settings[34] & ~((unsigned long)LED1_CURRENT)) | LED1InterimCode;
    AFE44xx_Current_Register_Settings[32] &= ~STAGE2EN_LED1;
    Disable_AFE44xx_SPI_Read();
    AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
    AFE44xx_Reg_Write((unsigned char)LEDCNTRL, AFE44xx_Current_Register_Settings[34]);                               //0x22
    Enable_AFE44xx_SPI_Read();
    LED1_DISABLE_HIGH_CHECK = FALSE;
    LED1_DISABLE_LOW_CHECK = FALSE;
    LED1_waitTimeCount = 0;
    LED1CalibrationState = sWaitTime1;
    LED1RFValue = s1M;
    break;
    
    // Wait State 1 : In this state, the calibration routine holds off any 
    // processing for the WAIT period.
    // WAIT period is defined as the duration when any configuration changes 
    // made to the AFE (like LED Current, TIA gain, 2nd stage gain) takes effect.
  case (sWaitTime1) :
    LED1_waitTimeCount++;
    if (LED1_waitTimeCount > TIMEWAIT)
    {
      LED1_waitTimeCount = 0;
      LED1_blockSizeCount = 0;
      LED1_Meas_DC = 0;
      LED1CalibrationState = sBlock1;
    }
    else
      LED1CalibrationState = sWaitTime1;
    break;
    
    // Block State 1: In this state, the calibration routine accumulates the LED1
    // phase data for a sample set of BLOCK size. The DC value 
    // is the average value from the BLOCK size sample data. 
  case (sBlock1) :
    LED1_blockSizeCount++;
	// The DC value is the average value from the BLOCK size sample data.
    LED1_Meas_DC += LED1data;
    //    // The DC value is the average value from the BLOCK size sample data.
    //    if (LED1data > LED1_Meas_DC)
    //      LED1_Meas_DC = LED1data;
    if (LED1_blockSizeCount == BLOCK_SIZE)
    {
      LED1_blockSizeCount = 0;
      LED1_Meas_DC /= (unsigned long) BLOCK_SIZE;
      // If the DC value is greater than the target threshold, then the TIA gain (Rf)
      // is decremented from the previous Rf setting. Once the Rf is decremented, 
      // then the calibration routine enters the Wait state 1 
      // since there was an AFE configuration change.
      if (LED1_Meas_DC > TARGET_THR)
      {
        // If TIA gain is greater than 10K, then decrement the TIA gain from the previous setting.
        if (LED1RFValue > s10K)
        {
          LED1RFValue--;
          switch (LED1RFValue)
          {
          case (s1M) :
            // Set TIA gain to 1MOhm.
            AFE44xx_Current_Register_Settings[32] = (AFE44xx_Current_Register_Settings[32] & ~(RF_LED1_NONE)) | (RF_LED1_1M);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
            Enable_AFE44xx_SPI_Read();
            break;
            
          case (s500K) :
            // Set TIA gain to 500K.
            AFE44xx_Current_Register_Settings[32] = (AFE44xx_Current_Register_Settings[32] & ~(RF_LED1_NONE)) | (RF_LED1_500K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
            Enable_AFE44xx_SPI_Read();
            break;
            
          case (s250K) :
            // Set TIA gain to 250K.
            AFE44xx_Current_Register_Settings[32] = (AFE44xx_Current_Register_Settings[32] & ~(RF_LED1_NONE)) | (RF_LED1_250K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
            Enable_AFE44xx_SPI_Read();
            break;
            
          case (s100K) :
            // Set TIA gain to 100K.
            AFE44xx_Current_Register_Settings[32] = (AFE44xx_Current_Register_Settings[32] & ~(RF_LED1_NONE)) | (RF_LED1_100K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
            Enable_AFE44xx_SPI_Read();            
            break;
            
          case (s50K) :
            // Set TIA gain to 50K.
            AFE44xx_Current_Register_Settings[32] = (AFE44xx_Current_Register_Settings[32] & ~(RF_LED1_NONE)) | (RF_LED1_50K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
            Enable_AFE44xx_SPI_Read();         
            break;
            
          case (s25K) :
            // Set TIA gain to 25K.
            AFE44xx_Current_Register_Settings[32] = (AFE44xx_Current_Register_Settings[32] & ~(RF_LED1_NONE)) | (RF_LED1_25K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
            Enable_AFE44xx_SPI_Read();
            break;
            
          case (s10K) :
            // Set TIA gain to 10K.
            AFE44xx_Current_Register_Settings[32] = (AFE44xx_Current_Register_Settings[32] & ~(RF_LED1_NONE)) | (RF_LED1_10K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
            Enable_AFE44xx_SPI_Read();           
            break;
            
          default:
            // Set Default TIA gain to 500K.
            AFE44xx_Current_Register_Settings[32] = (AFE44xx_Current_Register_Settings[32] & ~(RF_LED1_NONE)) | (RF_LED1_500K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
            Enable_AFE44xx_SPI_Read();           
            break;
          }
          LED1_waitTimeCount = 0;
          LED1CalibrationState = sWaitTime1;
        }
        else //LED1RFValue == s10K
        { 
          // If the TIA Gain is at the lowest setting and if the DC value is
          // greater than high threshold, then set "DISABLE_HIGH_CHECK" flag.
          // This flag covers the fail modes when the measured DC value is 
          // higher than HIGH_THR with the lowest Gain setting and Min LED current
          if (LED1_Meas_DC > HIGH_THR_HYS_LOW)
            LED1_DISABLE_HIGH_CHECK = TRUE;
          
          // The calibration routine is set to enter Wait 2 state.
          LED1_waitTimeCount = 0;
          LED1CalibrationState = sWaitTime2;
        }
      }
      // If the DC value is less than the target threshold, then the LED current
      // is chosen as the min value between the scaled LED current and the MAX LED Current.
      // The LED current is scaled linearly such that the estimated DC value reaches the target threshold.
      else if (LED1_Meas_DC < TARGET_THR)
      {
        ILED1Code = (TARGET_THR * ILED1_CURR_MIN_Code)/LED1_Meas_DC;
        if (ILED1Code > ILED1_CURR_MAX_Code)
          ILED1Code = ILED1_CURR_MAX_Code;
        LED1InterimCode = (ILED1Code<<8);
        AFE44xx_Current_Register_Settings[34] = (AFE44xx_Current_Register_Settings[34] & ~(LED1_CURRENT)) | LED1InterimCode;
        Disable_AFE44xx_SPI_Read();   
        AFE44xx_Reg_Write((unsigned char)LEDCNTRL, AFE44xx_Current_Register_Settings[34]);                               //0x22
        Enable_AFE44xx_SPI_Read();
        LED1_Meas_DC_Est = LED1_Meas_DC * ILED1Code / ILED1_CURR_MIN_Code;
        // If the estimated DC value is lower than the LOW_THR then Stage2 is enabled
        // and the stage 2 gain is set to maximum.
        // if the estimated DC value is greater than high threshold, then set "DISABLE_LOW_CHECK" flag.
        // This flag covers the fail modes when the measured DC value is 
        // lower than LOW_THR with the highest Gain setting and Max LED current
        if (LED1_Meas_DC_Est <= LOW_THR_HYS_HIGH)
        {
          unsigned long LED1_Meas_DC_Est_temp;
          AFE44xx_Current_Register_Settings[32] |= STAGE2EN_LED1; // Enable STAGE2_EN
          LED1_Meas_DC_Est_temp = (LED1_Meas_DC_Est * 3)/2;
          AFE44xx_Current_Register_Settings[32] =  (AFE44xx_Current_Register_Settings[32] & ~(STG2GAIN_LED1)) | (STG2GAIN_LED1_3DB); // Set STG2GAIN = 1.5
          if (LED1_Meas_DC_Est_temp < TARGET_THR)
          {
            LED1_Meas_DC_Est_temp = LED1_Meas_DC_Est * 2;
            AFE44xx_Current_Register_Settings[32] =  (AFE44xx_Current_Register_Settings[32] & ~(STG2GAIN_LED1)) | (STG2GAIN_LED1_6DB); // Set STG2GAIN = 2
            if (LED1_Meas_DC_Est_temp < TARGET_THR)
            {
              LED1_Meas_DC_Est_temp = LED1_Meas_DC_Est * 3;
              AFE44xx_Current_Register_Settings[32] =  (AFE44xx_Current_Register_Settings[32] & ~(STG2GAIN_LED1)) | (STG2GAIN_LED1_9DB); // Set STG2GAIN = 3
              if (LED1_Meas_DC_Est_temp < TARGET_THR)
              {
                LED1_Meas_DC_Est_temp = LED1_Meas_DC_Est * 4;
                AFE44xx_Current_Register_Settings[32] =  (AFE44xx_Current_Register_Settings[32] & ~(STG2GAIN_LED1)) | (STG2GAIN_LED1_12DB); // Set STG2GAIN = 4
              }
            }
          }
          LED1_Meas_DC_Est = LED1_Meas_DC_Est_temp;
          //LED1_Meas_DC_Est = LED1_Meas_DC_Est * 4;      // If LED1_Meas_DC_Est < 10%, then STG2Gain must be 4 to achieve 40%
          //AFE44xx_Current_Register_Settings[32] |= (STAGE2EN_LED1 + STG2GAIN_LED1_12DB); // Enable STAGE2_EN and set STG2GAIN = 4
          Disable_AFE44xx_SPI_Read();
          AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
          Enable_AFE44xx_SPI_Read();
          if (LED1_Meas_DC_Est <= LOW_THR_HYS_HIGH)
            LED1_DISABLE_LOW_CHECK = TRUE;
        }
        // The calibration routine state machine is set to enter Wait 2 state.
        LED1_waitTimeCount = 0;
        LED1CalibrationState = sWaitTime2;
      }
      else 
        // If the DC value is greater than or equal to the target threshold, 
        // the state machine is set to Wait 2 state.
      {
        LED1_waitTimeCount = 0;
        LED1CalibrationState = sWaitTime2;
      }
    }
    else
      LED1CalibrationState = sBlock1;
    break;
    
    // Wait State 2 : In this state, the calibration routine holds off any 
    // processing for the WAIT period.
    // WAIT period is defined as the duration when any configuration changes 
    // made to the AFE (like LED Current, TIA gain, 2nd stage gain) takes effect.
  case (sWaitTime2) :
    LED1_waitTimeCount++;
    if (LED1_waitTimeCount > TIMEWAIT)
    {
      LED1_waitTimeCount = 0;
      LED1_freezeTimeCount = 0;
      LED1CalibrationState = sFreezeTime;
    }
    else
      LED1CalibrationState = sWaitTime2;
    break;
    
    // Freeze Time : In this state, the calibration routine freezes the state machone
    // from doing any processing on the PPG data.
    // FREEZE period is defined as the duration when the calibration routine
    // stays idle without processing the PPG data.
  case (sFreezeTime) :
    LED1_freezeTimeCount++;
    if (LED1_freezeTimeCount > TFREEZE)
    {
      LED1_freezeTimeCount = 0;
      LED1_blockSizeCount = 0;
      LED1_Meas_DC = 0;
      LED1CalibrationState = sBlock2;
    }
    else
      LED1CalibrationState = sFreezeTime;
    break;
    
    // Block State 2: In this state, the calibration routine accumulates the LED1
    // phase data for a sample set of BLOCK size. The DC value 
    // is the average value from the BLOCK size sample data. 
  case (sBlock2) :
    LED1_blockSizeCount++;
    LED1_Meas_DC += LED1data;
    //    if (LED1data > LED1_Meas_DC)
    //      LED1_Meas_DC = LED1data;
    if (LED1_blockSizeCount == BLOCK_SIZE)
    {
      LED1_blockSizeCount = 0;
      LED1_Meas_DC /= (unsigned long)BLOCK_SIZE;
      // When the measured DC value is 
      // a) lower than LOW_THR with the highest Gain setting and Max LED current or
      // b) higher than HIGH_THR with the lowest Gain setting and Min LED current
      // the state machine is in fail mode.
      // During these fail modes, currently the algorithm keeps trying to measure
      // the DC value after a FREEZE period. The state machine can also report 
      // calibration failure so that the user can check the hardware setup.
      if ((LED1_Meas_DC < LOW_THR_HYS_LOW && LED1_DISABLE_LOW_CHECK == TRUE) || (LED1_Meas_DC > HIGH_THR_HYS_HIGH && LED1_DISABLE_HIGH_CHECK == TRUE))
      {
        LED1_freezeTimeCount = 0;
        LED1CalibrationState = sFreezeTime;
      }
      else
      {
        // If the DC value is greater than higher threshold value, then the routine
        // begins to perform calibration again.
        if (LED1_Meas_DC > HIGH_THR_HYS_HIGH)
        {         
          LED1CalibrationState = sInit;
        }
        else
        {
          // If the DC value is less than high threshold value, then check if 
          // the DC value is lower than low threshold value.
          if (LED1_Meas_DC < LOW_THR_HYS_LOW)
          {
            // If the DC value is less than low threshold value, then TIA gain
            // set to 1MOhm and the routine begins to perform calibration again.
            LED1RFValue = s1M;
            AFE44xx_Current_Register_Settings[32] = (AFE44xx_Current_Register_Settings[32] & ~(RF_LED1_NONE)) | (RF_LED1_1M);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIAGAIN, AFE44xx_Current_Register_Settings[32]);                                //0x20
            Enable_AFE44xx_SPI_Read();
            LED1CalibrationState = sInit;
          }
          else
          {
            // If the DC value is greater than low threshold value, the calibration
            // is complete, the state machine enters Freeze state where it does not 
            // process any PPG data for a FREEZE period.
            LED1CalibrationInProgress = FALSE;           
            LED1_freezeTimeCount = 0;
            LED1CalibrationState = sFreezeTime;
          } // End of Else - LED1_Meas_DC < LOW_THR
        } // End of Else - LED1_Meas_DC > HIGH_THR
      } // End of Else - Non- fail mode
    } //  End of IF - BlockSize
    else
      LED1CalibrationState = sBlock2;
    break;
    
  default:
    LED1CalibrationState = sInit;
    break;
  }
}

/*********************************************************************
* @fn      		calibrationLED2
*
* @brief		LED2 Calibration routine
*
* @param   		unsigned long
*
* @return  		void
*
*********************************************************************/
void calibrationLED2(unsigned long LED2data)
{
  // Variable declaration
  static unsigned int LED2_waitTimeCount = 0;
  static unsigned int LED2_blockSizeCount = 0;
  static unsigned int LED2_freezeTimeCount = 0;
  static unsigned long LED2_Meas_DC = 0;
  static unsigned long LED2_Meas_DC_Est = 0;
  static unsigned long ILED2Code;
  
  // LED2 Calibration State machine
  switch (LED2CalibrationState)
  {
    // Init State - This state disable stage 2, sets LED2 Current to min value 
    // and resets Flags "DISABLE_LOW_CHECK" and "DISABLE_HIGH_CHECK".
    // These 2 flags are essential to cover the fail modes when the measured DC 
    // value is 
    // a) lower than LOW_THR with the highest Gain setting and Max LED current or
    // b) higher than HIGH_THR with the lowest Gain setting and Min LED current
    // During these fail modes, currently the algorithm keeps trying to measure
    // the DC value after a FREEZE period.
    // FREEZE period is defined as the minimum duration when the calibration routine
    // stays idle without processing the PPG data.
  case (sInit) :
    LED2CalibrationInProgress = TRUE;
    AFE44xx_Current_Register_Settings[34] = (AFE44xx_Current_Register_Settings[34] & ~(LED2_CURRENT)) | (ILED2_CURR_MIN_Code);
    AFE44xx_Current_Register_Settings[33] &= ~STAGE2EN_LED2;
    Disable_AFE44xx_SPI_Read();
    AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                           //0x21
    AFE44xx_Reg_Write((unsigned char)LEDCNTRL, AFE44xx_Current_Register_Settings[34]);                               //0x22
    Enable_AFE44xx_SPI_Read();
    LED2_DISABLE_HIGH_CHECK = FALSE;
    LED2_DISABLE_LOW_CHECK = FALSE;
    LED2_waitTimeCount = 0;
    LED2CalibrationState = sWaitTime1;
    LED2RFValue = s1M;
    break;
    
    // Wait State 1 : In this state, the calibration routine holds off any 
    // processing for the WAIT period.
    // WAIT period is defined as the duration when any configuration changes 
    // made to the AFE (like LED Current, TIA gain, 2nd stage gain) takes effect.
  case (sWaitTime1) :
    LED2_waitTimeCount++;
    if (LED2_waitTimeCount > TIMEWAIT)
    {
      LED2_waitTimeCount = 0;
      LED2_blockSizeCount = 0;
      LED2_Meas_DC = 0;
      LED2CalibrationState = sBlock1;
    }
    else
      LED2CalibrationState = sWaitTime1;
    break;
    
    // Block State 1: In this state, the calibration routine accumulates the LED1
    // phase data for a sample set of BLOCK size. The DC value 
    // is the average value from the BLOCK size sample data. 
  case (sBlock1) :
    LED2_blockSizeCount++;
    LED2_Meas_DC += LED2data;
    //    // The DC value is the average value from the BLOCK size sample data.
    //    if (LED2data > LED2_Meas_DC)
    //      LED2_Meas_DC = LED2data;
    if (LED2_blockSizeCount == BLOCK_SIZE)
    {
      LED2_blockSizeCount = 0;
      LED2_Meas_DC /= (unsigned long)BLOCK_SIZE;
      // If the DC value is greater than the target threshold, then the TIA gain (Rf)
      // is decremented from the previous Rf setting. Once the Rf is decremented, 
      // then the calibration routine enters the Wait state 1 
      // since there was an AFE configuration change.
      if (LED2_Meas_DC > TARGET_THR)
      {
        // If TIA gain is greater than 10K, then decrement the TIA gain from the previous setting.
        if (LED2RFValue > s10K)
        {
          LED2RFValue--;
          switch (LED2RFValue)
          {
          case (s1M) :
            // Set TIA gain to 1MOhm.
            AFE44xx_Current_Register_Settings[33] = (AFE44xx_Current_Register_Settings[33] & ~(RF_LED2_NONE)) | (RF_LED2_1M);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x21
            Enable_AFE44xx_SPI_Read();
            break;
          case (s500K) :
            // Set TIA gain to 500K.
            AFE44xx_Current_Register_Settings[33] = (AFE44xx_Current_Register_Settings[33] & ~(RF_LED2_NONE)) | (RF_LED2_500K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x21
            Enable_AFE44xx_SPI_Read();
            break;
          case (s250K) :
            // Set TIA gain to 250K.
            AFE44xx_Current_Register_Settings[33] = (AFE44xx_Current_Register_Settings[33] & ~(RF_LED2_NONE)) | (RF_LED2_250K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x21
            Enable_AFE44xx_SPI_Read();
            break;
          case (s100K) :
            // Set TIA gain to 100K.
            AFE44xx_Current_Register_Settings[33] = (AFE44xx_Current_Register_Settings[33] & ~(RF_LED2_NONE)) | (RF_LED2_100K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x21
            Enable_AFE44xx_SPI_Read();
            break;
          case (s50K) :
            // Set TIA gain to 50K.
            AFE44xx_Current_Register_Settings[33] = (AFE44xx_Current_Register_Settings[33] & ~(RF_LED2_NONE)) | (RF_LED2_50K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x20
            Enable_AFE44xx_SPI_Read();
            break;
          case (s25K) :
            // Set TIA gain to 25K.
            AFE44xx_Current_Register_Settings[33] = (AFE44xx_Current_Register_Settings[33] & ~(RF_LED2_NONE)) | (RF_LED2_25K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x20
            Enable_AFE44xx_SPI_Read();
            break;
          case (s10K) :
            // Set TIA gain to 10K.
            AFE44xx_Current_Register_Settings[33] = (AFE44xx_Current_Register_Settings[33] & ~(RF_LED2_NONE)) | (RF_LED2_10K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x20
            Enable_AFE44xx_SPI_Read();
            break;
          default:
            // Set TIA gain to 500K.
            AFE44xx_Current_Register_Settings[33] = (AFE44xx_Current_Register_Settings[33] & ~(RF_LED2_NONE)) | (RF_LED2_500K);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x20
            Enable_AFE44xx_SPI_Read();
            break;
          }
          LED2_waitTimeCount = 0;
          LED2CalibrationState = sWaitTime1;
        }
        else //LED2RFValue == s10K
        {
          // If the TIA Gain is at the lowest setting and if the DC value is
          // greater than high threshold, then set "DISABLE_HIGH_CHECK" flag.
          // This flag covers the fail modes when the measured DC value is 
          // higher than HIGH_THR with the lowest Gain setting and Min LED current
          if (LED2_Meas_DC > HIGH_THR_HYS_LOW)
            LED2_DISABLE_HIGH_CHECK = TRUE;
          // The calibration routine is set to enter Wait 2 state.
          LED2_waitTimeCount = 0;
          LED2CalibrationState = sWaitTime2;
        }
      }
      // If the DC value is less than the target threshold, then the LED current
      // is chosen as the min value between the scaled LED current and the MAX LED Current.
      // The LED current is scaled linearly such that the estimated DC value reaches the target threshold.
      else if (LED2_Meas_DC < TARGET_THR)
      {
        ILED2Code = (TARGET_THR * ILED2_CURR_MIN_Code)/LED2_Meas_DC;
        if (ILED2Code > ILED2_CURR_MAX_Code)
          ILED2Code = ILED2_CURR_MAX_Code;

        Disable_AFE44xx_SPI_Read();
        AFE44xx_Current_Register_Settings[34] = (AFE44xx_Current_Register_Settings[34] & ~(LED2_CURRENT)) | (ILED2Code/4);
        AFE44xx_Reg_Write((unsigned char)LEDCNTRL, AFE44xx_Current_Register_Settings[34]);                               //0x22

        AFE44xx_Current_Register_Settings[34] = (AFE44xx_Current_Register_Settings[34] & ~(LED2_CURRENT)) | (ILED2Code/2);
        AFE44xx_Reg_Write((unsigned char)LEDCNTRL, AFE44xx_Current_Register_Settings[34]);                               //0x22

        AFE44xx_Current_Register_Settings[34] = (AFE44xx_Current_Register_Settings[34] & ~(LED2_CURRENT)) | (ILED2Code);
        AFE44xx_Reg_Write((unsigned char)LEDCNTRL, AFE44xx_Current_Register_Settings[34]);                               //0x22
        Enable_AFE44xx_SPI_Read();

        LED2_Meas_DC_Est = LED2_Meas_DC * ILED2Code / ILED2_CURR_MIN_Code;
        
        // If the estimated DC value is lower than the LOW_THR then Stage2 is enabled
        // and the stage 2 gain is set to maximum.
        // if the estimated DC value is greater than high threshold, then set "DISABLE_LOW_CHECK" flag.
        // This flag covers the fail modes when the measured DC value is 
        // lower than LOW_THR with the highest Gain setting and Max LED current
        if (LED2_Meas_DC_Est <= LOW_THR_HYS_HIGH)
        {
          unsigned long LED2_Meas_DC_Est_temp;
          AFE44xx_Current_Register_Settings[33] |= STAGE2EN_LED2; // Enable STAGE2_EN
          LED2_Meas_DC_Est_temp = (LED2_Meas_DC_Est * 3)/2;
          AFE44xx_Current_Register_Settings[33] =  (AFE44xx_Current_Register_Settings[33] & ~(STG2GAIN_LED2)) | (STG2GAIN_LED2_3DB); // Set STG2GAIN = 1.5
          if (LED2_Meas_DC_Est_temp < TARGET_THR)
          {
            LED2_Meas_DC_Est_temp = LED2_Meas_DC_Est * 2;
            AFE44xx_Current_Register_Settings[33] =  (AFE44xx_Current_Register_Settings[33] & ~(STG2GAIN_LED2)) | (STG2GAIN_LED2_6DB); // Set STG2GAIN = 2
            if (LED2_Meas_DC_Est_temp < TARGET_THR)
            {
              LED2_Meas_DC_Est_temp = LED2_Meas_DC_Est * 3;
              AFE44xx_Current_Register_Settings[33] =  (AFE44xx_Current_Register_Settings[33] & ~(STG2GAIN_LED2)) | (STG2GAIN_LED2_9DB); // Set STG2GAIN = 3
              if (LED2_Meas_DC_Est_temp < TARGET_THR)
              {
                LED2_Meas_DC_Est_temp = LED2_Meas_DC_Est * 4;
                AFE44xx_Current_Register_Settings[33] =  (AFE44xx_Current_Register_Settings[33] & ~(STG2GAIN_LED2)) | (STG2GAIN_LED2_12DB); // Set STG2GAIN = 4
              }
            }
          }
          LED2_Meas_DC_Est = LED2_Meas_DC_Est_temp;        
          //LED2_Meas_DC_Est = LED2_Meas_DC_Est * 4;      // If LED2_Meas_DC_Est < 10%, then STG2Gain must be 4 to achieve 40%
          //AFE44xx_Current_Register_Settings[33] |= (STAGE2EN_LED2 + STG2GAIN_LED2_12DB); // Enable STAGE2_EN and set STG2GAIN = 4
          Disable_AFE44xx_SPI_Read();
          AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x21
          Enable_AFE44xx_SPI_Read();
          
          if (LED2_Meas_DC_Est <= LOW_THR_HYS_HIGH)
            LED2_DISABLE_LOW_CHECK = TRUE;
        }
        // The calibration routine state machine is set to enter Wait 2 state.
        LED2_waitTimeCount = 0;
        LED2CalibrationState = sWaitTime2;
      }
      else
      {
        // If the DC value is greater than or equal to the target threshold, 
        // the state machine is set to Wait 2 state.
        LED2_waitTimeCount = 0;
        LED2CalibrationState = sWaitTime2;
      }
      //      // Temp: to use AMB_DAC and STG2Gain 6/62014
      //      AFE44xx_Current_Register_Settings[33] |= STAGE2EN_LED2; // Enable STAGE2_EN
      //      AFE44xx_Current_Register_Settings[33] =  (AFE44xx_Current_Register_Settings[33] & ~(AMBDAC_0uA)) | (AMBDAC_1uA); // Set AMBDAC = 1uA
      //      AFE44xx_Current_Register_Settings[33] =  (AFE44xx_Current_Register_Settings[33] & ~(STG2GAIN_LED2)) | (STG2GAIN_LED2_9DB); // Set STG2GAIN = 3
      //      Disable_AFE44xx_SPI_Read();
      //      AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x21
      //      Enable_AFE44xx_SPI_Read();
    }
    else
      LED2CalibrationState = sBlock1;
    break;
    
    // Wait State 2 : In this state, the calibration routine holds off any 
    // processing for the WAIT period.
    // WAIT period is defined as the duration when any configuration changes 
    // made to the AFE (like LED Current, TIA gain, 2nd stage gain) takes effect.
  case (sWaitTime2) :
    LED2_waitTimeCount++;
    if (LED2_waitTimeCount > TIMEWAIT)
    {
      LED2_waitTimeCount = 0;
      LED2_freezeTimeCount = 0;
      LED2CalibrationState = sFreezeTime;
    }
    else
      LED2CalibrationState = sWaitTime2;
    break;
    
    // Freeze Time : In this state, the calibration routine freezes the state machone
    // from doing any processing on the PPG data.
    // FREEZE period is defined as the duration when the calibration routine
    // stays idle without processing the PPG data.
  case (sFreezeTime) :
    LED2_freezeTimeCount++;
    if (LED2_freezeTimeCount > TFREEZE)
    {
      LED2_freezeTimeCount = 0;
      LED2_blockSizeCount = 0;
      LED2_Meas_DC = 0;
      LED2CalibrationState = sBlock2;
    }
    else
      LED2CalibrationState = sFreezeTime;
    break;
    
    // Block State 2: In this state, the calibration routine accumulates the LED1
    // phase data for a sample set of BLOCK size. The DC value 
    // is the average value from the BLOCK size sample data. 
  case (sBlock2) :
    LED2_blockSizeCount++;
    LED2_Meas_DC += LED2data;
    //    if (LED2data > LED2_Meas_DC)
    //      LED2_Meas_DC = LED2data;
    if (LED2_blockSizeCount == BLOCK_SIZE)
    {
      LED2_blockSizeCount = 0;
      LED2_Meas_DC /= (unsigned long) BLOCK_SIZE;
      // When the measured DC value is 
      // a) lower than LOW_THR with the highest Gain setting and Max LED current or
      // b) higher than HIGH_THR with the lowest Gain setting and Min LED current
      // the state machine is in fail mode.
      // During these fail modes, currently the algorithm keeps trying to measure
      // the DC value after a FREEZE period. The state machine can also report 
      // calibration failure so that the user can check the hardware setup.
      if ((LED2_Meas_DC < LOW_THR_HYS_LOW && LED2_DISABLE_LOW_CHECK == TRUE) || (LED2_Meas_DC > HIGH_THR_HYS_HIGH && LED2_DISABLE_HIGH_CHECK == TRUE))
      {
        LED2_freezeTimeCount = 0;
        LED2CalibrationState = sFreezeTime;
      }
      else
      {
        // If the DC value is greater than higher threshold value, then the routine
        // begins to perform calibration again.
        if (LED2_Meas_DC > HIGH_THR_HYS_HIGH)
        {
          LED2CalibrationState = sInit;
        }
        else
        {
          // If the DC value is less than high threshold value, then check if 
          // the DC value is lower than low threshold value.
          if (LED2_Meas_DC < LOW_THR_HYS_LOW)
          {
            // If the DC value is less than low threshold value, then TIA gain
            // set to 1MOhm and the routine begins to perform calibration again.
            LED2RFValue = s1M;
            AFE44xx_Current_Register_Settings[33] = (AFE44xx_Current_Register_Settings[33] & ~(RF_LED2_NONE)) | (RF_LED2_1M);
            Disable_AFE44xx_SPI_Read();
            AFE44xx_Reg_Write((unsigned char)TIA_AMB_GAIN, AFE44xx_Current_Register_Settings[33]);                                //0x21
            Enable_AFE44xx_SPI_Read();
            LED2CalibrationState = sInit;
          }
          else
          {
            // If the DC value is greater than low threshold value, the calibration
            // is complete, the state machine enters Freeze state where it does not 
            // process any PPG data for a FREEZE period.
            LED2CalibrationInProgress = FALSE;
            LED2_freezeTimeCount = 0;
            LED2CalibrationState = sFreezeTime;
          } // End of Else - LED2_Meas_DC < LOW_THR_HYS_LOW
        } // End of Else - LED2_Meas_DC > HIGH_THR_HYS_HIGH
      } // End of Else - Non- fail mode
    } //  End of IF - BlockSize
    else
      LED2CalibrationState = sBlock2;
    break;
    
  default:
    LED1CalibrationState = sInit;
    break;
  }
}


/*********************************************************************
* @fn      		ledCurrent2ledCode
*
* @brief		Routine to convert LED current to LED code
*
* @param   		unsigned char
*
* @return  		void
*
*********************************************************************/
unsigned char ledCurrent2ledCode(const unsigned char ledCurrent)
{
  unsigned int ledCode;
  // calculate LED code from given LED current.
  if (ledCurrent == maxLEDCurrent)
    ledCode = 255;
  else
    ledCode = ((256 * ledCurrent)/maxLEDCurrent)+1;
  return (unsigned char)ledCode;
}
