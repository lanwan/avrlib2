/*! \file fixedpt.h \brief Fixed-point math function library. */
//*****************************************************************************
//
// File Name	: 'fixedpt.h'
// Title		: Fixed-point math function library
// Author		: Pascal Stang - Copyright (C) 2003
// Created		: 2003.01.26
// Revised		: 2003.02.04
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
///	\ingroup general
/// \defgroup fixedpt Fixed-Point Math Function Library (fixedpt.c)
/// \code #include "fixedpt.h" \endcode
/// \par Overview
///		This library provides basic fixed-point math operations implemented in
///		C. Fixed-point math is a system by which integer variables can
///		represent fractional values with a fixed precision "behind" the decimal
///		point (like a fixed number of decimal places).  Fixed-point math is
///		typically 10x faster to execute than floating-point math, and should
///		be used when a limited amount of fractional precision is sufficient
///		(such as 4ths, or 10ths, or 128ths, etc).
///
///		\note This library is really meant more as a demonstration of fixed-pt
///		math.  For best code-efficiency and speed, you are enouraged to take
///		the code you see in these functions and copy it into your own code.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef FIXEDPT_H
#define FIXEDPT_H

#include "global.h"

// constants/macros/typdefs

// functions

//! fixedptInit() initializes fixed-point math function library
//   set the number of bits to use behind the point
void fixedptInit(uint8_t fixedPtBits);

//! convert integer to fixed-point number
int32_t fixedptConvertFromInt(int32_t int_number);

//! convert fixed-point number to integer
int32_t fixedptConvertToInt(int32_t fp_number);

//! add a and b (a+b) with fixed-point math
int32_t fixedptAdd(int32_t a, int32_t b);

//! subtract a and b (a-b) with fixed-point math
int32_t fixedptSubtract(int32_t a, int32_t b);

//! multiply a and b (a*b) with fixed-point math
int32_t fixedptMultiply(int32_t a, int32_t b);

//! divide numer by denom (numer/denom) with fixed-point math
int32_t fixedptDivide(int32_t numer, int32_t denom);

#endif
