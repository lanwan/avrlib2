/*! \file uart2.c \brief Dual UART driver with buffer support. */
//*****************************************************************************
//
// File Name	: 'uart2.c'
// Title		: Dual UART driver with buffer support
// Author		: Pascal Stang - Copyright (C) 2000-2004
// Created		: 11/20/2000
// Revised		: 07/04/2004
// Version		: 1.0
// Target MCU	: ATMEL AVR Series
// Editor Tabs	: 4
//
// Description	: This is a UART driver for AVR-series processors with two
//		hardware UARTs such as the mega161 and mega128 
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>

#include "buffer.h"
#include "uart2.h"

// UART global variables
// flag variables
volatile uint8_t   uartReadyTx[2];
volatile uint8_t   uartBufferedTx[2];
// receive and transmit buffers
cBuffer uartRxBuffer[2];
cBuffer uartTxBuffer[2];
unsigned short uartRxOverflow[2];
#ifndef UART_BUFFER_EXTERNAL_RAM
	// using internal ram,
	// automatically allocate space in ram for each buffer
	static char uart0RxData[UART0_RX_BUFFER_SIZE];
	static char uart0TxData[UART0_TX_BUFFER_SIZE];
	static char uart1RxData[UART1_RX_BUFFER_SIZE];
	static char uart1TxData[UART1_TX_BUFFER_SIZE];
#endif

typedef void (*voidFuncPtruint8_t)(unsigned char);
volatile static voidFuncPtruint8_t UartRxFunc[2];

void uartInit(void)
{
	// initialize both uarts
	uart0Init();
	uart1Init();
}

void uart0Init(void)
{
	// initialize the buffers
	uart0InitBuffers();
	// initialize user receive handlers
	UartRxFunc[0] = 0;
	// enable RxD/TxD and interrupts
	outb(UCSR0B, BV(RXCIE)|BV(TXCIE)|BV(RXEN)|BV(TXEN));
	// set default baud rate
	uartSetBaudRate(0, UART0_DEFAULT_BAUD_RATE); 
	// initialize states
	uartReadyTx[0] = TRUE;
	uartBufferedTx[0] = FALSE;
	// clear overflow count
	uartRxOverflow[0] = 0;
	// enable interrupts
	sei();
}

void uart1Init(void)
{
	// initialize the buffers
	uart1InitBuffers();
	// initialize user receive handlers
	UartRxFunc[1] = 0;
	// enable RxD/TxD and interrupts
	outb(UCSR1B, BV(RXCIE)|BV(TXCIE)|BV(RXEN)|BV(TXEN));
	// set default baud rate
	uartSetBaudRate(1, UART1_DEFAULT_BAUD_RATE);
	// initialize states
	uartReadyTx[1] = TRUE;
	uartBufferedTx[1] = FALSE;
	// clear overflow count
	uartRxOverflow[1] = 0;
	// enable interrupts
	sei();
}

void uart0InitBuffers(void)
{
	#ifndef UART_BUFFER_EXTERNAL_RAM
		// initialize the UART0 buffers
		bufferInit(&uartRxBuffer[0], uart0RxData, UART0_RX_BUFFER_SIZE);
		bufferInit(&uartTxBuffer[0], uart0TxData, UART0_TX_BUFFER_SIZE);
	#else
		// initialize the UART0 buffers
		bufferInit(&uartRxBuffer[0], (uint8_t*) UART0_RX_BUFFER_ADDR, UART0_RX_BUFFER_SIZE);
		bufferInit(&uartTxBuffer[0], (uint8_t*) UART0_TX_BUFFER_ADDR, UART0_TX_BUFFER_SIZE);
	#endif
}

void uart1InitBuffers(void)
{
	#ifndef UART_BUFFER_EXTERNAL_RAM
		// initialize the UART1 buffers
		bufferInit(&uartRxBuffer[1], uart1RxData, UART1_RX_BUFFER_SIZE);
		bufferInit(&uartTxBuffer[1], uart1TxData, UART1_TX_BUFFER_SIZE);
	#else
		// initialize the UART1 buffers
		bufferInit(&uartRxBuffer[1], (uint8_t*) UART1_RX_BUFFER_ADDR, UART1_RX_BUFFER_SIZE);
		bufferInit(&uartTxBuffer[1], (uint8_t*) UART1_TX_BUFFER_ADDR, UART1_TX_BUFFER_SIZE);
	#endif
}

void uartSetRxHandler(uint8_t nUart, void (*rx_func)(unsigned char c))
{
	// make sure the uart number is within bounds
	if(nUart < 2)
	{
		// set the receive interrupt to run the supplied user function
		UartRxFunc[nUart] = rx_func;
	}
}

void uartSetBaudRate(uint8_t nUart, uint32_t baudrate)
{
	// calculate division factor for requested baud rate, and set it
	uint16_t bauddiv = ((F_CPU+(baudrate*8L))/(baudrate*16L)-1);
	if(nUart)
	{
		outb(UBRR1L, bauddiv);
		#ifdef UBRR1H
		outb(UBRR1H, bauddiv>>8);
		#endif
	}
	else
	{
		outb(UBRR0L, bauddiv);
		#ifdef UBRR0H
		outb(UBRR0H, bauddiv>>8);
		#endif
	}
}

cBuffer* uartGetRxBuffer(uint8_t nUart)
{
	// return rx buffer pointer
	return &uartRxBuffer[nUart];
}

cBuffer* uartGetTxBuffer(uint8_t nUart)
{
	// return tx buffer pointer
	return &uartTxBuffer[nUart];
}

