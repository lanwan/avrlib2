/*! \file edpdebug.h \brief Emerald Data Protocol Debug Functions. */
//*****************************************************************************
//
// File Name	: 'edpdebug.h'
// Title		: Emerald Data Protocol Debug Functions
// Author		: Pascal Stang - Copyright (C) 2003
// Created		: 2003.09.20
// Revised		: 2003.09.20
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef EDPDEBUG_H
#define EDPDEBUG_H

#include "edp.h"

// functions
uint8_t edpComposeCommand(uint8_t srcEdpAddr, uint8_t* cmdBuffer);
void edpRunCommand(uint8_t destEdpAddr, uint8_t cmdLength, uint8_t* cmdBuffer);

// display functions
void edpDisplayCommand(uint8_t length, EdpCommand* edpCommand);
void edpDisplayReply(uint8_t response, EdpReply* edpReply);
//void edpDisplayReplyOld(uint8_t response, uint8_t replyLength, uint8_t* replyData);

#endif
