//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs

#include "ModeManager.h"
#include "Mode_Global.h"

#include "KeyboardEvents.h"
#include "SwitchEvents.h"
#include "ADCEvents.h"
#include "HAL_MIDI.h"
#include "HAL_ADC.h"

#include "App_Keyboard.h"
#include "App_GlobalSettings.h"

#include "Global_ADC.h"

#include "DisplayManager.h"

#include "HAL_Switch.h"
#include "InputManager.h"

#include "uint16toa.h"

#include <stdlib.h>

//Used to indicate the direction of increasing / decreasing variables
//Used to stop unintuitive behaviour when using transpose
enum {
	VARIABLE_INACTIVE = 0,
	VARIABLE_INCREASING,
	VARIABLE_DECREASING,
} eVariableDirectionFlag;


StateMachine_t GlobalStateMachine =
{
	.currentState = MM_CONTROLLER,
	//.stateMap = (MM_States_t**) &AppStates,
	.stateCount = MM_MODE_COUNT,
};

const uint8_t ControllerMode_String[] = "Con";
const uint8_t RemapMode_String[] = "Rem";
const uint8_t SequencerMode_String[] = "Seq";
const uint8_t* ModeStrings[] = {ControllerMode_String, RemapMode_String, SequencerMode_String};

static uint8_t Global_TransposeFlag = VARIABLE_INACTIVE;
static uint8_t Global_MIDIChannelFlag = VARIABLE_INACTIVE;

static uint8_t handle_SWInput(MM_Input_t* input)
{
	SwitchEvent_t* swEvent = input->input.sw;

	//Only process on key release.
	if (swEvent->value == 1)
	{
		return !MM_INPUT_WAS_PROCESSED;
	}
	//GlobEvents_ProcessButton(, swEvent->value);

	switch (swEvent->index)
	{
		case SW_TRACK_LEFT:
			if (IM_GetButtonState(SW_TRACK_RIGHT))
			{

				if( Global_MIDIChannelFlag == VARIABLE_INACTIVE )
				{
					Global_MIDIChannelFlag = VARIABLE_DECREASING;
				}

				if( Global_MIDIChannelFlag == VARIABLE_DECREASING )
				{
					AppGlobal_SetMIDIChannel( AppGlobal_GetMIDIChannel() - 1);
					DispMan_Print7SegInt( AppGlobal_GetMIDIChannel() + 1 , 0);
				}
			}
			else
			{
				if( Global_MIDIChannelFlag == VARIABLE_INACTIVE )
				{
					MM_PrevMode(&GlobalStateMachine);
					DispMan_Print7SegAlpha( (uint8_t*)ModeStrings[MM_GetMode(&GlobalStateMachine)] , 0);
				}
			}


			break;

		case SW_TRACK_RIGHT:
			if (IM_GetButtonState(SW_TRACK_LEFT))
			{

				if( Global_MIDIChannelFlag == VARIABLE_INACTIVE )
				{
					Global_MIDIChannelFlag = VARIABLE_INCREASING;
				}

				if( Global_MIDIChannelFlag == VARIABLE_INCREASING )
				{
					AppGlobal_SetMIDIChannel( AppGlobal_GetMIDIChannel() + 1);
					DispMan_Print7SegInt( AppGlobal_GetMIDIChannel() + 1, 0);
				}
			}
			else
			{
				if( Global_MIDIChannelFlag == VARIABLE_INACTIVE )
				{
					MM_NextMode(&GlobalStateMachine);
					DispMan_Print7SegAlpha( (uint8_t*)ModeStrings[MM_GetMode(&GlobalStateMachine)] , 0);
				}
			}
			break;

		case SW_OCTAVE_DOWN:
			if (IM_GetButtonState(SW_OCTAVE_UP))
			{
				if( Global_TransposeFlag == VARIABLE_INACTIVE )
				{
					Global_TransposeFlag = VARIABLE_DECREASING;
				}

				if( Global_TransposeFlag == VARIABLE_DECREASING )
				{
					KB_SetTranspose(KB_GetCurrentTranspose() - 1);
					DispMan_Print7SegInt(KB_GetCurrentTranspose(), 0);
				}

			}
			else
			{
				if (Global_TransposeFlag == VARIABLE_INACTIVE)
				{
					KB_SetOctave(KB_GetCurrentOctave() - 1);
					DispMan_Print7SegInt(KB_GetCurrentOctave(), 0);

					//DispMan_Print7Seg(abs(KB_GetCurrentOctave()), 0);
				}
			}

			break;

		case SW_OCTAVE_UP:
			if (IM_GetButtonState(SW_OCTAVE_DOWN))
			{
				if( Global_TransposeFlag == VARIABLE_INACTIVE )
				{
					Global_TransposeFlag = VARIABLE_INCREASING;
				}

				if( Global_TransposeFlag == VARIABLE_INCREASING )
				{
					KB_SetTranspose(KB_GetCurrentTranspose() + 1);
					DispMan_Print7SegInt(KB_GetCurrentTranspose(), 0);
				}
			}
			else
			{
				if (Global_TransposeFlag == VARIABLE_INACTIVE)
				{
					KB_SetOctave(KB_GetCurrentOctave() + 1);
					DispMan_Print7SegInt(KB_GetCurrentOctave(), 0);
				}
			}

			break;

		default:
			break;
	}

	if ((IM_GetButtonState(SW_OCTAVE_UP) == 0) && IM_GetButtonState(SW_OCTAVE_DOWN) == 0)
	{
		Global_TransposeFlag = VARIABLE_INACTIVE;
	}

	if ((IM_GetButtonState(SW_TRACK_RIGHT) == 0) && IM_GetButtonState(SW_TRACK_LEFT) == 0)
	{
		Global_MIDIChannelFlag = VARIABLE_INACTIVE;
	}

	return MM_INPUT_WAS_PROCESSED;
}

static uint8_t handle_ADCInput(MM_Input_t* input)
{
	ADCEvent_t* adcEvent = input->input.adc;

	if( (adcEvent->index == ADC_PITCHBEND) || (adcEvent->index == ADC_MODULATION) )
	{
		Global_HandleADC(adcEvent);
		DispMan_Print7Seg(adcEvent->value, 0);
		return MM_INPUT_WAS_PROCESSED;
	}

	//DispMan_Print7Seg(adcEvent->value, 0);
	return !MM_INPUT_WAS_PROCESSED;
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

