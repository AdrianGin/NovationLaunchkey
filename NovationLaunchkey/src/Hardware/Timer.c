

#include "Timer.h"
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "LED.h"
#include "ADC.h"

#include "Softtimer.h"
#include "TimerCallbacks.h"




//500kHz Timer;
void TMR0_Callback(uint32_t u32Param)
{
	RunCriticalTimer();
}

void Timer_Init(void)
{
	DrvTIMER_Init();
	/* Set all the default TIMER clock sources are from 12MHz */
	DrvSYS_SelectIPClockSource(E_SYS_TMR0_CLKSRC, 0x02);
	
	/* Using TIMER0 in PERIODIC_MODE, 100kHz resolution */
	DrvTIMER_Open(E_TMR0, 100000, E_PERIODIC_MODE);

	/* Install callback "TMR0_Callback" and trigger callback when Interrupt happen twice */
	DrvTIMER_SetTimerEvent(E_TMR0, 1, (TIMER_CALLBACK)TMR0_Callback, 0);

	/* Enable TIMER0 Intettupt */
	DrvTIMER_EnableInt(E_TMR0);
	DrvTIMER_Start(E_TMR0);
}








