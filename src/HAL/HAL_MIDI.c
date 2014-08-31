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

#include "HAL_MIDI.h"

void HAL_MIDI_TxMsg(MIDIMsg_t* msg)
{
	uint8_t bytesToSend;
	uint8_t messageIndex;

	uint8_t cableNo = 0;

	messageIndex = LookupMIDIMessage(msg->status);
	bytesToSend = GetNumberOfBytesToRead(messageIndex);

	UART_TxByte(msg->status);
	USBMIDI_PutByte(msg->status, cableNo);

	if( bytesToSend > 1)
	{

		UART_TxByte(msg->data1);
		USBMIDI_PutByte(msg->data1, cableNo);
	}

	if( bytesToSend > 2)
	{
		UART_TxByte(msg->data2);
		USBMIDI_PutByte(msg->data2, cableNo);
	}

}

void HAL_USB_MIDI_TxMsg(uint8_t cableNo, MIDIMsg_t* msg)
{

}

