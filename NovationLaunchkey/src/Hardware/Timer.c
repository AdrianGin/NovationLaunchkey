

#include "Timer.h"
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "LED.h"
#include "ADC.h"

#define TMR_ADC_SAMPLE_TIME (1)

typedef enum
{
	TMR_ADC_NEWCOLUMN = 0,
	TMR_ADC_SAMPLING,

} TMR_ADC_STATES;

//1ms Timer;
void TMR0_Callback(uint32_t u32Param)
{
	static uint32_t lastTickTime = 0;

	static uint8_t adcState;

	uint32_t deltaTime;
	uint32_t currentTime = DrvTIMER_GetIntTicks(E_TMR0);

	if( lastTickTime > currentTime )
	{
		deltaTime = (ULONG_MAX - (lastTickTime-currentTime)) + 1;
	}
	else
	{
		deltaTime = lastTickTime - currentTime;
	}


	if( (deltaTime) > TMR_ADC_SAMPLE_TIME)
	{
		if(adcState == TMR_ADC_SAMPLING)
		{
			ADC_SetNextColumn();
			adcState = TMR_ADC_NEWCOLUMN;
		}
		else if(adcState == TMR_ADC_NEWCOLUMN)
		{
			if( ADC_IsFinishedSample() )
			{
				ADC_StartConversion();
				adcState = TMR_ADC_SAMPLING;
			}
		}



	}

	lastTickTime = currentTime;

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








