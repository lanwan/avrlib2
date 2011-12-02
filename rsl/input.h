/*! \file input.h \brief User-Input Functions. */
//*****************************************************************************
//
// File Name	: 'input.h'
// Title		: User-Input Functions
// Author		: Pascal Stang - Copyright (C) 2003
// Created		: 2003.09.11
// Revised		: 2003.09.11
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef INPUT_H
#define INPUT_H

// functions
uint8_t inputString(uint8_t termChar, uint8_t termLen, uint8_t* data);
uint8_t asciiHexToByte(uint8_t* string);
uint8_t asciiHexToNibble(uint8_t character);

#endif
