#ifndef _MIDI_H
#define _MIDI_H

#define MIDI_NOTE_ON 	0x90
#define MIDI_NOTE_OFF 	0x80

// 1010cccc 0nnnnnnn 0vvvvvvv
#define MIDI_POLY_TOUCH 	0xA0
// 1011cccc 0nnnnnnn 0vvvvvvv
#define MIDI_CONTROL_CHANGE	0xB0
// 1100cccc 0ppppppp
#define MIDI_PROGRAM_CHANGE	0xC0

#define MIDI_CHANNEL_TOUCH	0xD0

#define MIDI_PITCH_BEND	        0xE0

#define MIDI_DATA_MASK			0x7F
#define MIDI_STATUS_MASK		0xF0
#define MIDI_CHANNEL_MASK		0x0F

#define MIDI_BAUD_RATE			31250

#include "global.h"
#include "buffer.h"

void midiInit(void);
uint8_t midiNoteOnOut(uint8_t note, uint8_t vel, uint8_t channel);
uint8_t midiNoteOffOut(uint8_t note, uint8_t vel, uint8_t channel);
uint8_t midiControlChangeOut(uint8_t controller, uint8_t value, uint8_t channel);
uint8_t midiProgramChangeOut(uint8_t program, uint8_t channel);
uint8_t midiPolyTouchOut(uint8_t note, uint8_t pressure, uint8_t channel);
uint8_t midiChannelTouchOut(uint8_t pressure, uint8_t channel);
uint8_t midiPitchBendOut(uint8_t bendLSB, uint8_t bendMSB, uint8_t channel);

#endif
