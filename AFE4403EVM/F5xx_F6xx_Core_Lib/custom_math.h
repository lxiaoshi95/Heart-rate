
//*****************************************************************************
//
// \file custom_math.h
//
// \brief This file defines math custom prototypes
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
#ifndef _CUSTOM_MATH_H_
#define _CUSTOM_MATH_H_

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
#undef acos
#define acos(x)               acosf(x)
#undef asin
#define asin(x)               asinf(x)
#undef atan
#define atan(x)               atanf(x)
#undef atan2
#define atan2(x)              atan2f(x)
#undef ceil
#define ceil(x)               ceilf(x)
#undef exp
#define exp(x)                expf(x)
#undef fabs
#define fabs(x)               fabsf(x)
#undef floor
#define floor(x)              floorf(x)
#undef fmod
#define fmod(x)               fmodf(x)
#undef fmod
#define frexp(x)              frexpf(x)
#undef ldexp
#define ldexp(x)              ldexpf(x)
#undef modf
#define modf(x)               modff(x)
#undef pow
#define pow(x)                powf(x)
#undef sqrt
#define sqrt(x)               sqrtf(x)
#undef tan
#define tan(x)                tanf(x)
#undef tanh
#define tanh(x)               tanhf(x)
#undef cos
#define cos(x)                cosf(x)
#undef cosh
#define cosh(x)               coshf(x)
#undef log
#define log(x)                logf(x)
#undef log10
#define log10(x)              log10f(x)
#undef sin
#define sin(x)                sinf(x)
#undef sinh
#define sinh(x)               sinhf(x)
#undef acosh
#define acosh(x)              acoshf(x)
#undef asinh
#define asinh(x)              asinhf(x)
#undef atanh
#define atanh(x)              atanhf(x)
#undef cbrt
#define cbrt(x)               cbrtf(x)
#undef copysign
#define copysign(x)         copysignf(x)
#undef erf
#define erf(x)                erff(x)
#undef erfc
#define erfc(x)               erfcf(x)
#undef expm1
#define expm1(x)              expm1f(x)
#undef exp2
#define exp2(x)               exp2f(x)
#undef fdim
#define fdim(x)               fdimf(x)
#undef fma
#define fma(x)                fmaf(x)
#undef fmax
#define fmax(x)               fmaxf(x)
#undef fmin
#define fmin(x)               fminf(x)
#undef hypot
#define hypot(x)              hypotf(x)
#undef ilogb
#define ilogb(x)              ilogbf(x)
#undef lgamma
#define lgamma(x)             lgammaf(x)
#undef llrint
#define llrint(x)             llrintf(x)
#undef llround
#define llround(x)          llroundf(x)
#undef log1p
#define log1p(x)              log1pf(x)
#undef log2
#define log2(x)               log2f(x)
#undef logb
#define logb(x)               logbf(x)
#undef lrint
#define lrint(x)              lrintf(x)
#undef lround
#define lround(x)             lroundf(x)
#undef nan
#define nan(x)                nanf(x)
#undef nearbyint
#define nearbyint(x)        nearbyintf(x)
#undef nextafter
#define nextafter(x)        nextafterf(x)
#undef nexttoward
#define nexttoward(x)       nextafterf(x)
#undef remainder
#define remainder(x)        remainderf(x)
#undef remquo
#define remquo(x)           remquof(x)
#undef rint
#define rint(x)           rintf(x)
#undef round
#define round(x)            roundf(x)
#undef scalbn
#define scalbn(x)           scalbnf(x)
#undef scalbn
#define scalbln(x)        scalblnf(x)
#undef tgamma
#define tgamma(x)           tgammaf(x)
#undef trunc
#define trunc(x)            truncf(x)

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

#endif  /* _CUSTOM_MATH_H_ */
