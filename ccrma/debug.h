

#ifndef _DEBUG_H
#define _DEBUG_H

#include <avr/pgmspace.h>
#include "global.h"

#define DEBUG_MODE_LCD		0x01
#define DEBUG_MODE_SERIAL	0x02
#define DEBUG_MODE_OSC		0x04

void debugInitLCD(uint8_t x, uint8_t y);

void debugInitOSC(void);

void debug(const char * fmt);

void debugFlash(uint8_t port, uint8_t pin);

#endif

