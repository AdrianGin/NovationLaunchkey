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



#ifndef _CENTRE_DETENT_H
#define _CENTRE_DETENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

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

	//whether debounce is active
	uint8_t debounceIsActive;

} ADC_CentreDetent_t;


typedef enum {
	CD_DEBOUNCE_DISABLED = 0,
	CD_DEBOUNCE_ENABLED = 0,
} CD_Debounce_t;

enum {
	CD_NO_ZERO_CROSS = 0,
	CD_ZEROCROSS_INCREASING,
	CD_ZEROCROSS_DECREASING,
};

extern ADC_CentreDetent_t PitchBendDetent;


uint8_t CentreDetent_ApplyFilter(ADC_CentreDetent_t* filter, uint8_t value);
void CentreDetent_SetDebounceState(ADC_CentreDetent_t* cd, CD_Debounce_t newState);
uint8_t CentreDetent_Compare2Values(ADC_CentreDetent_t* cd, uint8_t oldValue, uint8_t newValue);


#ifdef __cplusplus
}
#endif

#endif
