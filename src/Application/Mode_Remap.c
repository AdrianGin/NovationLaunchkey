//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs
#include "Mode_Remap.h"
#include "ModeManager.h"
#include "HAL_Switch.h"
#include "DisplayManager.h"

#include <stdlib.h>


typedef enum
{
	MR_SETMIN = 0,
	MR_SETMAX,
	MR_SETTYPE,
	MR_SETVALUE,
	MR_SETCHAN,

} MM_REMAP_STATES;

const uint8_t SetMin_String[] = "Lo";
const uint8_t SetMax_String[] = "Hi";
const uint8_t SetType_String[] = "Typ";

const uint8_t SetVal_String[] = "val";
const uint8_t SetChan_String[] = "Ch";

const uint8_t* const RemapModeStrings[] = {SetMin_String, SetMax_String, SetType_String, SetVal_String, SetChan_String};



static uint8_t handle_SWInput(MM_Input_t* input)
{
	SwitchEvent_t* swEvent = input->input.sw;
	uint8_t inputProcessed = !MM_INPUT_WAS_PROCESSED;
	//Only process on key release.
	if (swEvent->value == SWITCH_ON )
	{
		return !MM_INPUT_WAS_PROCESSED;
	}
	//GlobEvents_ProcessButton(, swEvent->value);

	if( swEvent->value == SWITCH_OFF )
	{
		switch (swEvent->index)
		{
			case SW_REWIND:
				DispMan_Print7SegAlpha( (uint8_t*)RemapModeStrings[MR_SETMIN] , 0);
				inputProcessed = MM_INPUT_WAS_PROCESSED;
				break;

			case SW_FASTFWD:
				DispMan_Print7SegAlpha( (uint8_t*)RemapModeStrings[MR_SETMAX] , 0);
				inputProcessed = MM_INPUT_WAS_PROCESSED;
				break;

			default:
				inputProcessed = !MM_INPUT_WAS_PROCESSED;
				break;
		}
	}


	return inputProcessed;
}

static uint8_t handle_ADCInput(MM_Input_t* input)
{
	ADCEvent_t* adcEvent = input->input.adc;

	//DispMan_Print7Seg(adcEvent->value, 0);
	return !MM_INPUT_WAS_PROCESSED;
}

static uint8_t handle_KeyBoardInput(MM_Input_t* input)
{
	return !MM_INPUT_WAS_PROCESSED;
}

static uint8_t handle_MIDIInput(MM_Input_t* input)
{
	return !MM_INPUT_WAS_PROCESSED;
}

uint8_t ModeRemap_HandleInput(MM_Input_t* input)
{
	uint8_t ret = 0;

	switch (input->class)
	{
		case eSW_INPUT:
		{
			ret = handle_SWInput(input);
			break;
		}

		case eADC_INPUT:
		{
			ret = handle_ADCInput(input);
			break;
		}

		case eKEYBOARD_INPUT:
		{
			ret = handle_KeyBoardInput(input);
			break;
		}

		case eMIDI_INPUT:
		{
			ret = handle_MIDIInput(input);
			break;
		}

		default:
		{
			break;
		}

	}
	return ret;
}

