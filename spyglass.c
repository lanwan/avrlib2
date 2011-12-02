/*! \file spyglass.c \brief Spyglass Control Panel UI Board Driver. */
//*****************************************************************************
//
// File Name	: 'spyglass.c'
// Title		: Spyglass Control Panel UI Board Driver
// Author		: Pascal Stang - Copyright (C) 2005
// Created		: 7/20/2005
// Revised		: 7/23/2005
// Version		: 0.9
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************


#include "global.h"
#include "rprintf.h"
#include "timer.h"
#include "i2c.h"
#include "spyglass.h"

#include "lcd.h"

uint8_t PcfCtrlData;

uint8_t pcf8574Write(uint8_t nodeAddr, uint8_t data)
{
	// write data
	return i2cMasterSendNI(PCF8574_I2C_BASE_ADDR+(nodeAddr<<1), 1, &data);
}

uint8_t pcf8574Read(uint8_t nodeAddr)
{
	uint8_t data;
	i2cMasterReceiveNI(PCF8574_I2C_BASE_ADDR+(nodeAddr<<1), 1, &data);
	return data;
}

void spyglassInit(void)
{
	i2cInit();
	i2cSetBitrate(100);
	PcfCtrlData = (SPYGLASS_LED0 | SPYGLASS_LED1 | SPYGLASS_BEEPER);
	spyglassSetLeds(0);
	spyglassSetBeeper(0);
}

uint8_t spyglassGetPushbuttons(void)
{
	return ~pcf8574Read(PCF_NODE_BUTTONS);
}

void spyglassSetLeds(uint8_t leds)
{
	PcfCtrlData = (PcfCtrlData|0x03) & ~(leds&0x03);
	pcf8574Write(PCF_NODE_CONTROL, PcfCtrlData);
}

void spyglassSetBeeper(uint8_t state)
{
	if(state)
		PcfCtrlData &=~SPYGLASS_BEEPER;		// beeper on
	else
		PcfCtrlData |= SPYGLASS_BEEPER;		// beeper off
	pcf8574Write(PCF_NODE_CONTROL, PcfCtrlData);
}

uint8_t spyglassSetLcdContrast(uint8_t contrast)
{
	uint8_t data[2];
	data[0] = 0;
	data[1] = contrast;
	return i2cMasterSendNI(MAX517_I2C_BASE_ADDR, 2, data);
}

void spyglassLcdWrite(uint8_t rs, uint8_t data)
{
	// prepare LCD control lines
	PcfCtrlData &= ~(SPYGLASS_LCD_RW | SPYGLASS_LCD_RS | SPYGLASS_LCD_E);
	if(rs)
		PcfCtrlData |= SPYGLASS_LCD_RS;
	// set LCD control lines
	pcf8574Write(PCF_NODE_CONTROL, PcfCtrlData);
	// set data lines
	pcf8574Write(PCF_NODE_LCD_DATA, data);
	// clock E line
	pcf8574Write(PCF_NODE_CONTROL, PcfCtrlData|SPYGLASS_LCD_E);
	pcf8574Write(PCF_NODE_CONTROL, PcfCtrlData);
}

void spyglassLcdWriteChar(uint8_t c)
{
	spyglassLcdWrite(1,c);
}

void spyglassLcdInit(void)
{
	// LCD function set
	spyglassLcdWrite(0,LCD_FUNCTION_DEFAULT);
	// clear LCD
	spyglassLcdWrite(0,1<<LCD_CLR);
	delay(60000);	// wait 60ms
	// set entry mode
	spyglassLcdWrite(0,1<<LCD_ENTRY_MODE | 1<<LCD_ENTRY_INC);
	// set display to on
	spyglassLcdWrite(0,1<<LCD_ON_CTRL | 1<<LCD_ON_DISPLAY );
	// move cursor to home
	spyglassLcdWrite(0,1<<LCD_HOME);
	// set data address to 0
	spyglassLcdWrite(0,1<<LCD_DDRAM | 0x00);

	// set contrast
	spyglassSetLcdContrast(20);
}

void spyglassLcdHome(void)
{
	// move cursor to home
	spyglassLcdWrite(0,1<<LCD_HOME);
}

void spyglassLcdClear(void)
{
	// clear LCD
	spyglassLcdWrite(0,1<<LCD_CLR);
}

void spyglassLcdGotoXY(uint8_t x, uint8_t y)
{
	register uint8_t DDRAMAddr;

	// remap lines into proper order
	switch(y)
	{
	case 0: DDRAMAddr = LCD_LINE0_DDRAMADDR+x; break;
	case 1: DDRAMAddr = LCD_LINE1_DDRAMADDR+x; break;
	case 2: DDRAMAddr = LCD_LINE2_DDRAMADDR+x; break;
	case 3: DDRAMAddr = LCD_LINE3_DDRAMADDR+x; break;
	default: DDRAMAddr = LCD_LINE0_DDRAMADDR+x;
	}

	// set data address
	spyglassLcdWrite(0,1<<LCD_DDRAM | DDRAMAddr);
}
