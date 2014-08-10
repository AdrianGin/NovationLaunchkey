#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "EventManager.h"

#define MM_EXIT_MODE (0xFF)
#define MM_INPUT_WAS_PROCESSED (0x01)

typedef enum
{
	MM_MAIN = 0,
	MM_MODIFY_OCTAVE,
	MM_MODIFY_TRANSPOSE,
	MM_STATE_COUNT,
} MM_STATES;




typedef struct
{
	uint8_t class;
	union {
		SwitchEvent_t* sw;
		KeyboardEvent_t* kb;
		ADCEvent_t* adc;
		MIDIMsg_t*  midi;
	} input;
} MM_Input_t;


typedef struct
{
	uint8_t assignedState;
	uint8_t (*action)(MM_Input_t* );
} MM_States_t;

typedef struct
{
	uint8_t currentState;
	uint8_t nextInput;
	MM_States_t** stateMap;

} StateMachine_t;


extern StateMachine_t CurrentMode;

uint8_t MM_ApplyInput(StateMachine_t* mode, MM_Input_t* input);



#ifdef __cplusplus
}
#endif

#endif
