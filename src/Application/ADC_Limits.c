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

#include "ADC_Limits.h"

#define ADCLIMIT_MULT_FACTOR (0x100)
uint8_t ADCLimits_ApplyRescale(ADC_Limits_t* filter, uint8_t value)
{
	if( value >= filter->adcMax )
	{
		return filter->scaledMax;
	}

	if( value <= filter->adcMin )
	{
		return filter->scaledMin;
	}

	uint16_t grad;
	int16_t offset;
	int16_t newVal;

	uint8_t dy = filter->scaledMax - filter->scaledMin;
	uint8_t dx = filter->adcMax - filter->adcMin;
	if( dx == 0 )
	{
		return value;
	}

	grad = (dy * ADCLIMIT_MULT_FACTOR) / dx;
	offset = (filter->scaledMax * ADCLIMIT_MULT_FACTOR) - (filter->adcMax * grad);
	newVal = ((grad * value) + offset) / (CENTRE_DETENT_MULT_FACTOR);


	return newVal;
}












