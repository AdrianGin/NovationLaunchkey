


#include "App_Data.h"


ADC_CentreDetent_t PitchBendDetent =
{
	.centre = 125,
	.threshold = 1,
	.virtualCentreValue = 0x40 << 1,
	.debounceIsActive = 0,
};


ControlSurfaceMap_t LoadedADCMap[POT_COUNT+FADER_COUNT];

const ControlSurfaceMap_t DefaultADCMap[POT_COUNT+FADER_COUNT] =
{
	{
		.statusBytes.globalMIDIchanFlag = 0,
		.statusBytes.midiStatus 				= MIDI_CONTROL_CHANGE - MIDI_NOTE_OFF,
		.statusBytes.midiChannel 			= 0,
		.controlVal 	= GENERAL_PURPOSE_CONTROLLER_1,
		.min			= 0,
		.max			= MIDI_MAX_DATA,
	},
};


ControlSurfaceMap_t** CurrentADCMap = (ControlSurfaceMap_t**)&DefaultADCMap[0];

const Rescale_t Rescale_ModWheel =
{
	.xMax = 250,
	.xMin = 8,

	.yMin = 0,
	.yMax = 127, };












