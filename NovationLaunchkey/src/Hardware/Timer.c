

#include "Timer.h"
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "LED.h"
#include "ADC.h"


typedef enum
{
	ADC_HOLD_TIMER = 0,
	TEST_TIMER,
	TIMER0_COUNT

} TIMER0_t;

volatile uint16_t ADC_NewColumnTimer = 0;
void ADC_SetNewColumnTimer(uint16_t time)
{
	ADC_NewColumnTimer = time;
}

uint32_t TIM_GetDeltaTime(uint32_t currentTime, uint32_t lastTime)
{
	uint32_t deltaTime;
	if( lastTime > currentTime )
	{
		deltaTime = (ULONG_MAX - (lastTime-currentTime)) + 1;
	}
	else
	{
		deltaTime = currentTime - lastTime;
	}	
	return deltaTime;
}

//1ms Timer;
void TMR0_Callback(uint32_t u32Param)
{
	static uint32_t timestamps[TIMER0_COUNT];
	uint32_t currentTime = DrvTIMER_GetIntTicks(E_TMR0);

	if( TIM_GetDeltaTime(currentTime, timestamps[ADC_HOLD_TIMER]) > ADC_NewColumnTimer)
	{
		if( ADC_IsFinishedSample() )
		{
			ADC_NewColumnTimer = 0xFFFF;
			ADC_StartConversion();
		}
		timestamps[ADC_HOLD_TIMER] = currentTime;
	}

}

void Timer_Init(void)
{
	DrvTIMER_Init();
	/* Set all the default TIMER clock sources are from 12MHz */
	DrvSYS_SelectIPClockSource(E_SYS_TMR0_CLKSRC, 0);
	
	/* Using TIMER0 in PERIODIC_MODE, 1 ms resolution */
	DrvTIMER_Open(E_TMR0, 1000, E_PERIODIC_MODE);

	/* Install callback "TMR0_Callback" and trigger callback when Interrupt happen twice */
	DrvTIMER_SetTimerEvent(E_TMR0, 1, (TIMER_CALLBACK)TMR0_Callback, 0);

	/* Enable TIMER0 Intettupt */
	DrvTIMER_EnableInt(E_TMR0);
	DrvTIMER_Start(E_TMR0);
}








