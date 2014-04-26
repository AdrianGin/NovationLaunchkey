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
volatile SoftTimer_16  SoftTimer1[TIMER1_COUNT] = { {1, 0, 0, Callback_ColumnMux},};

volatile SoftTimer_16  SoftTimer2[TIMER2_COUNT] = { {200,0, 0, Callback_ADC_Handle},
													{600,0, 0, Callback_UpdateDisplay}, };


#define TIMER3_MAX_TIME (10)

volatile SoftTimer_16 SoftTimer3[TIMER3_COUNT] = {{1, 0, 1, Callback_LED_Strobe},
												  {10, 0, 1, Callback_Switch_Read}};


volatile static uint8_t CanChangeColumn = FALSE;


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
			adcSample = ADC_GetRawSample(30);
			printNumber(adcSample);
		}
	}


}

void Callback_ADC_Handle(void)
{
	if( ADC_IsFinishedSampling() )
	{
		ADC_StartConversion();
		SoftTimerStop(SoftTimer2[SC_ADC]);
	}
}

#define LED_TIME_ON	(10)
#define LED_TIME_OFF	(1)

void Callback_ColumnMux(void)
{
	static uint8_t column = 0;


	//Run our nexted Timers;
	//MUX_ActivateLineColumn(column);
	RunAndExecuteTimers( (SoftTimer_16*)SoftTimer3, TIMER3_COUNT);

	if( CanChangeColumn )
	{
		CanChangeColumn = FALSE;

		column++;
		if( column >= MAX_LINE_COLUMNS )
		{
			column = 0;
		}
	}
}

uint8_t TIM_IsColumnChangeReady(uint8_t count)
{
	if( count == TIMER3_MAX_TIME )
	{
		return 1;
	}
	return 0;
}




void Callback_LED_Strobe(void)
{
	static uint8_t column = 0;
	
	static uint8_t ledState = LED_STATE_BLANK;

	if( ledState == LED_STATE_BLANK )
	{
		MUX_ActivateLineColumn(column);
		LED_TimerRoutine( MUX_GetCurrentColumn() );

		column++;
		if( column >= MAX_LINE_COLUMNS )
		{
			column = 0;
		}

		ledState = LED_STATE_ON;
		SoftTimer3[SC_LED].timerCounter = LED_TIME_ON;
		SoftTimer3[SC_LED].timeCompare  = LED_TIME_ON;
	}
	else
	{
		ledState = LED_STATE_BLANK;
		SoftTimer3[SC_LED].timerCounter = LED_TIME_OFF;
		SoftTimer3[SC_LED].timeCompare  = LED_TIME_OFF;
		LED_Blank();

		CanChangeColumn = TRUE;

	}

}


void Callback_Switch_Read(void)
{


}










