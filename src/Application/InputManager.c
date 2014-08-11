//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs

#include "SwitchEvents.h"
#include "KeyboardEvents.h"
#include "ADCEvents.h"
#include "HAL_MIDI.h"

#include "ModeManager.h"
#include "HAL_Switch.h"

#include "InputManager.h"

//Each button must remember the Application Mode of when it was pressed (done in EventManager)
uint32_t ButtonStates;

inline void IM_SetButtonState(uint8_t index)
{
	ButtonStates |= (1<<index);
}

inline void IM_ClearButtonState(uint8_t index)
{
	ButtonStates &= ~(1<<index);
}

uint8_t IM_GetButtonState(uint8_t index)
{
	if( index >= SW_COUNT )
	{
		return SWITCH_INVALID;
	}
	return ((ButtonStates & (1<<index)) != 0);
}


void IM_HandleSwitchInput(SwitchEvent_t* input)
{
	MM_Input_t tmp;
	tmp.class = eSW_INPUT;
	tmp.input.sw = input;

	if( tmp.input.sw->value )
	{
		IM_SetButtonState(tmp.input.sw->index);
	}
	else
	{
		IM_ClearButtonState(tmp.input.sw->index);
	}

	MM_ApplyInput(&CurrentMode, &tmp);
}

void IM_HandleKBInput(KeyboardEvent_t* input)
{
	MM_Input_t tmp;
	tmp.class = eKEYBOARD_INPUT;
	tmp.input.kb = input;
	MM_ApplyInput(&CurrentMode, &tmp);
}


void IM_HandleADCInput(ADCEvent_t* input)
{
	MM_Input_t tmp;
	tmp.class = eADC_INPUT;
	tmp.input.adc = input;
	MM_ApplyInput(&CurrentMode, &tmp);
}


void IM_HandleMIDIInput(MIDIMsg_t* input)
{
	MM_Input_t tmp;
	tmp.class = eMIDI_INPUT;
	tmp.input.midi = input;
	MM_ApplyInput(&CurrentMode, &tmp);
}




void IM_InitCallBacks(void)
{
	EM_RegisterCallBack(eSW_INPUT, (void (*)(void*)) &IM_HandleSwitchInput);
	EM_RegisterCallBack(eKEYBOARD_INPUT, (void (*)(void*)) &IM_HandleKBInput);
	EM_RegisterCallBack(eADC_INPUT, (void (*)(void*))	&IM_HandleADCInput);
	EM_RegisterCallBack(eMIDI_INPUT,(void (*)(void*)) &IM_HandleMIDIInput);
}

