#ifndef MODE_GLOBAL_H
#define MODE_GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ModeManager.h"


typedef enum
{
	MM_CONTROLLER = 0,
	MM_REMAP,
	MM_SEQUENCER,
	MM_MODE_COUNT,
} MM_STATES;


typedef enum
{
	GLOBAL_MODIFY_OCTAVE,
	GLOBAL_MODIFY_TRANSPOSE,
} GLOBAL_SUBSTATES;



uint8_t Global_Mode(MM_Input_t* input);


#ifdef __cplusplus
}
#endif

#endif
