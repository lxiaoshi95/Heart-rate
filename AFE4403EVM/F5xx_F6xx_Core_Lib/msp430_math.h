/* --COPYRIGHT--
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 *
 * msp430_math.h
 * Provides Functions to initialize the MPU9150
 * 
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  
 ******************************************************************************/
#ifndef _MSP430MATHH_
#define _MSP430MATHH_

#include "math.h"
#include "custom_math.h"

/* Mathlib function prototypes */
extern float_t msp430_sinf(float_t input);
extern float_t msp430_cosf(float_t input);
extern float_t msp430_tanf(float_t input);
extern float_t msp430_asinf(float_t input);
extern float_t msp430_acosf(float_t input);
extern float_t msp430_atanf(float_t input);
extern float_t msp430_atan2f(float_t input1, float_t input2);
extern float_t msp430_logf(float_t input);
extern float_t msp430_expf(float_t input);
extern float_t msp430_sqrtf(float_t input);
extern float_t msp430_fmodf(float_t input1, float_t input2);
extern float_t msp430_reciprocalf(float_t input);

/* 
 *  Define MSP430MATHUNMAP in the compiler options if you do not want to map
 *  the Mathlib functions to the existing function names.
 */
#ifndef MSP430MATHUNMAP

#undef sinf
#define sinf(x) msp430_sinf(x)
#undef cosf
#define cosf(x) msp430_cosf(x)
#undef tanf
#define tanf(x) msp430_tanf(x)
#undef asinf
#define asinf(x) msp430_asinf(x)
#undef acosf
#define acosf(x) msp430_acosf(x)
#undef atanf
#define atanf(x) msp430_atanf(x)
#undef atan2f
#define atan2f(x,y) msp430_atan2f(x,y)
#undef logf
#define logf(x) msp430_logf(x)
#undef expf
#define expf(x) msp430_expf(x)
#undef sqrtf
#define sqrtf(x) msp430_sqrtf(x)
#undef fmodf
#define fmodf(x,y) msp430_fmodf(x,y)
#define reciprocalf(x) msp430_reciprocalf(x)

#endif  /* MSP430MATHUNMAP */

#endif  /* _MSP430MATHH_ */
