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



#ifndef _TIMER_CALLBACKS_H
#define _TIMER_CALLBACKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Softtimer.h"

/* CCR1B associated timers */
typedef enum {  
	SC_COLUMN_MUX = 0,
	TIMER1_COUNT
   
} timer1Ids;

typedef enum {  
	SC_ADC,
	SC_UPDATE_ADC,
	SC_UPDATE_DISPLAY,
	SC_PITCHBEND_DEBOUNCE,
	SC_SWITCH_READ,
   TIMER2_COUNT 
   
} timer2Ids;


typedef enum {
	SC_LED = 0,
	//SC_KEYBOARD,
	SC_SWITCH,
   TIMER3_COUNT

} timer3Ids;

#define TMR_ADC_SAMPLE_TIME (20)

extern volatile SoftTimer_16  SoftTimer1[];
extern volatile SoftTimer_16  SoftTimer2[];

void Callback_CriticalTimers(void);
void Callback_UpdateADC(void);
void Callback_UpdateDisplay(void);
void Callback_ADC_Handle(void);
void Callback_ColumnMux(void);
void Callback_Switch_Read(void);
void Callback_PitchBendDebounce(void);
void Callback_SwitchRead(void);


#ifdef __cplusplus
}
#endif

#endif
