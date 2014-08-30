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

#include "ADC_CentreDetent.h"
#include "TimerCallbacks.h"

ADC_CentreDetent_t PitchBendDetent = {
		.centre = 125,
		.threshold = 1,
		.virtualCentreValue = 0x40 << 1,
		.debounceIsActive = 0,
};

const uint16_t CENTRE_DETENT_MULT_FACTOR = 0x100;
const uint16_t MAX_RANGE = 0x100;
//Returns the conditioned value after applying a centre detent.
//Resolution / Dynamic range is not lost, but certain values may be skipped.
uint8_t CentreDetent_ApplyFilter(ADC_CentreDetent_t* filter, uint8_t value)
{
	uint16_t topThreshold = filter->centre + filter->threshold;
	uint16_t botThreshold = filter->centre - filter->threshold;

	uint16_t grad;
	int16_t offset;
	uint16_t newVal;

	if ((value <= topThreshold) && (value >= botThreshold))
	{
		return filter->virtualCentreValue;
	}
	else
	{
		if (value < botThreshold)
		{
			grad = (filter->virtualCentreValue * CENTRE_DETENT_MULT_FACTOR) / botThreshold;
			newVal = (grad * value) / (CENTRE_DETENT_MULT_FACTOR);
		}
		else
		{
			grad = ((MAX_RANGE - filter->virtualCentreValue) * CENTRE_DETENT_MULT_FACTOR) / (MAX_RANGE - topThreshold);
			offset = (topThreshold * grad) - (filter->virtualCentreValue * CENTRE_DETENT_MULT_FACTOR);
			newVal = (((grad * value) - offset) / (CENTRE_DETENT_MULT_FACTOR)) + 1;
		}
	}
	return newVal;
}

uint8_t CentreDetent_Compare2Values(ADC_CentreDetent_t* cd, uint8_t oldValue, uint8_t newValue)
{
	uint8_t ret = CD_NO_ZERO_CROSS;
	if ((oldValue < cd->virtualCentreValue) && (newValue >= cd->virtualCentreValue))
	{
		ret = CD_ZEROCROSS_INCREASING;
	}

	if ((newValue <= cd->virtualCentreValue) && (oldValue > cd->virtualCentreValue))
	{
		ret = CD_ZEROCROSS_DECREASING;
	}
	return ret;
}

void CentreDetent_SetDebounceState(ADC_CentreDetent_t* cd, uint8_t newState)
{
	cd->debounceIsActive = newState;
	if (newState == CD_DEBOUNCE_ENABLED)
	{
		SoftTimerStart(SoftTimer2[SC_PITCHBEND_DEBOUNCE]);
	}
	else
	{
		SoftTimerStop(SoftTimer2[SC_PITCHBEND_DEBOUNCE]);
	}
}

