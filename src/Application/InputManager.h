#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void IM_InitCallBacks(void);

void IM_SetButtonState(uint8_t index);
void IM_ClearButtonState(uint8_t index);
uint8_t IM_GetButtonState(uint8_t index);



#ifdef __cplusplus
}
#endif

#endif
