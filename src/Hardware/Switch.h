
#ifndef _SWITCH_H
#define _SWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "HAL_Switch.h"
#include "HardwareSpecific.h"


#define SWITCHES_PER_COLUMN (4)

void Switch_GPIO_Init(void);
uint8_t Switch_GetState(uint8_t index);
void Switch_ProcessState(uint8_t switchStates);
uint32_t Switch_CheckForChange(void);
uint8_t Switch_ReadState(void);
uint32_t Switch_GetSwitchStates(void);

uint16_t Switch_AddCount(uint8_t index, uint8_t switchMode);
uint16_t Switch_ResetCount(uint8_t index, uint8_t switchMode);
uint16_t Switch_SubtractCount(uint8_t index);


#ifdef __cplusplus
}
#endif



#endif





