/*! \file edp.h \brief Emerald Data Protocol System. */
//*****************************************************************************
//
// File Name	: 'edp.h'
// Title		: Emerald Data Protocol System
// Author		: Pascal Stang - Copyright (C) 2003
// Created		: 2003.07.01
// Revised		: 2003.07.21
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef EDP_H
#define EDP_H

#include "edpdefs.h"

// defines
//#define EDP_DEBUG
// edp reply buffer size
#ifndef EDP_REPLY_BUFFER_SIZE
#define EDP_REPLY_BUFFER_SIZE	128
#endif
// edpSendCommand return values
#define EDP_COMMAND_OK		0
#define EDP_COMMAND_NODEV	1
// edpGetCommandReply return values
#define EDP_REPLY_OK		0
#define EDP_REPLY_BADCHKSUM	1

// structs and typedefs
typedef struct
{
	uint8_t SrcAddr;
	uint8_t Command;
	uint8_t Data[];
} EdpCommand;

typedef struct
{
	uint8_t Length;
	uint8_t Data[];
} EdpReply;

// typedefs
typedef uint8_t (*EdpSlaveHandlerFuncType)(uint8_t edpCmdLength, EdpCommand* edpCmd,
					uint8_t edpReplyLengthMax, EdpReply* edpReply);

// functions
void edpInit(void);
void edpSetSlaveHandler(EdpSlaveHandlerFuncType edpSlaveHandlerFunction);

// ************ EDP Master operations ************
uint8_t edpSendCommand(uint8_t deviceAddr, uint8_t cmdLength, EdpCommand* edpCommand);
uint8_t edpGetCommandReply(uint8_t* responseCode, EdpReply** edpReply);
//uint8_t edpSendCommand(uint8_t deviceAddr, uint8_t sendLength, uint8_t* sendData);
//uint8_t edpGetCommandReply(uint8_t* responseCode, uint8_t* replyLength, uint8_t** replyData);

// ************ EDP Slave operations ************
void edpSlaveReceiveService(uint8_t receiveDataLength, uint8_t* receiveData);
uint8_t edpSlaveTransmitService(uint8_t transmitDataLengthMax, uint8_t* transmitData);

#endif
