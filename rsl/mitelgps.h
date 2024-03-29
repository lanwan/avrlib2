/*! \file mitelgps.c \brief Mitel GPS STX/ETX driver function library. */
//*****************************************************************************
//
// File Name	: 'mitelgps.h'
// Title		: Mitel GPS STX/ETX driver function library
// Author		: Pascal Stang - Copyright (C) 2002
// Created		: 2003.04.11
// Revised		: 2003.06.08
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

#ifndef MITELGPS_H
#define MITELGPS_H

#include "global.h"

// constants/macros/typdefs
// packet buffer size (must be able to contain biggest packet)
#define MITELGPS_BUFFERSIZE 0x0400

// packet delimiters
#define STX					0x02
#define ETX					0x03

// report packet types
#define MITELTYPE_NAVDATAGND	0x00
#define MITELTYPE_CHNLSTATGND	0x03
#define MITELTYPE_NAVDATA		0x40
#define MITELTYPE_RAWDATA		0x42
#define MITELTYPE_CHNLSTAT		0x43
#define MITELTYPE_RELNAVECEF	0x45
#define MITELTYPE_RELNAVRTN		0x46

// functions
void mitelgpsInit(void (*txbytefunc)(unsigned char c));
void mitelgpsSendPacket(uint8_t* data, uint8_t dataLength);
uint8_t mitelgpsProcess(cBuffer* rxBuffer);

// packet processing functions
void mitelgpsProcessNAVDATAGND(uint8_t* packet);
void mitelgpsProcessCHNLSTATGND(uint8_t* packet);
void mitelgpsProcessNAVDATA(uint8_t* packet);
void mitelgpsProcessRAWDATA(uint8_t* packet);
void mitelgpsProcessCHNLSTAT(uint8_t* packet);

// data conversions (these functions should move somewhere else)
uint32_t convertAsciiHexToInt(uint8_t* string, uint8_t numdigits);
void convertIntToAsciiHex(uint32_t num, uint8_t* string, uint8_t numdigits);



#endif
