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

#include "DisplayManager.h"
#include "uint16toa.h"
#include <stdlib.h>

#include <string.h>

typedef struct
{
	uint8_t  type; //for char or digit or both
	uint8_t  ch[DISPLAY_COUNT];
	int16_t number;
	uint8_t counter;
	uint8_t rate;
	uint8_t state;

} Flash_7Seg;

Flash_7Seg DispMan_7SegDisplay;

enum
{
	DISP_NUMBER,
	DISP_ALPHANUMERIC,
};

//Flash rate of 0 means no FLASH.
void DispMan_Print7Seg(uint16_t number, uint8_t flashRate)
{
	DispMan_7SegDisplay.rate = flashRate;
	DispMan_7SegDisplay.number = number;
	DispMan_7SegDisplay.type = DISP_NUMBER;
	DispMan_7SegDisplay.state = DISPLAY_MODE_NORMAL;
}

//Flash rate of 0 means no FLASH.
//up to +/- 99
void DispMan_Print7SegInt(int8_t number, uint8_t flashRate)
{
	uint8_t outputStr[4];

	DispMan_7SegDisplay.rate = flashRate;
	DispMan_7SegDisplay.number = 0;
	DispMan_7SegDisplay.type = DISP_ALPHANUMERIC;
	DispMan_7SegDisplay.state = DISPLAY_MODE_NORMAL;

	outputStr[0] = ' ';
	if (number < 0)
	{
		outputStr[0] = '-';
	}

	number = abs(number);
	if (number < 10)
	{
		uint16toa(number, (char*) &outputStr[2], 0);
	}
	else
	{
		uint16toa(number, (char*) &outputStr[1], 0);
	}
	memcpy( &DispMan_7SegDisplay.ch, outputStr, DISPLAY_COUNT);

}

//Flash rate of 0 means no FLASH.
void DispMan_Print7SegAlpha(uint8_t* buf, uint8_t flashRate)
{
	memcpy( &DispMan_7SegDisplay.ch, buf, DISPLAY_COUNT);
	DispMan_7SegDisplay.type = DISP_ALPHANUMERIC;

	DispMan_7SegDisplay.rate = flashRate;
	DispMan_7SegDisplay.number = 0;
	DispMan_7SegDisplay.state = DISPLAY_MODE_NORMAL;
}

void DispMan_Poll(void)
{

	if( DispMan_7SegDisplay.rate )
	{

		if( DispMan_7SegDisplay.counter++ >= DispMan_7SegDisplay.rate )
		{
			DispMan_7SegDisplay.counter = 0;
			DispMan_7SegDisplay.state ^= 1;
		}


		if( DispMan_7SegDisplay.state )
		{
			LED_7Segment_Write(0, LED_7SEG_CLEAR);
			LED_7Segment_Write(1, LED_7SEG_CLEAR);
			LED_7Segment_Write(2, LED_7SEG_CLEAR);
		}
	}

	if( DispMan_7SegDisplay.state == DISPLAY_MODE_NORMAL )
	{
		if( DispMan_7SegDisplay.type == DISP_NUMBER )
		{
			LED_7Segment_WriteNumber(DispMan_7SegDisplay.number);
		}
		else
		{
			LED_7Segment_WriteChar(0, DispMan_7SegDisplay.ch[0]);
			LED_7Segment_WriteChar(1, DispMan_7SegDisplay.ch[1]);
			LED_7Segment_WriteChar(2, DispMan_7SegDisplay.ch[2]);
		}
	}

}








