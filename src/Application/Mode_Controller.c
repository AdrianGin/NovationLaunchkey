//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs

#include "ModeManager.h"
#include "Mode_Controller.h"

#include "ControlSurfaceMap.h"
#include "App_Data.h"
#include "HAL_MIDI.h"

#include "App_MIDI.h"
#include "DisplayManager.h"

#include "HAL_Switch.h"

static uint8_t handle_SWInput(MM_Input_t* input)
{
	uint8_t index = input->input.sw->index;
	//Select which profile to use.
	if( (index >= SW_MUTE_0)  && (index <= SW_MUTE_8) )
	{
		CurrentADCMap = (ControlSurfaceMap_t**)&LoadedADCMap[0];
		DispMan_Print7Seg( (index-SW_MUTE_0) + 1, 0);
	}
	return 0;
}

static uint8_t handle_ADCInput(MM_Input_t* input)
{
	MIDIMsg_t msg;
	uint8_t valueToShow;

	valueToShow = ControlMap_TransformADCInput( CurrentADCMap, input->input.adc, &msg);

	if( valueToShow )
	{
		AppMIDI_ADCOutputMIDI(&msg, input->input.adc->index);

		if( valueToShow == 2 )
		{
			DispMan_Print7Seg(msg.data2, 0);
		}
		else
		{
			DispMan_Print7Seg(msg.data1, 0);
		}
	}

	return 0;
}

static uint8_t handle_KeyBoardInput(MM_Input_t* input)
{

	return 0;
}

uint8_t ModeController_HandleInput(MM_Input_t* input)
{
	uint8_t ret;

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

		default:
		{
			break;
		}

	}

	return ret;
}

