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
#include "App_Data.h"
#include "Rescale.h"

#include "App_MIDI.h"


void Global_HandleADC(ADCEvent_t* adcEvent)
{
	MIDIMsg_t msg;

	msg.port = 0x00;

	if (adcEvent->index == ADC_PITCHBEND)
	{
		uint8_t value;
		value = CentreDetent_ApplyFilter(&PitchBendDetent, adcEvent->value);

		msg.status = MIDI_PITCH_CHANGE | AppGlobal_GetMIDIChannel();
		msg.data1 = (value & 0x01) ? 0x40 : 0;
		if (value == 0xFF)
		{
			msg.data1 = MIDI_MAX_DATA;
		}
		msg.data2 = value >> 1;

		//Centre values must pass, as the debounce may miss the centre if the debounce
		//is too long
		if ((PitchBendDetent.debounceIsActive == CD_DEBOUNCE_DISABLED))
		{

			MIDIMsg_t* savedEvent = AppMIDI_GetSavedEvent(ADC_PITCHBEND);

			uint8_t oldValue = savedEvent->data2 << 1;
			//ensure only changed events are sent out.
			if (memcmp(&msg, savedEvent, sizeof(MIDIMsg_t)) != 0)
			{
				if ((CentreDetent_Compare2Values(&PitchBendDetent, oldValue, value) != CD_NO_ZERO_CROSS))
				{
					//CentreDetent_SetDebounceState(&PitchBendDetent, CD_DEBOUNCE_ENABLED);
				}
				AppMIDI_ADCOutputMIDI(&msg, ADC_PITCHBEND);
			}
		}
		else
		{
			if ((value == PitchBendDetent.virtualCentreValue))
			{
				//ensure only changed events are sent out.
				AppMIDI_ADCOutputMIDI(&msg, ADC_PITCHBEND);
			}

		}

	}

	if (adcEvent->index == ADC_MODULATION)
	{
		msg.status = MIDI_CONTROL_CHANGE | AppGlobal_GetMIDIChannel();
		msg.data1 = MODULATION_WHEEL;
		msg.data2 = Rescale_Apply( (Rescale_t*)&Rescale_ModWheel, adcEvent->value);

		//ensure only changed events are sent out.
		AppMIDI_ADCOutputMIDI(&msg, ADC_MODULATION);
	}

}

