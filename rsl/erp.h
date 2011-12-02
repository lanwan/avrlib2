/*! \file erp.h \brief Emerald Radio Protocol System. */
//*****************************************************************************
//
// File Name	: 'erp.h'
// Title		: Emerald Radio Protocol System
// Author		: Pascal Stang - Copyright (C) 2003
// Created		: 2003.09.10
// Revised		: 2003.09.10
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef ERP_H
#define ERP_H

#include "edp.h"

// defines and typedefs
// Packet Types (tentative)
#define ERP_ECHO			0x01
#define ERP_ECHOREPLY		0x02
#define ERP_TEST			0x03
#define ERP_EDPCOMMAND		0x10
#define ERP_EDPREPLY		0x11
#define ERP_EDPREPLYNODEV	0x12

#define CALLSIGN_FIELD_LEN		6

// structures and typedefs
typedef struct
{
	uint8_t CallSign[6];
	uint8_t ToAddress;
	uint8_t FromAddress;
	uint8_t SequenceNum;
	uint8_t Type;
	uint8_t Data[];
} ErpPacket;
#define ERP_HEADER_LEN		10

typedef struct
{
	uint8_t EdpDestAddr;
	EdpCommand EdpCommand;
} ErpEdpCommand;

typedef struct
{
	uint8_t EdpResponse;
	EdpReply EdpReply;
} ErpEdpReply;


// functions
// ERP display
void erpDisplayHeader(ErpPacket* erpPacket);
void erpDisplayPacket(ErpPacket* erpPacket, uint8_t pktLength);
void erpDisplayEdpCommand(uint8_t length, ErpEdpCommand* erpEdpCommand);
void erpDisplayEdpReply(uint8_t length, ErpEdpReply* erpEdpReply);

#endif
