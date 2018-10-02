
//------------------------------------------------------------------------------
//******************************************************************************
//  Praveen Aroul
//  HealthTech, MHR
//  (C) Texas Instruments Inc., 2014
//  All Rights Reserved.
//  Built with IAR Workbench 6.10.1
//******************************************************************************

#ifndef CALIBRATIONROUTINE_H_
#define CALIBRATIONROUTINE_H_

/* #defines */

#define FALSE   0
#define TRUE    1

/* Data Structure */

typedef struct calibRoutineStruct
{
  unsigned char ILED1_CURR_MAX_mA;      // LED1 max current
  unsigned char ILED1_CURR_MIN_mA;      // LED1 min current
  unsigned char ILED2_CURR_MAX_mA;      // LED2 max current
  unsigned char ILED2_CURR_MIN_mA;      // LED2 min current
  unsigned char TARGET_THR_PERCENT;     // Target Threshold percent
  unsigned char LOW_THR_PERCENT;        // Low Threshold Percent
  unsigned char HIGH_THR_PERCENT;       // High Threshold percent
  unsigned char HYS_PERCENT;            // Hysteresis percent
} calibRoutineParams;

/* Function Declarations */

void initCalibrationRoutine(calibRoutineParams *pCalibParams);
void calibrationLED1(unsigned long LED1data);
void calibrationLED2(unsigned long LED2data);

#endif /*CALIBRATIONROUTINE_H_*/

