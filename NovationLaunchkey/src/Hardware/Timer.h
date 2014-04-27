#ifndef HW_TIMER_H
#define HW_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif


#include "DrvSys.h"
#include "DrvTIMER.h"
#include "DrvGPIO.h"
#include "HardwareSpecific.h"
#include <stdint.h>



typedef enum
{
	ADC_HOLD_TIMER = 0,
	LED_STROBE,
	TEST_TIMER,
	TIMER0_COUNT

} TIMER0_t;







uint8_t TIM_IsMasterTickTriggered(void);
void TIM_ResetMasterTick(void);

void Timer_Init(uint32_t kHzSpeed);
void TMR0_Callback(uint32_t u32Param);
uint32_t TIM_GetDeltaTime(uint32_t currentTime, uint32_t lastTime);


//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
