/*! \file megaio.h \brief MegaIO Control/Access function library. */
//*****************************************************************************
//
// File Name	: 'megaio.h'
// Title		: MegaIO Control/Access function library
// Author		: Pascal Stang - Copyright (C) 2004
// Created		: 5/18/2004
// Revised		: 5/18/2004
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef MEGAIO_H
#define MEGAIO_H

#include "megaio/megaioreg.h"	// include MegaIO register definitions

// defines

// function prototypes

//! initialize the MegaIO interface
uint8_t megaioInit(void);
//! write an 8-32 bit number to a MegaIO register
void megaioWriteReg(unsigned char regnum, unsigned char nbytes, unsigned long data);
//! read an 8-32 bit number from a MegaIO register
unsigned long megaioReadReg(unsigned char regnum, unsigned char nbytes);

//! set the baudrate of the MegaIO serial port
void megaioSetBaudRate(uint32_t baudrate);
//! send a byte out the MegaIO serial port
void megaioSendByte(uint8_t data);
//! get a byte from the MegaIO serial port
int megaioGetByte(void);
//! get a complete receive buffer with data from MegaIO serial port
cBuffer* megaioGetRxBuffer(void);

//! turn on MegaIO PWM and set for bitRes resolution
void megaioPWMInit(uint8_t bitRes);
//! turn off MegaIO PWM
void megaioPWMOff(void);
//! set MegaIO PWM1A duty cycle
void megaioPWMASet(uint16_t pwmDuty);
//! set MegaIO PWM1B duty cycle
void megaioPWMBSet(uint16_t pwmDuty);
//! set MegaIO prescaler division rate
void megaioSetPrescaler(uint8_t prescaleDiv);

//! do A/D conversion on channel [ch] and return result
uint16_t megaioA2DConvert(uint8_t ch);

#endif
