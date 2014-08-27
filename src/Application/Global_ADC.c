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
#include <string.h>

#include "Global_ADC.h"
#include "TimerCallbacks.h"
#include "CentreDetent.h"

//
MIDIMsg_t savedEvents[2];
void Global_HandleADC(ADCEvent_t* adcEvent)
{
	MIDIMsg_t msg;

	msg.port = 0x00;

	if( adcEvent->index == ADC_PITCHBEND )
	{
		uint8_t value;
		value = CentreDetent_ApplyFilter(&PitchBendDetent, adcEvent->value);

		msg.status = MIDI_PITCH_CHANGE | AppGlobal_GetMIDIChannel();
		msg.data1 = (value & 0x01) ? 0x40 : 0;
		msg.data2 = value >> 1;

		if( PitchBendDetent.debounceIsActive == CD_DEBOUNCE_DISABLED )
		{
			uint8_t oldValue = savedEvents[0].data2 << 1;

			if( CentreDetent_Compare2Values(&PitchBendDetent, oldValue, value) )
			{
				CentreDetent_SetDebounceState(&PitchBendDetent, CD_DEBOUNCE_ENABLED);
				savedEvents[0] = msg;
			}
			else
			{
				//ensure only changed events are sent out.
				if( memcmp( &msg, &savedEvents[0], sizeof(MIDIMsg_t)) != 0 )
				{

					HAL_MIDI_TxMsg(&msg);
					savedEvents[0] = msg;
				}
			}
		}
	}

	if( adcEvent->index == ADC_MODULATION )
	{
		msg.status = MIDI_CONTROL_CHANGE | AppGlobal_GetMIDIChannel();
		msg.data1 = MODULATION_WHEEL;
		msg.data2 = adcEvent->value >> 1;

		//ensure only changed events are sent out.
		if( memcmp( &msg, &savedEvents[1], sizeof(MIDIMsg_t)) != 0 )
		{
			HAL_MIDI_TxMsg(&msg);
			savedEvents[1] = msg;
		}
	}


}

