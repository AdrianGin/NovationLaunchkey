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
#include <string.h>

uint8_t EM_ProcessKeyboard(void)
{
	KeyboardEvent_t kbEvent;
	uint8_t res = HAS_EVENT;

	while( res == HAS_EVENT)
	{
		res = GenericEvents_GetEvent( (VoidBuffer_t*)&KeyboardMsgQueue, (void*)&kbEvent);
		//Can do a raw Keypress here too.
		if (res == HAS_EVENT)
		{
			App_HandleKeyEvent(&kbEvent);
		}
	}

	return 0;
}

uint8_t EM_ProcessADC(void)
{

	ADCEvent_t adcEvent;
	uint8_t res = HAS_EVENT;

	while( res == HAS_EVENT)
	{
		res = GenericEvents_GetEvent( (VoidBuffer_t*)&ADCMsgQueue, (void*)&adcEvent);
		//Can do a raw Keypress here too.
		if (res == HAS_EVENT)
		{
			DispMan_Print7Seg(adcEvent.value, 0);
		}
	}

	return 0;
}

uint8_t EM_ProcessButton(void)
{
	SwitchEvent_t swEvent;
	uint8_t res = HAS_EVENT;

	while( res == HAS_EVENT)
	{
		res = GenericEvents_GetEvent( (VoidBuffer_t*)&SwitchMsgQueue, (void*)&swEvent);
		//Can do a raw Keypress here too.
		if (res == HAS_EVENT)
		{
			GlobEvents_ProcessButton(swEvent.index, swEvent.value);
			DispMan_Print7Seg(swEvent.index, 0);
		}
	}
	
	return 0;
}

uint8_t EM_ProcessMIDI(void)
{
	//do MIDI processing here for Application
	uint8_t i;
	MIDIMsg_t msg;
	uint8_t* data;
	uint8_t len;

	len = USBAudio_GetBytesReceived();

	if (len != 0)
	{
		data = USBAudio_GetRxBuffer();
		for (i = 0; i < len; i = i + sizeof(usbMIDIMessage_t))
		{
			if ( data[i] )
			{
				memcpy(&msg, &data[i], sizeof(usbMIDIMessage_t));
				App_MIDIEvent(&msg);
			}
		}
		USBMIDI_SetReadyToReceive();
	}

	return 0;
}

