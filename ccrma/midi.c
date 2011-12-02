//  Midi.c
//
//  Midi output routines for the atmel atmega163 (and others)
//  
//  depends on avrlib for buffer
//  

#include "uart.h"
#include "midi.h"
//#include "debug.h"


void midiInit() {
	uartInit();
	uartSetBaudRate(MIDI_BAUD_RATE);
}

// send a midi NOTE ON message from the uart of the form [0x9n, note, vel]
// where n is the midi channel from 0-F, note and vel are 7-bit numbers
uint8_t midiNoteOnOut(uint8_t note, uint8_t vel, uint8_t channel) {
	uartSendByte(MIDI_NOTE_ON | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & note);
	uartSendByte(MIDI_DATA_MASK & vel);

	return 0;
}

// send a midi NOTE OFF  message from the uart of the form [0x8n, note, vel]
// where n is the midi channel from 0-F, note and vel are 7-bit numbers
uint8_t midiNoteOffOut(uint8_t note, uint8_t vel, uint8_t channel) {
	uartSendByte(MIDI_NOTE_OFF | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & note);
	uartSendByte(MIDI_DATA_MASK & vel);

	return 0;
}

// send a midi CONTROL CHANGE message from the uart of the form [0xBn, controller, value]
// where n is the midi channel from 0-F, controller and value are 7-bit numbers
uint8_t midiControlChangeOut(uint8_t controller, uint8_t value, uint8_t channel) {
	uartSendByte(MIDI_CONTROL_CHANGE | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & controller);
	uartSendByte(MIDI_DATA_MASK & value);

	return 0;
}

// send a midi PROGRAM CHANGE message from the uart of the form [0xCn, program]
// where n is the midi channel from 0-F, program is a 7-bit number
uint8_t midiProgramChangeOut(uint8_t program, uint8_t channel) {
	uartSendByte(MIDI_PROGRAM_CHANGE | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & program);

	return 0;
}

// send a midi POLYPHONIC AFTERTOUCH message from the uart of the form [0xCn, controller, value]
// where n is the midi channel from 0-F, note and pressure are 7-bit numbers
uint8_t midiPolyTouchOut(uint8_t note, uint8_t pressure, uint8_t channel) {
	uartSendByte(MIDI_POLY_TOUCH | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & note);
	uartSendByte(MIDI_DATA_MASK & pressure);

	return 0;
}

// send a midi CHANNEL AFTERTOUCH message from the uart of the form [0xDn, pressure]
// where n is the midi channel from 0-F, and pressure is a 7-bit number
uint8_t midiChannelTouchOut(uint8_t pressure, uint8_t channel) {
	uartSendByte(MIDI_CHANNEL_TOUCH | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & pressure);

	return 0;
}

// send a midi PITCH BEND message from the uart of the form [0xEn, bendLSB, bendMSB ]
// where n is the midi channel from 0-F, and bendLSB and bendMSB are 7-bit numbers
// note that MIDI devices normally pack together bendLSB and bendMSB to make a 14-bit number
uint8_t midiPitchBendOut(uint8_t bendLSB, uint8_t bendMSB, uint8_t channel) {
	uartSendByte(MIDI_PITCH_BEND | (channel & MIDI_CHANNEL_MASK));
	uartSendByte(MIDI_DATA_MASK & bendLSB);
	uartSendByte(MIDI_DATA_MASK & bendMSB);
	return 0;
}
