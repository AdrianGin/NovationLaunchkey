/*

Copyright (c) 2011 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "App_Keyboard.h"
#include <stdint.h>

#include "KeyboardEvents.h"


#include "HAL_MIDI.h"


void App_HandleKeyEvent(KeyboardEvent_t* kbEvent)
{
	MIDIMsg_t msg;

	msg.port = 0x00;

	kbEvent->note = KB_ApplyOctaveTranspose(kbEvent->note, KB_GetCurrentOctave(), KB_GetCurrentTranspose() );

	msg.status = kbEvent->status;
	msg.data1 = kbEvent->note;
	msg.data2 = kbEvent->velocity;

	HAL_MIDI_TxMsg(&msg);
}


#define KEYS_PER_OCTAVE (12)
#define MAX_OCTAVE		(4)
#define MAX_TRANSPOSE	(12)

uint8_t KB_ApplyOctaveTranspose(uint8_t rawNote, int8_t octave, int8_t transpose)
{
	int16_t tmpNote;

	tmpNote = rawNote + octave*KEYS_PER_OCTAVE + transpose;
	if( tmpNote > MIDI_MAX_KEY )
	{
		tmpNote = MIDI_MAX_KEY;
	}

	if( tmpNote < 0 )
	{
		tmpNote = 0;
	}

	return (uint8_t)tmpNote;
}


static int8_t KB_CurrentOctave = 0;
static int8_t KB_CurrentTranspose = 0;


inline void KB_SetOctave(int8_t newOctave)
{
	if( abs(newOctave) < MAX_OCTAVE )
	{
		KB_CurrentOctave = newOctave;
	}
}

inline void KB_SetTranspose(int8_t newTranspose)
{
	if( abs(newTranspose) < MAX_TRANSPOSE )
	{
		KB_CurrentTranspose = newTranspose;
	}
}

inline int8_t KB_GetCurrentOctave(void)
{
	return KB_CurrentOctave;
}

inline int8_t KB_GetCurrentTranspose(void)
{
	return KB_CurrentTranspose;
}
