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


#include "TimerCallbacks.h"
#include "Timer.h"

#include "LED.h"
#include "ADC.h"
#include "MultiplexControl.h"

/* These are the critical timers, 500kHz resolution */
volatile SoftTimer_16  SoftTimer1[TIMER1_COUNT] = { {16, 0, 0, Callback_LED_Strobe},};   

volatile SoftTimer_16  SoftTimer2[TIMER2_COUNT] = { {100,0, 0, Callback_ADC_Handle},
																	 {600,0, 0, Callback_UpdateDisplay}, };




void Callback_UpdateDisplay(void)
{
	uint16_t adcSample;

	uint8_t i;

	for(i = 0; i <= ADC_MODULATION; i++)
	{
		if( ADC_GetChangeFlag(i) )
		{
			adcSample = ADC_GetSample(i);
			LED_7Segment_WriteNumber(adcSample);
			ADC_ClearChangeFlag(i);


			printNumber(adcSample);
		}
	}


}

void Callback_ADC_Handle(void)
{
	if( ADC_IsFinishedSampling() )
	{
		ADC_StartConversion();
	}
}


#define LED_TIME_ON	(10)
#define LED_TIME_OFF	(1)

void Callback_LED_Strobe(void)
{
	static uint8_t column;
	static uint8_t ledState = LED_STATE_BLANK;

	if( ledState == LED_STATE_BLANK )
	{
		MUX_ActivateLineColumn(column);
		LED_TimerRoutine(column);
		column++;
		if( column >= MAX_LINE_COLUMNS )
		{
			column = 0;
		}

		ledState = LED_STATE_ON;
		SoftTimer1[SC_LED_ON].timerCounter = LED_TIME_ON;
		SoftTimer1[SC_LED_ON].timeCompare  = LED_TIME_ON;
	}
	else
	{
		ledState = LED_STATE_BLANK;
		SoftTimer1[SC_LED_ON].timerCounter = LED_TIME_OFF;
		SoftTimer1[SC_LED_ON].timeCompare  = LED_TIME_OFF;
		LED_Blank();
	}

}












