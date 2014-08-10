//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs

#include "ModeManager.h"

#include "Mode_OctaveTranspose.h"
#include "Mode_Global.h"

const MM_States_t AppStates[] = { { MM_MODIFY_OCTAVE, &OctaveTranspose_Mode }, };

StateMachine_t CurrentMode = { .currentState = MM_MAIN, .stateMap = (MM_States_t**)&AppStates };



uint8_t MM_ApplyInput(StateMachine_t* mode, MM_Input_t* input)
{
	//Process Global events first
	if( Global_Mode(input) == MM_INPUT_WAS_PROCESSED )
	{


	}
	else
	{


	}

	return 0;

}

