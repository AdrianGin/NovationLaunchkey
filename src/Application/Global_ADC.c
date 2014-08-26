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

typedef struct
{
	//values which represent the max and min limits of the ADC value. (ADC events are 8bit only)
	uint16_t max;
	uint16_t min;

} ADC_Limits_t;

typedef struct
{
	//either side of the "centre" is a threshold which must be exceeded before any change is registered
	uint8_t centre;
	uint8_t threshold;

	//the value which is given to inputs within the thresholds.
	uint8_t virtualCentreValue;

} ADC_CentreDetent_t;


#define CENTRE_DETENT_MULT_FACTOR (0x100)
//Returns the conditioned value after applying a centre detent.
//Resolution / Dynamic range is not lost, but certain values may be skipped.
uint8_t Global_ApplyCentreDetent(ADC_CentreDetent_t* filter, uint8_t value)
{
	uint16_t topThreshold = filter->centre + filter->threshold;
	uint16_t botThreshold = filter->centre - filter->threshold;

	uint16_t grad;
	uint16_t offset;
	uint16_t newVal;

	if( (value <= topThreshold) && (value >= botThreshold) )
	{
		return filter->virtualCentreValue;
	}
	else
	{
		if(value < botThreshold)
		{
			grad = (filter->virtualCentreValue*CENTRE_DETENT_MULT_FACTOR) / botThreshold;
			newVal = (grad*value) / (CENTRE_DETENT_MULT_FACTOR);
		}
		else
		{
			grad = ((0xFF - filter->virtualCentreValue) * CENTRE_DETENT_MULT_FACTOR) / (0xFF - topThreshold);
			offset = (topThreshold * grad) - (filter->virtualCentreValue* CENTRE_DETENT_MULT_FACTOR);
			newVal = ((grad*value) - offset)  / (CENTRE_DETENT_MULT_FACTOR);
		}
	}
	return newVal;
}

//
MIDIMsg_t savedEvents[2];

ADC_CentreDetent_t PitchBendDetent = {
		.centre = 127,
		.threshold = 3,
		.virtualCentreValue = 0x40<<1,
};


void Global_HandleADC(ADCEvent_t* adcEvent)
{
	MIDIMsg_t msg;

	msg.port = 0x00;

	if( adcEvent->index == ADC_PITCHBEND )
	{
		uint8_t value;
		value = Global_ApplyCentreDetent(&PitchBendDetent, adcEvent->value);

		msg.status = MIDI_PITCH_CHANGE | AppGlobal_GetMIDIChannel();
		msg.data1 = (value & 0x01) ? 0x40 : 0;
		msg.data2 = value >> 1;
		//ensure only changed events are sent out.
		if( memcmp( &msg, &savedEvents[0], sizeof(MIDIMsg_t)) != 0 )
		{
			HAL_MIDI_TxMsg(&msg);
			savedEvents[0] = msg;
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

