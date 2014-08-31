//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs
#include "Mode_Remap.h"
#include "ModeManager.h"
#include "HAL_Switch.h"
#include "DisplayManager.h"

#include "App_Data.h"

#include <stdlib.h>


typedef enum
{

	MR_SETMIN = 0,
	MR_SETMAX,
	MR_SETTYPE,
	MR_SETVALUE,
	MR_SETCHAN,
	MR_INVALID,

} MM_REMAP_STATES;

const uint8_t SetMin_String[] = "Lo";
const uint8_t SetMax_String[] = "Hi";
const uint8_t SetType_String[] = "Typ";
const uint8_t SetVal_String[] = "val";
const uint8_t SetChan_String[] = "Ch";

const uint8_t* const RemapModeStrings[] = {
	SetMin_String,
	SetMax_String,
	SetType_String,
	SetVal_String,
	SetChan_String};

static uint8_t RemapMode_State = MR_INVALID;

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

		uint8_t swIndex = swEvent->index;
		if( (swIndex >= SW_REWIND) && (swIndex <= SW_REC) )
		{
			MM_REMAP_STATES newMode = swIndex - SW_REWIND;

			DispMan_Print7SegAlpha( (uint8_t*)RemapModeStrings[newMode] , 0);
			RemapMode_State = newMode;
			inputProcessed = MM_INPUT_WAS_PROCESSED;
		}
	}


	return inputProcessed;
}

static uint8_t handle_ADCInput(MM_Input_t* input)
{
	ADCEvent_t* adcEvent = input->input.adc;
	uint8_t index = adcEvent->index;
	eCM_Parameters param;

	switch( RemapMode_State )
	{
		case MR_SETMIN:
		{
			param = CM_MIN;
			break;
		}

		case MR_SETMAX:
		{
			param = CM_MAX;
			break;
		}

		case MR_SETTYPE:
		{
			param = CM_STATUSBYTE;
			break;
		}


		case MR_SETVALUE:
		{
			param = CM_CONTROLVAL;
			break;
		}


		case MR_SETCHAN:
		{
			param = CM_CHANNEL;
			break;
		}

		default:
		{
			break;
		}
	}

	if( (index >= ADC_KNOB_0)  && (index <= ADC_SLIDER_7) )
	{
		uint8_t newVal;
		newVal = ControlMap_EditADCParameter( (ControlSurfaceMap_t**)&LoadedADCMap[0], param, adcEvent);
		DispMan_Print7Seg(newVal, 0);
	}


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

