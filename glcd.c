/*! \file glcd.c \brief Graphic LCD API functions. */
//*****************************************************************************
//
// File Name	: 'glcd.c'
// Title		: Graphic LCD API functions
// Author		: Pascal Stang - Copyright (C) 2002
// Date			: 5/30/2002
// Revised		: 5/30/2002
// Version		: 0.5
// Target MCU	: Atmel AVR
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

#ifndef WIN32
// AVR specific includes
	#include <avr/io.h>
	#include <avr/pgmspace.h>
#endif

#include "glcd.h"

// include hardware support
#include "ks0108.h"
// include fonts
#include "font5x7.h"
#include "fontgr.h"

// graphic routines

// set dot
void glcdSetDot(uint8_t x, uint8_t y)
{
	unsigned char temp;

	glcdSetAddress(x, y/8);
	temp = glcdDataRead();	// dummy read
	temp = glcdDataRead();	// read back current value
	glcdSetAddress(x, y/8);
	glcdDataWrite(temp | (1 << (y % 8)));

	glcdStartLine(0);
}

// clear dot
void glcdClearDot(uint8_t x, uint8_t y)
{
	unsigned char temp;

	glcdSetAddress(x, y/8);
	temp = glcdDataRead();	// dummy read
	temp = glcdDataRead();	// read back current value
	glcdSetAddress(x, y/8);
	glcdDataWrite(temp & ~(1 << (y % 8)));

	glcdStartLine(0);
}

// draw line
void glcdLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
};

// draw rectangle
void glcdRectangle(uint8_t x, uint8_t y, uint8_t a, uint8_t b)
{
  unsigned char j;

  for (j = 0; j < a; j++) {
		glcdSetDot(x, y + j);
		glcdSetDot(x + b - 1, y + j);
	}
  for (j = 0; j < b; j++)	{
		glcdSetDot(x + j, y);
		glcdSetDot(x + j, y + a - 1);
	}
}

// draw circle
void glcdCircle(uint8_t xcenter, uint8_t ycenter, uint8_t radius)
{
  int tswitch, y, x = 0;
  unsigned char d;

  d = ycenter - xcenter;
  y = radius;
  tswitch = 3 - 2 * radius;
  while (x <= y) {
    glcdSetDot(xcenter + x, ycenter + y);     glcdSetDot(xcenter + x, ycenter - y);
    glcdSetDot(xcenter - x, ycenter + y);     glcdSetDot(xcenter - x, ycenter - y);
    glcdSetDot(ycenter + y - d, ycenter + x); glcdSetDot(ycenter + y - d, ycenter - x);
    glcdSetDot(ycenter - y - d, ycenter + x); glcdSetDot(ycenter - y - d, ycenter - x);

    if (tswitch < 0) tswitch += (4 * x + 6);
    else {
      tswitch += (4 * (x - y) + 10);
      y--;
    }
    x++;
  }
}

// text routines

// write a character at the current position
void glcdWriteChar(unsigned char c)
{
	uint8_t i = 0;

	for(i=0; i<5; i++)
	{
		glcdDataWrite(pgm_read_byte(&Font5x7[((c - 0x20) * 5) + i]));
	}

	// write a spacer line
	glcdDataWrite(0x00);
	// unless we're at the end of the display
	//if(xx == 128)
	//	xx = 0;
	//else 
	//	glcdWriteData(0x00);

	//cbi(GLCD_Control, GLCD_CS1);
	//cbi(GLCD_Control, GLCD_CS2);
	glcdStartLine(0);
}

void glcdWriteCharGr(uint8_t grCharIdx)
{
	uint8_t idx;
	uint8_t grLength;
	uint8_t grStartIdx = 0;

	// get starting index of graphic bitmap
	for(idx=0; idx<grCharIdx; idx++)
	{
		// add this graphic's length to the startIdx
		// to get the startIdx of the next one
		grStartIdx += pgm_read_byte(FontGr+grStartIdx);
	}
	grLength = pgm_read_byte(FontGr+grStartIdx);

	// write the lines of the desired graphic to the display
	for(idx=0; idx<grLength; idx++)
	{
		// write the line
		glcdDataWrite(pgm_read_byte(FontGr+(grStartIdx+1)+idx));
	}
}

void glcdPutStr(unsigned char *data)
{
  while (*data) {
    glcdWriteChar(*data);
    data++;
  }
}
