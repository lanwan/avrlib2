/*! \file rtc.h \brief real-time clock function library. */
//*****************************************************************************
//
// File Name	: 'rtc.h'
// Title		: real-time clock function library
// Author		: Pascal Stang - Copyright (C) 2002
// Created		: 5/10/2002
// Revised		: 7/12/2002
// Version		: 0.5
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

#ifndef RTC_H
#define RTC_H

#include "global.h"

// constants/macros/typdefs
typedef struct struct_RtcTime
{	
	// hardware
	uint8_t tics;
	uint16_t totaltics;
	// time of day
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	// date
	uint8_t day;
	uint8_t month;
	uint16_t year;
} RtcTimeType;


// functions
void rtcInit(void);
void rtcService(void);
RtcTimeType* rtcGetTime(void);

#endif
