/*! \file uartsw2.c \brief Interrupt-driven Software UART Driver. */
//*****************************************************************************
//
// File Name	: 'uartsw2.c'
// Title		: Interrupt-driven Software UART Driver
// Author		: Pascal Stang - Copyright (C) 2002-2004
// Created		: 7/20/2002
// Revised		: 4/27/2004
// Version		: 0.6
// Target MCU	: Atmel AVR Series (intended for the ATmega16 and ATmega32)
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "timer128.h"
#include "uartsw128.h"

// Program ROM constants

// Global variables

// uartsw transmit status and data variables
static volatile uint8_t UartswTxBusy;
static volatile uint8_t UartswTxData;
static volatile uint8_t UartswTxBitNum;

// baud rate common to transmit and receive
static volatile uint8_t UartswBaudRateDiv;

// uartsw receive status and data variables
static volatile uint8_t UartswRxBusy;
static volatile uint8_t UartswRxData;
static volatile uint8_t UartswRxBitNum;
// receive buffer
static cBuffer uartswRxBuffer;               ///< uartsw receive buffer
// automatically allocate space in ram for each buffer
static char uartswRxData[UARTSW_RX_BUFFER_SIZE];

// functions

//! enable and initialize the software uart
void uartswInit(void)
{
    // initialize the buffers
	uartswInitBuffers();
	// initialize the ports
	sbi(UARTSW_TX_DDR, UARTSW_TX_PIN);
	cbi(UARTSW_RX_DDR, UARTSW_RX_PIN);
	cbi(UARTSW_RX_PORT, UARTSW_RX_PIN);
	// initialize baud rate
	uartswSetBaudRate(9600);
	
	// setup the transmitter
	UartswTxBusy = FALSE;
	// disable OC2 interrupt
	cbi(TIMSK, OCIE2);
	// attach TxBit service routine to OC2
	timerAttach(TIMER2OUTCOMPARE_INT, uartswTxBitService);
		
	// setup the receiver
	UartswRxBusy = FALSE;
	// disable OC0 interrupt
	cbi(TIMSK, OCIE0);
	// attach RxBit service routine to OC0
	timerAttach(TIMER0OUTCOMPARE_INT, uartswRxBitService);
	// INT2 trigger on rising edge
	sbi(EICRA, ISC20);
	// enable INT2 interrupt
	sbi(EIMSK, INT2);

	// turn on interrupts
	sei();
}

//! create and initialize the uart buffers
void uartswInitBuffers(void)
{
	// initialize the UART receive buffer
	bufferInit(&uartswRxBuffer, uartswRxData, UARTSW_RX_BUFFER_SIZE);
}

//! turns off software UART
void uartswOff(void)
{
	// disable interrupts
	cbi(TIMSK, OCIE2);
	cbi(TIMSK, OCIE0);
	cbi(EIMSK, INT2);
	// detach the service routines
	timerDetach(TIMER2OUTCOMPARE_INT);
	timerDetach(TIMER0OUTCOMPARE_INT);
}

void uartswSetBaudRate(uint32_t baudrate)
{
	uint16_t div;

	// set timer prescaler
	if( baudrate > (F_CPU/64L*256L) ) 
	//CHANGE THE ABOVE LINE IF CLOCK FREQ OF AVRLINX BOARD DIFFERS
	//FROM AVRSAT
	{
		// if the requested baud rate is high,
		// set timer prescalers to div-by-64
		timer2SetPrescaler(TIMERRTC_CLK_DIV64);
		timer0SetPrescaler(TIMER_CLK_DIV64);
		div = 64;
	}
	else
	{
		// if the requested baud rate is low,
		// set timer prescalers to div-by-256
		timer2SetPrescaler(TIMERRTC_CLK_DIV256);
		timer0SetPrescaler(TIMER_CLK_DIV256);
		div = 256;
	}

	// calculate division factor for requested baud rate, and set it
	//UartswBaudRateDiv = (uint8_t)(((F_CPU/64L)+(baudrate/2L))/(baudrate*1L));
	//UartswBaudRateDiv = (uint8_t)(((F_CPU/256L)+(baudrate/2L))/(baudrate*1L));
	UartswBaudRateDiv = (uint8_t)(((F_CPU/div)+(baudrate/2L))/(baudrate*1L));
}

