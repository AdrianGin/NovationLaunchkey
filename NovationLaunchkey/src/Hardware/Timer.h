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

void Timer_Init(void);
void TMR0_Callback(uint32_t u32Param);



//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
