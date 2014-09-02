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


void ModeControl_OutputMessage(uint8_t index, uint8_t value)
{
	//Sending NRPNs, RPNs need 3 MIDI messages.
	MIDIMsg_t msgArray[3];

	MIDIMsg_t* msg = &msgArray[0];
	uint8_t msgCount;
	uint8_t valueToShow = 0;

	valueToShow = ControlMap_TransformInput(CurrentADCMap, &msgArray[0], &msgCount, index, value);

	if( valueToShow )
	{
		switch( valueToShow )
		{
			case SINGLE_MSG_DB2:
			case SINGLE_MSG_DB1:
			{
				if( AppMIDI_IsSavedEventDifferent(msg, index) == APP_MIDI_MSG_DIFFERENT)
				{
					MIDIMsg_t* lastMsg = AppMIDI_GetSavedEvent(index);
					//Make sure we send a MIDI Note off when we send the next MIDI On,
					//otherwise we will have stuck notes.
					if( (lastMsg->status & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON )
					{
						lastMsg->status &= ~MIDI_MSG_TYPE_MASK;
						lastMsg->status |= MIDI_NOTE_OFF;
						HAL_MIDI_TxMsg(lastMsg);

						//Restore the state of the last message
						lastMsg->status &= ~MIDI_MSG_TYPE_MASK;
						lastMsg->status |= MIDI_NOTE_ON;
					}

					AppMIDI_ADCOutputMIDI(msg, index);

					if( valueToShow == SINGLE_MSG_DB2 )
					{
						DispMan_Print7Seg(msg->data2, 0);
					}
					else
					{
						DispMan_Print7Seg(msg->data1, 0);
					}
				}
				break;
			}

			case TRIPLE_MSG:
				if( AppMIDI_IsSavedEventDifferent(&msgArray[2], index) == APP_MIDI_MSG_DIFFERENT)
				{
					AppMIDI_ADCOutputMIDI(&msgArray[0], index);
					AppMIDI_ADCOutputMIDI(&msgArray[1], index);
					AppMIDI_ADCOutputMIDI(&msgArray[2], index);
					DispMan_Print7Seg(msgArray[2].data2, 0);
				}

				break;

			default:
				break;
		}


	}
}

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
	uint8_t index = input->input.adc->index;
	uint8_t value = input->input.adc->value;
	ModeControl_OutputMessage(input->input.adc->index, input->input.adc->value);
	return MM_INPUT_WAS_PROCESSED;
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

