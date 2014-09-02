#ifndef MODECONTROLLER_H
#define MODECONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ModeManager.h"


void ModeControl_OutputMessage(uint8_t index, uint8_t value);

uint8_t ModeController_HandleInput(MM_Input_t* input);


#ifdef __cplusplus
}
#endif

#endif
