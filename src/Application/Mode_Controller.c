//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs

#include "ModeManager.h"
#include "Mode_Controller.h"

#include "ControlSurfaceMap.h"
#include "App_Data.h"
#include "HAL_MIDI.h"

#include "App_MIDI.h"
#include "DisplayManager.h"

static uint8_t handle_SWInput(MM_Input_t* input)
{

	return 0;
}

static uint8_t handle_ADCInput(MM_Input_t* input)
{
	MIDIMsg_t msg;

	if( ControlMap_TransformADCInput( (const ControlSurfaceMap_t** const)&ADCMap[0], input->input.adc, &msg) )
	{
		AppMIDI_ADCOutputMIDI(&msg, input->input.adc->index);
		DispMan_Print7Seg(msg.data2, 0);
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

