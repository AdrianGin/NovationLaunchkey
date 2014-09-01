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


#include <stdint.h>

#include "App_MIDI.h"
#include "HAL_ADC.h"




static MIDIMsg_t savedEvents[ADC_MODULATION+1];


void App_MIDIEvent(MIDIMsg_t* msg)
{
	USBMIDI_PutByte(msg->status, 0);
	USBMIDI_PutByte(msg->data1, 0);
	USBMIDI_PutByte(msg->data2, 0);
}

MIDIMsg_t* AppMIDI_GetSavedEvent(uint8_t index)
{
	return &savedEvents[index];
}

//compares the saved event with the passed in event and returns whether they are the same or different.
uint8_t AppMIDI_IsSavedEventDifferent(MIDIMsg_t* msg, uint8_t index)
{
	if (memcmp(msg, &savedEvents[index], sizeof(MIDIMsg_t)) != 0)
	{
		return APP_MIDI_MSG_DIFFERENT;
	}
	return !APP_MIDI_MSG_DIFFERENT;
}

uint8_t AppMIDI_ADCOutputMIDI(MIDIMsg_t* msg, uint8_t index)
{
	if ( AppMIDI_IsSavedEventDifferent(msg, index) == APP_MIDI_MSG_DIFFERENT)
	{
		HAL_MIDI_TxMsg(msg);
		savedEvents[index] = *msg;
		return APP_MIDI_MSG_DIFFERENT;
	}
	return !APP_MIDI_MSG_DIFFERENT;
}
