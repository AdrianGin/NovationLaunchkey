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

#include "EventManager.h"


uint8_t EM_ProcessKeyboard(void)
{
	KeyboardEvent_t kbEvent;
	uint8_t res;
	res = KeyboardEvents_GetEvent(&kbEvent);

	//Can do a raw Keypress here too.
	if( res == HAS_EVENT)
	{
		App_HandleKeyEvent(&kbEvent);
	}

	return 0;
}


uint8_t EM_ProcessADC(uint8_t adcIndex, uint16_t value)
{
	DispMan_Print7Seg(value, 0);
	return 0;
}


uint8_t EM_ProcessButton(uint8_t inputIndex, uint8_t value)
{
	GlobEvents_ProcessButton(inputIndex, value);
	return 0;
}


uint8_t EM_ProcessMIDI(void)
{
	//do MIDI processing here for Application
	uint8_t i;
	MIDIMsg_t msg;
	for( i = 0; i < USB_MIDI_CABLE_COUNT; i++ )
	{
		uint8_t byte;
		uint8_t result = 0;
		msg.port = i;

		result = USBMIDI_GetByte(&byte, i);

		if(result != NO_DATA_BYTE)
		{
			msg.status = byte;

			result = USBMIDI_GetByte(&byte, i);
			msg.data1 = byte;

			result = USBMIDI_GetByte(&byte, i);
			msg.data2 = byte;

			App_MIDIEvent(&msg);
		}
	}

	USBMIDI_SetReadyToReceive();
	return 0;
}








