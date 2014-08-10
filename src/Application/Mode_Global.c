//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs

#include "ModeManager.h"
#include "Mode_Global.h"

#include "KeyboardEvents.h"
#include "SwitchEvents.h"
#include "ADCEvents.h"
#include "HAL_MIDI.h"

#include "App_Keyboard.h"
#include "DisplayManager.h"


static uint8_t handle_SWInput(MM_Input_t* input)
{
	SwitchEvent_t* swEvent = input->input.sw;

	GlobEvents_ProcessButton(swEvent->index, swEvent->value);
	DispMan_Print7Seg(swEvent->index, 0);
	return MM_INPUT_WAS_PROCESSED;
}

static uint8_t handle_ADCInput(MM_Input_t* input)
{
	ADCEvent_t* adcEvent = input->input.adc;
	DispMan_Print7Seg(adcEvent->value, 0);
	return MM_INPUT_WAS_PROCESSED;
}

static uint8_t handle_KeyBoardInput(MM_Input_t* input)
{

	KeyboardEvent_t* kbEvent = input->input.kb;
	App_HandleKeyEvent(kbEvent);
	return MM_INPUT_WAS_PROCESSED;
}

static uint8_t handle_MIDIInput(MM_Input_t* input)
{
	MIDIMsg_t* msg = input->input.midi;
	App_MIDIEvent(msg);
	return MM_INPUT_WAS_PROCESSED;
}

uint8_t Global_Mode(MM_Input_t* input)
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

//MM_States_t AppStates[] =
//{
//		{MM_MODIFY_OCTAVE, {eSW_INPUT, } },
//		{},
//};

