/*! \file fixedpt.c \brief Fixed-point math function library. */
//*****************************************************************************
//
// File Name	: 'fixedpt.c'
// Title		: Fixed-point math function library
// Author		: Pascal Stang - Copyright (C) 2003
// Created		: 2003.01.26
// Revised		: 2003.02.02
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************


#include "fixedpt.h"

// Program ROM constants

// Global variables
uint8_t FixedPtBits;

// Functions

// fixedptInit() initializes fixed-point math function library
void fixedptInit(uint8_t fixedPtBits)
{
	// set the number of bits to use behind the point
	FixedPtBits = fixedPtBits;
}

int32_t fixedptConvertFromInt(int32_t int_number)
{
	// convert integer to fixed-point number
	return (int_number<<FixedPtBits);
}

int32_t fixedptConvertToInt(int32_t fp_number)
{
	// convert fixed-point number to integer
	// do rounding
	if( fp_number & 1<<(FixedPtBits-1) )
	{
		// bit behind the point was a '1'
		// round up to next higher integer
		return (fp_number>>FixedPtBits)+1;
	}
	else
	{
		// bit behind the point was a '0'
		// round down (truncate) to next lower integer
		return (fp_number>>FixedPtBits);
	}
}

int32_t fixedptAdd(int32_t a, int32_t b)
{
	// add a and b (a+b) with fixed-point math
	return a+b;
}

int32_t fixedptSubtract(int32_t a, int32_t b)
{
	// subtract a and b (a-b) with fixed-point math
	return a-b;
}

int32_t fixedptMultiply(int32_t a, int32_t b)
{
	// multiply a and b (a*b) with fixed-point math
	return (a*b)>>FixedPtBits;
}

int32_t fixedptDivide(int32_t numer, int32_t denom)
{
	// divide numer by denom (numer/denom) with fixed-point math
	return (numer<<FixedPtBits)/denom;
}
