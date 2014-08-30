//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs
#include <stdint.h>

#include "ModeManager.h"

#include "Mode_OctaveTranspose.h"
#include "Mode_Global.h"



void MM_SetMode(StateMachine_t* sm, uint8_t newMode)
{
	if( newMode < sm->stateCount )
	{
		sm->currentState = newMode;
	}
}

uint8_t MM_NextMode(StateMachine_t* sm)
{
	uint8_t nextMode = MM_GetMode(sm) + 1;
	if( nextMode >= sm->stateCount )
	{
		nextMode = 0;
	}
	MM_SetMode(sm, nextMode);
	return nextMode;
}

uint8_t MM_PrevMode(StateMachine_t* sm)
{
	uint8_t nextMode = MM_GetMode(sm);
	if( nextMode == 0 )
	{
		nextMode = sm->stateCount;
	}
	nextMode = nextMode - 1;
	MM_SetMode(sm, nextMode);
	return nextMode;
}

uint8_t MM_GetMode(StateMachine_t* sm)
{
	return sm->currentState;
}


uint8_t MM_ApplyInput(StateMachine_t* sm, MM_Input_t* input)
{
	//Process Global events first
	if (Global_Mode(input) == MM_INPUT_WAS_PROCESSED)
	{

	}
	else
	{

	}

	return 0;
}

