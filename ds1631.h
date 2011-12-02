/*! \file ds1631.h \brief Dallas DS1631 Temperature Sensor Driver Library. */
//*****************************************************************************
//
// File Name	: 'ds1631.h'
// Title		: Dallas DS1631 Temperature Sensor Driver Library
// Author		: Pascal Stang - Copyright (C) 2004
// Created		: 2004.02.10
// Revised		: 2004.02.19
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
///	\ingroup driver_hw
/// \defgroup ds1631 Dallas DS1631 Temperature Sensor Driver (ds1631.c)
/// \code #include "ds1632.h" \endcode
/// \par Overview
///		This library provides high-level functions for accessing the Dallas
///		Semiconductor DS1631 I2C Temperature Sensor.
///
///		The basic specs of the DS1631 are:
///		- operating temperature range -55 to +125 degrees C
///		- high accuracy of +/-0.5C over 0 to +70 degrees C
///		- 8 to 12-bit signed output
///		- thermostat functionality
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef DS1631_H
#define DS1631_H

#include "global.h"

// constants/macros/typdefs
#define DS1631_I2C_ADDR			0x90	///< Base I2C address of DS1631 devices

#define DS1631_CMD_STARTCONV	0x51	///< DS1631 Start conversion command byte
#define DS1631_CMD_STOPCONV		0x22	///< DS1631 Stop conversion command byte
#define DS1631_CMD_READTEMP		0xAA	///< DS1631 Read Temperature command byte
#define DS1631_CMD_ACCESSTH		0xA1	///< DS1631 TH read/write command byte
#define DS1631_CMD_ACCESSTL		0xA2	///< DS1631 TL read/write command byte
#define DS1631_CMD_ACCESSCONFIG	0xAC	///< DS1631 Config read/write command byte
#define DS1631_CMD_SWPOR		0x54	///< DS1631 Software Reset command byte

#define DS1631_CONFIG_1SHOT		0x01
#define DS1631_CONFIG_POL		0x02
#define DS1631_CONFIG_R0		0x04
#define DS1631_CONFIG_R1		0x08
#define DS1631_CONFIG_NVB		0x10
#define DS1631_CONFIG_TLF		0x20
#define DS1631_CONFIG_THF		0x40
#define DS1631_CONFIG_DONE		0x80

// functions

//! Initialize the DS1631 chip
uint8_t ds1631Init(uint8_t i2cAddr);

//! Reset the DS1631 chip to its power-on defaults
uint8_t ds1631Reset(uint8_t i2cAddr);

//! Set the configuration byte of the DS1631
void ds1631SetConfig(uint8_t i2cAddr, uint8_t config);

//! Get the configuration byte of the DS1631
uint8_t ds1631GetConfig(uint8_t i2cAddr);

//! Start a temperature conversion
void ds1631StartConvert(uint8_t i2cAddr);

//! Stop a temperature conversion (or stop continuous conversion mode)
void ds1631StopConvert(uint8_t i2cAddr);

//! Read the result of a temperature conversion
int16_t ds1631ReadTemp(uint8_t i2cAddr);

//! Set the Temp-High threshold
void ds1631SetTH(uint8_t i2cAddr, int16_t value);

//! Set the Temp-Low threshold
void ds1631SetTL(uint8_t i2cAddr, int16_t value);

//! Get the Temp-High threshold
int16_t ds1631GetTH(uint8_t i2cAddr);

//! Get the Temp-Low threshold
int16_t ds1631GetTL(uint8_t i2cAddr);

void ds1631WriteTempReg(uint8_t i2cAddr, uint8_t cmd, int16_t value);
int16_t ds1631ReadTempReg(uint8_t i2cAddr, uint8_t cmd);


#endif
