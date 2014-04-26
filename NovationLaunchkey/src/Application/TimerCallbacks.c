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
#include "Switch.h"

/* These are the critical timers, 500kHz resolution */
volatile SoftTimer_16  SoftTimer1[TIMER1_COUNT] = { {1, 0, 0, Callback_ColumnMux},};

volatile SoftTimer_16  SoftTimer2[TIMER2_COUNT] = { {4,0, 0, Callback_ADC_Handle},
																	 {100,0, 0, Callback_UpdateDisplay}, };


volatile SoftTimer_16 SoftTimer3[TIMER3_COUNT] = {{1, 0, 1, Callback_LED_Strobe},
																  {1, 0, 0, Callback_Switch_Read}};


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
			adcSample = ADC_GetRawSample(i);
			printNumber(adcSample);
		}
	}


	static uint32_t switchChanges;

	if( switchChanges != Switch_GetSwitchStates() )
	{
		uint8_t i;
		uint32_t switchChangeMap;
		switchChangeMap = switchChanges ^ Switch_GetSwitchStates();

		switchChanges = Switch_GetSwitchStates();

		for( i = 0 ; i < SW_COUNT; i++ )
		{
			if( switchChangeMap & (1<<i) )
			{
				uint8_t switchState;
				switchState = Switch_GetState(i);
				LED_7Segment_WriteNumber(i);
				printNumber(switchState);
			}
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

#define LED_TIME_ON	(5)
#define LED_TIME_OFF	(1)

void Callback_ColumnMux(void)
{
	//Run our nested Timers;
	RunAndExecuteTimers( (SoftTimer_16*)SoftTimer3, TIMER3_COUNT);
}

volatile uint8_t LEDState = LED_STATE_BLANK;


void Callback_LED_Strobe(void)
{
	static uint8_t column = 0;
	
	static uint16_t count;


	if( LEDState == LED_STATE_BLANK )
	{
		if( column != MUX_GetCurrentColumn() )
		{
			MUX_ActivateLineColumn(column);
		}
		//LED_TimerRoutine( MUX_GetCurrentColumn() );

		LED_CountUpRoutine(column, count++);

		if( count > MAX_LED_BRIGHTNESS )
		{
			count = 0;

			column++;
			if( column >= MAX_LINE_COLUMNS )
			{
				column = 0;
			}
		}

		LEDState = LED_STATE_ON;
		SoftTimer3[SC_LED].timerCounter = LED_TIME_ON;
		SoftTimer3[SC_LED].timeCompare  = LED_TIME_ON;
	}
	else
	{
		LEDState = LED_STATE_BLANK;
		SoftTimer3[SC_LED].timerCounter = LED_TIME_OFF;
		SoftTimer3[SC_LED].timeCompare  = LED_TIME_OFF;
		LED_Blank();

		Callback_Switch_Read();

	}

}


void Callback_Switch_Read(void)
{
	//We can only sample when the LEDs are off.
	if( LEDState == LED_STATE_BLANK )
	{
		Switch_ProcessState( Switch_ReadState() );
	}
}






