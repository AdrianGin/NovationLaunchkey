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
#include "Keyboard.h"


#include "EventManager.h"
#include "DisplayManager.h"

#include "ADCEvents.h"
#include "SwitchEvents.h"
#include "Global_ADC.h"

#include "App_Data.h"

/* These are the critical timers, 500kHz resolution */
volatile SoftTimer_16  SoftTimer1[TIMER1_COUNT] = { {1, 0, 0, Callback_CriticalTimers},};

volatile SoftTimer_16  SoftTimer2[TIMER2_COUNT] = { {25		,0	, 1, Callback_ADC_Handle},
													{50		,0	, 1, Callback_UpdateADC},
													{50		,0	, 1, Callback_UpdateDisplay},
													{1200	,0	, 0, Callback_PitchBendDebounce},  //SC_PITCHBEND_DEBOUNCE}
													{100	,0	, 1, Callback_SwitchRead}, //SC_SWITCH_READ
												};


volatile SoftTimer_16 SoftTimer3[TIMER3_COUNT] = {{2, 0, 1, Callback_ColumnMux},
												  {1, 0, 0, Callback_Switch_Read}};


void Callback_PitchBendDebounce(void)
{
	CentreDetent_SetDebounceState(&PitchBendDetent, CD_DEBOUNCE_DISABLED);
}


void Callback_SwitchRead(void)
{
	static uint32_t oldswitchState;
	uint8_t i;
	
	if( oldswitchState != Switch_GetSwitchStates() )
	{
		uint32_t switchChangeMap;
		switchChangeMap = oldswitchState ^ Switch_GetSwitchStates();
		oldswitchState = Switch_GetSwitchStates();
	}


	//Add debounce counters.
	for( i = 0 ; i < SW_COUNT; i++ )
	{
		uint8_t singleSwState = oldswitchState & (1<<i) ? SWITCH_ON : SWITCH_OFF;
		uint16_t switchCount;
		if( singleSwState )
		{
			switchCount = Switch_AddCount(i, SWITCH_ON);
			singleSwState = SWITCH_ON;
		}
		else
		{
			switchCount = Switch_AddCount(i, SWITCH_OFF);
			singleSwState = SWITCH_OFF;
		}

		if( switchCount == SWITCH_CHANGE_THRESHOLD )
		{

			uint8_t oppositeState = (singleSwState == SWITCH_OFF) ? SWITCH_ON : SWITCH_OFF;
			Switch_ResetCount(i, oppositeState);

			SwitchEvent_t event;
			event.index = i;
			event.value = singleSwState;
			GenericEvents_AddEvent( (VoidBuffer_t*)&SwitchMsgQueue, (void*)&event);
		}
	}

}

void Callback_UpdateADC(void)
{
	uint16_t adcSample;
	uint8_t i;
	for(i = ADC_PAD_0; i <= ADC_MODULATION; i++)
	{
		if( ADC_GetChangeFlag(i) )
		{
			adcSample = ADC_GetSample(i);
			ADCEvent_t event;
			event.index = i;
			event.value = adcSample;
			GenericEvents_AddEvent( (VoidBuffer_t*)&ADCMsgQueue, (void*)&event);
			ADC_ClearChangeFlag(i);
		}
	}
}


void Callback_UpdateDisplay(void)
{
	DispMan_Poll();
}

void Callback_ADC_Handle(void)
{
	if( ADC_IsFinishedSampling() )
	{
		ADC_StartConversion();
	}
}

#define LED_TIME_NEW_COL	(1)
#define LED_TIME_SHIFT		(4)
#define LED_TIME_BLANK		(1)

inline void Callback_CriticalTimers(void)
{
	//Run our nested Timers;
	RunAndExecuteTimers( (SoftTimer_16*)SoftTimer3, TIMER3_COUNT);

	//Callback_LED_Strobe();
}

volatile uint8_t LEDState = LED_STATE_SHIFT_DATA;

void Callback_ColumnMux(void)
{
	static uint8_t column = 0;

	switch( LEDState )
	{
		case LED_STATE_SHIFT_DATA:

			MUX_ActivateLineColumn(column);
			column++;
			if( column >= MAX_LINE_COLUMNS )
			{
				column = 0;
			}
			LED_TimerRoutine( MUX_GetCurrentColumn() );
			LEDState = LED_STATE_BLANK;
			SoftTimer3[SC_LED].timerCounter = LED_TIME_SHIFT;
			SoftTimer3[SC_LED].timeCompare  = LED_TIME_SHIFT;
			break;

		case LED_STATE_BLANK:
			//Do the switch reading when it's blank
			Callback_Switch_Read();
			LEDState = LED_STATE_SHIFT_DATA;
			SoftTimer3[SC_LED].timerCounter = LED_TIME_BLANK;
			SoftTimer3[SC_LED].timeCompare  = LED_TIME_BLANK;
			LED_Blank();
		break;

		default:
			break;
	}



}


void Callback_LED_StrobeCount(void)
{
	static uint8_t column = 0;
	
	static uint16_t count;


	if( LEDState == LED_STATE_BLANK )
	{
		//LED_TimerRoutine( MUX_GetCurrentColumn() );
		if( count > MAX_LED_BRIGHTNESS )
		{
			count = 0;
			Callback_Switch_Read();
			LED_Blank();

			column++;
			if( column >= MAX_LINE_COLUMNS )
			{
				column = 0;
			}
			MUX_ActivateLineColumn(column);
		}
		else
		{
			LED_CountUpRoutine(column, count++);
		}

		//LEDState = LED_STATE_ON;
		//SoftTimer3[SC_LED].timerCounter = LED_TIME_ON;
		//SoftTimer3[SC_LED].timeCompare  = LED_TIME_ON;
	}
	else
	{
		LEDState = LED_STATE_BLANK;
		//SoftTimer3[SC_LED].timerCounter = LED_TIME_OFF;
		//SoftTimer3[SC_LED].timeCompare  = LED_TIME_OFF;
		LED_Blank();
	}

}


inline void Callback_Switch_Read(void)
{
	//We can only sample when the LEDs are off.
	//if( LEDState == LED_STATE_BLANK )
	{
		Switch_ProcessState( Switch_ReadState() );

	}
}







