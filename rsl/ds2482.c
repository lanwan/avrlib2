/*! \file ds2482.c \brief Dallas DS2482 I2C-to-Dallas1Wire Master Library. */
//*****************************************************************************
//
// File Name	: 'ds2482.c'
// Title		: Dallas DS2482 I2C-to-Dallas1Wire Master Library
// Author		: Pascal Stang - Copyright (C) 2004
// Created		: 2004.09.27
// Revised		: 2004.09.27
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

#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "i2c.h"
#include "ds2482.h"

#include "rprintf.h"
#include "timer.h"

// global variables
uint8_t DS2482I2cAddr;

// Functions
uint8_t ds2482Init(uint8_t i2cAddr)
{
	// select device
	DS2482I2cAddr = i2cAddr;
	// reset DS2482 chip
	return ds2482Reset(DS2482I2cAddr);

}

uint8_t ds2482Reset(uint8_t i2cAddr)
{
	// select device
	DS2482I2cAddr = i2cAddr;
	return ds2482SendCmd(DS2482_CMD_DRST);
}

uint8_t ds2482SendCmd(uint8_t cmd)
{
	uint8_t data;
	uint8_t i2cStat;

	// send command
	i2cStat = i2cMasterSendNI(DS2482I2cAddr, 1, &cmd);
	if(i2cStat == I2C_ERROR_NODEV)
	{
		rprintf("No I2C Device\r\n");
		return i2cStat;
	}
	// check status
	i2cStat = i2cMasterReceiveNI(DS2482I2cAddr, 1, &data);

//	rprintf("Cmd=0x%x  Status=0x%x\r\n", cmd, data);

	return (i2cStat == I2C_OK);
}

uint8_t ds2482SendCmdArg(uint8_t cmd, uint8_t arg)
{
	uint8_t data[2];
	uint8_t i2cStat;

	// prepare command
	data[0] = cmd;
	data[1] = arg;
	// send command
	i2cStat = i2cMasterSendNI(DS2482I2cAddr, 2, data);
	if(i2cStat == I2C_ERROR_NODEV)
	{
		rprintf("No I2C Device\r\n");
		return i2cStat;
	}
	// check status
	i2cStat = i2cMasterReceiveNI(DS2482I2cAddr, 1, data);

//	rprintf("Cmd=0x%x  Arg=0x%x  Status=0x%x\r\n", cmd, arg, data[0]);

	return (i2cStat == I2C_OK);
}

uint8_t ds2482BusyWait(void)
{
	uint8_t status;
	// set read pointer to status register
	ds2482SendCmdArg(DS2482_CMD_SRP, DS2482_READPTR_SR);
	// check status until busy bit is cleared
	do
	{
		i2cMasterReceiveNI(DS2482I2cAddr, 1, &status);
	} while(status & DS2482_STATUS_1WB);
	// return the status register value
	return status;
}

uint8_t ds2482BusReset(void)
{
	uint8_t status;
	// send 1-Wire bus reset command
	ds2482SendCmd(DS2482_CMD_1WRS);
	// wait for bus reset to finish, and get status
	status = ds2482BusyWait();
	// return state of the presence bit
	return (status & DS2482_STATUS_PPD);
}

uint8_t ds2482BusTransferBit(uint8_t bit)
{
	uint8_t status;
	// writes and reads a bit on the bus
	// wait for DS2482 to be ready
	ds2482BusyWait();
	// send 1WSB command
	ds2482SendCmdArg(DS2482_CMD_1WSB, bit?0x00:0x80);
	// wait for command to finish
	status = ds2482BusyWait();
	// return read-slot bit value
	if(status & DS2482_STATUS_SBR)
		return 1;
	else
		return 0;
}

uint8_t ds2482BusTriplet(uint8_t dir)
{
	uint8_t status;
	// this command is used to simplify search-rom operations
	// generates two read timeslots and one write timeslot
	// dir input determines value of write if reads are both 0

	// wait for DS2482 to be ready
	ds2482BusyWait();
	// send 1WSB command
	ds2482SendCmdArg(DS2482_CMD_1WT, dir?0x00:0x80);
	// wait for command to finish
	status = ds2482BusyWait();
	// return the value of the read slots
	return (status & (DS2482_STATUS_SBR|DS2482_STATUS_TSB))>>5;
}

uint8_t ds2482BusLevel(void)
{
	uint8_t status;
	// get status
	status = ds2482BusyWait();
	// return bus level value
	if(status & DS2482_STATUS_LL)
		return 1;
	else
		return 0;
}

void ds2482BusWriteByte(uint8_t data)
{
	// wait for DS2482 to be ready
	ds2482BusyWait();
	// send 1WWB command
	ds2482SendCmdArg(DS2482_CMD_1WWB, data);
}

uint8_t ds2482BusReadByte(void)
{
	uint8_t data;
	// wait for DS2482 to be ready
	ds2482BusyWait();
	// send 1WRB command
	ds2482SendCmd(DS2482_CMD_1WRB);
	// wait for read to finish
	ds2482BusyWait();
	// set read pointer to data register
	ds2482SendCmdArg(DS2482_CMD_SRP, DS2482_READPTR_RDR);
	// read data
	i2cMasterReceiveNI(DS2482I2cAddr, 1, &data);
	// return data
	return data;
}