void uartSendByte(uint8_t nUart, uint8_t txData)
{
	// wait for the transmitter to be ready
//	while(!uartReadyTx[nUart]);
	// send byte
	if(nUart)
	{
		while(!(UCSR1A & (1<<UDRE)));
		outb(UDR1, txData);
	}
	else
	{
		while(!(UCSR0A & (1<<UDRE)));
		outb(UDR0, txData);
	}
	// set ready state to FALSE
	uartReadyTx[nUart] = FALSE;
}

void uart0SendByte(uint8_t data)
{
	// send byte on UART0
	uartSendByte(0, data);
}

void uart1SendByte(uint8_t data)
{
	// send byte on UART1
	uartSendByte(1, data);
}

int uart0GetByte(void)
{
	// get single byte from receive buffer (if available)
	uint8_t c;
	if(uartReceiveByte(0,&c))
		return c;
	else
		return -1;
}

int uart1GetByte(void)
{
	// get single byte from receive buffer (if available)
	uint8_t c;
	if(uartReceiveByte(1,&c))
		return c;
	else
		return -1;
}


uint8_t uartReceiveByte(uint8_t nUart, uint8_t* rxData)
{
	// make sure we have a receive buffer
	if(uartRxBuffer[nUart].size)
	{
		// make sure we have data
		if(uartRxBuffer[nUart].datalength)
		{
			// get byte from beginning of buffer
			*rxData = bufferGetFromFront(&uartRxBuffer[nUart]);
			return TRUE;
		}
		else
			return FALSE;			// no data
	}
	else
		return FALSE;				// no buffer
}

void uartFlushReceiveBuffer(uint8_t nUart)
{
	// flush all data from receive buffer
	bufferFlush(&uartRxBuffer[nUart]);
}

uint8_t uartReceiveBufferIsEmpty(uint8_t nUart)
{
	return (uartRxBuffer[nUart].datalength == 0);
}

void uartAddToTxBuffer(uint8_t nUart, uint8_t data)
{
	// add data byte to the end of the tx buffer
	bufferAddToEnd(&uartTxBuffer[nUart], data);
}

void uart0AddToTxBuffer(uint8_t data)
{
	uartAddToTxBuffer(0,data);
}

void uart1AddToTxBuffer(uint8_t data)
{
	uartAddToTxBuffer(1,data);
}

void uartSendTxBuffer(uint8_t nUart)
{
	// turn on buffered transmit
	uartBufferedTx[nUart] = TRUE;
	// send the first byte to get things going by interrupts
	uartSendByte(nUart, bufferGetFromFront(&uartTxBuffer[nUart]));
}

uint8_t uartSendBuffer(uint8_t nUart, char *buffer, uint16_t nBytes)
{
	register uint8_t first;
	register uint16_t i;

	// check if there's space (and that we have any bytes to send at all)
	if((uartTxBuffer[nUart].datalength + nBytes < uartTxBuffer[nUart].size) && nBytes)
	{
		// grab first character
		first = *buffer++;
		// copy user buffer to uart transmit buffer
		for(i = 0; i < nBytes-1; i++)
		{
			// put data bytes at end of buffer
			bufferAddToEnd(&uartTxBuffer[nUart], *buffer++);
		}

		// send the first byte to get things going by interrupts
		uartBufferedTx[nUart] = TRUE;
		uartSendByte(nUart, first);
		// return success
		return TRUE;
	}
	else
	{
		// return failure
		return FALSE;
	}
}

// UART Transmit Complete Interrupt Function
void uartTransmitService(uint8_t nUart)
{
	// check if buffered tx is enabled
	if(uartBufferedTx[nUart])
	{
		// check if there's data left in the buffer
		if(uartTxBuffer[nUart].datalength)
		{
			// send byte from top of buffer
			if(nUart)
				outb(UDR1,  bufferGetFromFront(&uartTxBuffer[1]) );
			else
				outb(UDR0,  bufferGetFromFront(&uartTxBuffer[0]) );
		}
		else
		{
			// no data left
			uartBufferedTx[nUart] = FALSE;
			// return to ready state
			uartReadyTx[nUart] = TRUE;
		}
	}
	else
	{
		// we're using single-byte tx mode
		// indicate transmit complete, back to ready
		uartReadyTx[nUart] = TRUE;
	}
}

// UART Receive Complete Interrupt Function
void uartReceiveService(uint8_t nUart)
{
	uint8_t c;
	// get received char
	if(nUart)
		c = inb(UDR1);
	else
		c = inb(UDR0);

	// if there's a user function to handle this receive event
	if(UartRxFunc[nUart])
	{
		// call it and pass the received data
		UartRxFunc[nUart](c);
	}
	else
	{
		// otherwise do default processing
		// put received char in buffer
		// check if there's space
		if( !bufferAddToEnd(&uartRxBuffer[nUart], c) )
		{
			// no space in buffer
			// count overflow
			uartRxOverflow[nUart]++;
		}
	}
}

UART_INTERRUPT_HANDLER(SIG_UART0_TRANS)      
{
	// service UART0 transmit interrupt
	uartTransmitService(0);
}

UART_INTERRUPT_HANDLER(SIG_UART1_TRANS)      
{
	// service UART1 transmit interrupt
	uartTransmitService(1);
}

UART_INTERRUPT_HANDLER(SIG_UART0_RECV)      
{
	// service UART0 receive interrupt
	uartReceiveService(0);
}

UART_INTERRUPT_HANDLER(SIG_UART1_RECV)      
{
	// service UART1 receive interrupt
	uartReceiveService(1);
}
