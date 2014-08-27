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

#include "Rescale.h"

#define RESACLE_MULT_FACTOR (0x10000)
uint16_t Rescale_Apply(Rescale_t* filter, uint16_t value)
{
	if( value >= filter->xMax )
	{
		return filter->yMax;
	}

	if( value <= filter->xMin )
	{
		return filter->yMin;
	}

	uint32_t grad;
	int32_t offset;
	uint16_t newVal;

	uint16_t dy = filter->yMax - filter->yMin;
	uint16_t dx = filter->xMax - filter->xMin;
	if( dx == 0 )
	{
		return value;
	}

	grad = (dy * RESACLE_MULT_FACTOR) / dx;
	offset = (filter->yMax * RESACLE_MULT_FACTOR) - (filter->xMax * grad);
	newVal = ((grad * value) + offset) / (RESACLE_MULT_FACTOR);


	return newVal;
}












