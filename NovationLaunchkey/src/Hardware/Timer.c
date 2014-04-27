

#include "Timer.h"
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "LED.h"
#include "ADC.h"

#include "Softtimer.h"
#include "TimerCallbacks.h"

volatile uint8_t TIM_MasterTick = 0;


uint8_t TIM_IsMasterTickTriggered(void)
{
	return TIM_MasterTick;
}

void TIM_ResetMasterTick(void)
{
	TIM_MasterTick = 0;
}

//500kHz Timer;
void TMR0_Callback(uint32_t u32Param)
{
	TIM_MasterTick = 1;
	RunAndExecuteTimers( (SoftTimer_16*)SoftTimer1, TIMER1_COUNT);
}

void Timer_Init(uint32_t kHzSpeed)
{
	DrvTIMER_Init();
	/* Set all the default TIMER clock sources are from 12MHz */
	DrvSYS_SelectIPClockSource(E_SYS_TMR0_CLKSRC, 0x02);
	
	/* Using TIMER0 in PERIODIC_MODE, 100kHz resolution */
	printNumber(DrvTIMER_Open(E_TMR0, kHzSpeed, E_PERIODIC_MODE));

	/* Install callback "TMR0_Callback" and trigger callback when Interrupt happen twice */
	DrvTIMER_SetTimerEvent(E_TMR0, 1, (TIMER_CALLBACK)TMR0_Callback, 0);

	/* Enable TIMER0 Intettupt */
	DrvTIMER_EnableInt(E_TMR0);
	DrvTIMER_Start(E_TMR0);
}








