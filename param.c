/*! \file param.c \brief EEPROM Parameter Storage Library. */
//*****************************************************************************
//
// File Name	: 'param.c'
// Title		: EEPROM Parameter Storage Library
// Author		: Pascal Stang (c)2005
// Created		: 9/16/2005
// Revised		: 9/20/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************

#include <avr/eeprom.h>

#include "global.h"

uint8_t paramLoad(uint8_t* parameters, uint8_t* memaddr, uint16_t sizebytes)
{
	uint16_t i;
	uint8_t checksum_stored=0;
	uint8_t checksum=0;

	// load parameters
	eeprom_read_block(parameters, memaddr, sizebytes);
	// load checksum
	eeprom_read_block(&checksum_stored, memaddr+sizebytes, sizeof(uint8_t));

	// calculate own checksum
	for(i=0;i<sizebytes;i++)
		checksum += parameters[i];
	checksum = ~checksum;
	
	if(checksum == checksum_stored)
		return TRUE;
	else
		return FALSE;
}

void paramStore(uint8_t* parameters, uint8_t* memaddr, uint16_t sizebytes)
{
	uint16_t i;
	uint8_t checksum=0;

	// calculate checksum
	for(i=0;i<sizebytes;i++)
		checksum += parameters[i];
	checksum = ~checksum;

	// store parameters
	eeprom_write_block(parameters, memaddr, sizebytes);
	// store checksum
	eeprom_write_block(&checksum, memaddr+sizebytes, sizeof(uint8_t));
}

