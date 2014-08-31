#ifndef MODECONTROLLER_H
#define MODECONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ModeManager.h"


uint8_t ModeController_HandleInput(MM_Input_t* input);


#ifdef __cplusplus
}
#endif

#endif
