#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "EventManager.h"

#define MM_EXIT_MODE (0xFF)
#define MM_INPUT_WAS_PROCESSED (0x01)



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
	uint8_t stateCount;
	MM_States_t** stateMap;

} StateMachine_t;


extern StateMachine_t GlobalStateMachine;

uint8_t MM_ApplyInput(StateMachine_t* mode, MM_Input_t* input);
void MM_SetMode(StateMachine_t* sm, uint8_t newMode);
uint8_t MM_GetMode(StateMachine_t* sm);
uint8_t MM_NextMode(StateMachine_t* sm);
uint8_t MM_PrevMode(StateMachine_t* sm);

#ifdef __cplusplus
}
#endif

#endif