//! returns the receive buffer structure 
cBuffer* uartswGetRxBuffer(void)
{
	// return rx buffer pointer
	return &uartswRxBuffer;
}

void uartswSendByte(uint8_t data)
{
	// wait until uart is ready
	while(UartswTxBusy);
	// set busy flag
	UartswTxBusy = TRUE;
	// save data
	UartswTxData = data;
	// set number of bits (+1 for stop bit)
	UartswTxBitNum = 9;
	
	// set the start bit
	cbi(UARTSW_TX_PORT, UARTSW_TX_PIN);//changed to cbi -JGM
	// schedule the next bit
	outb(OCR2, inb(TCNT2) + UartswBaudRateDiv); 
	// enable OC2 interrupt
	sbi(TIMSK, OCIE2);
}

//! gets a byte (if available) from the uart receive buffer
uint8_t uartswReceiveByte(uint8_t* rxData)
{
	// make sure we have a receive buffer
	if(uartswRxBuffer.size)
	{
		// make sure we have data
		if(uartswRxBuffer.datalength)
		{
			// get byte from beginning of buffer
			*rxData = bufferGetFromFront(&uartswRxBuffer);
			return TRUE;
		}
		else
		{
			// no data
			return FALSE;
		}
	}
	else
	{
		// no buffer
		return FALSE;
	}
}

void uartswTxBitService(void)
{
	if(UartswTxBitNum)
	{
		// there are bits still waiting to be transmitted
		if(UartswTxBitNum > 1)
		{
			// transmit data bits (inverted, LSB first)
			if( !(UartswTxData & 0x01) )
				cbi(UARTSW_TX_PORT, UARTSW_TX_PIN);//changed to cbi -JGM
			else
				sbi(UARTSW_TX_PORT, UARTSW_TX_PIN);//changed to sbi -JGM
			// shift bits down
			UartswTxData = UartswTxData>>1;
		}
		else
		{
			// transmit stop bit
			sbi(UARTSW_TX_PORT, UARTSW_TX_PIN);//changed to sbi -JGM
		}
		// schedule the next bit
		outb(OCR2, inb(OCR2) + UartswBaudRateDiv); //WHAT IS INB?
		// count down
		UartswTxBitNum--;
	}
	else
	{
		// transmission is done
		// clear busy flag
		UartswTxBusy = FALSE;
		// disable OC2 interrupt
		cbi(TIMSK, OCIE2);
	}
}

void uartswRxBitService(void)
{
	// this function runs on either:
	// - a rising edge interrupt
	// - Timer 0 output compare
	if(!UartswRxBusy)
	{
		// UART was not previously busy,
		// this must be is a start bit
		
		// disable INT2 interrupt
		cbi(EIMSK, INT2);
		// schedule data bit sampling 1.5 bit periods from now
		outb(OCR0, inb(TCNT0) + UartswBaudRateDiv + UartswBaudRateDiv/2);
		// clear OC0 interrupt flag
		sbi(TIFR, OCF0);
		// enable OC0 interrupt
		sbi(TIMSK, OCIE0);
		// set busy flag
		UartswRxBusy = TRUE;
		// reset bit counter
		UartswRxBitNum = 0;
		// reset data
		UartswRxData = 0;
	}
	else
	{
		// start bit has already been received
		// we're in the data bits
		
		// shift data byte to make room for new bit
		UartswRxData = UartswRxData>>1;

		// sample the data line
		if((inb(UARTSW_RX_PORTIN) & (1<<UARTSW_RX_PIN)) ) //Removed ! condition -JGM
		{
			// serial line is low
			// record '1' bit (data inverted)
			UartswRxData |= 0x80;
		}

		// increment bit counter
		UartswRxBitNum++;
		// schedule next bit sample
		outb(OCR0, inb(OCR0) + UartswBaudRateDiv);

		// check if we have a full byte
		if(UartswRxBitNum >= 8)
		{
			// save data in receive buffer
			bufferAddToEnd(&uartswRxBuffer, UartswRxData);
			// disable OC0 interrupt
			cbi(TIMSK, OCIE0);
			// clear INT2 interrupt flag
			sbi(EIFR, INTF2);
			// enable INT interrupt
			sbi(EIMSK, INT2);
			// clear busy flag
			UartswRxBusy = FALSE;
		}
	}
}

SIGNAL(SIG_INTERRUPT2)
{
	// run RxBit service routine
	uartswRxBitService();
}
