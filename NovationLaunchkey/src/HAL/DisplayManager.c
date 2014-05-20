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


typedef struct
{
	uint16_t number;
	uint8_t counter;
	uint8_t rate;
	uint8_t state;

} Flash_7Seg;

Flash_7Seg DispMan_7SegDisplay;

//Flash rate of 0 means no FLASH.
void DispMan_Print7Seg(uint16_t number, uint8_t flashRate)
{
	DispMan_7SegDisplay.rate = flashRate;
	LED_7Segment_WriteNumber(number);
	DispMan_7SegDisplay.number = number;
}

void DispMan_Poll(void)
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
	else
	{
		LED_7Segment_WriteNumber(DispMan_7SegDisplay.number);
	}

}








