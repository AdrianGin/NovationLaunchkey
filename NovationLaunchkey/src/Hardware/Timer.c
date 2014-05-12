

#include "Timer.h"
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "LED.h"
#include "ADC.h"

#include "Keyboard.h"
#include "Softtimer.h"
#include "TimerCallbacks.h"

volatile uint8_t TIM_MasterTick = 0;

inline uint8_t TIM_IsMasterTickTriggered(void)
{
	return TIM_MasterTick;
}

inline void TIM_ResetMasterTick(void)
{
	TIM_MasterTick = 0;
}


//500kHz Timer;
void TMR0_Callback(uint32_t u32Param)
{

}


void Timer_InitNoBSP(uint8_t prescale, uint32_t compare)
{
	//Select 12MHz
	DrvSYS_SetIPClock(E_SYS_TMR0_CLK, 1);
	DrvSYS_SelectIPClockSource(E_SYS_TMR0_CLKSRC, 0x00);

	TIMER0->TCSR.MODE = E_PERIODIC_MODE;
	TIMER0->TCSR.TDR_EN = 1;


	TIMER0->TCSR.PRESCALE = prescale;
	TIMER0->TCMPR = compare;

	TIMER0->TCSR.CEN = 1;
	TIMER0->TCSR.IE = 1;

	//Needs to be higher priority than the ADC.
    //NVIC_SetPriority((IRQn_Type)((uint32_t)TMR0_IRQn + (uint32_t)0), (1<<__NVIC_PRIO_BITS) - 3);
	NVIC_EnableIRQ((IRQn_Type)((uint32_t)TMR0_IRQn + (uint32_t)0));
}

void Timer_DisableNoBSP(void)
{
	TIMER0->TCSR.CEN = 0;
	TIMER0->TCSR.IE = 0;
}


void TMR0_IRQHandler(void)
{
    if ((TIMER0->TCSR.IE == 1) && (TIMER0->TISR.TIF == 1))
        TIMER0->TISR.TIF = 1;

	TIM_MasterTick = 1;
	Keyboard_IncrementTimer();
	//Keyboard_Timer++;
	RunAndExecuteTimers( (SoftTimer_16*)SoftTimer1, TIMER1_COUNT);
}

void Timer_Init(uint32_t kHzSpeed)
{
	DrvTIMER_Init();
	/* Set all the default TIMER clock sources are from 12MHz */
	DrvSYS_SelectIPClockSource(E_SYS_TMR0_CLKSRC, 0x02);
	
	/* Using TIMER0 in PERIODIC_MODE, 100kHz resolution */
	DrvTIMER_Open(E_TMR0, kHzSpeed, E_PERIODIC_MODE);

	/* Install callback "TMR0_Callback" and trigger callback when Interrupt happen twice */
	DrvTIMER_SetTimerEvent(E_TMR0, 1, (TIMER_CALLBACK)TMR0_Callback, 0);

	/* Enable TIMER0 Intettupt */
	DrvTIMER_EnableInt(E_TMR0);
	DrvTIMER_Start(E_TMR0);
}


void Timer_Disable(E_TIMER_CHANNEL ch)
{
	DrvTIMER_Close(ch);
	DrvTIMER_ClearTimerEvent(ch, 0);
}





