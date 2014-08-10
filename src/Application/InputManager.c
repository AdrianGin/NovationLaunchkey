//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs

#include "SwitchEvents.h"
#include "KeyboardEvents.h"
#include "ADCEvents.h"
#include "HAL_MIDI.h"

#include "ModeManager.h"


void IM_HandleSwitchInput(SwitchEvent_t* input)
{
	MM_Input_t tmp;
	tmp.class = eSW_INPUT;
	tmp.input.sw = input;
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

