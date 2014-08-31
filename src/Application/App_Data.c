


#include "App_Data.h"


ADC_CentreDetent_t PitchBendDetent =
{
	.centre = 125,
	.threshold = 1,
	.virtualCentreValue = 0x40 << 1,
	.debounceIsActive = 0,
};


const ControlSurfaceMap_t ADCMap[POT_COUNT+FADER_COUNT+MUTE_SOLO_COUNT] =
{
	{
		.statusBytes 	= MIDI_CONTROL_CHANGE,
		.controlVal 	= GENERAL_PURPOSE_CONTROLLER_1,
		.min			= 0,
		.max			= MIDI_MAX_DATA,
	},





};


const Rescale_t Rescale_ModWheel =
{
	.xMax = 250,
	.xMin = 8,

	.yMin = 0,
	.yMax = 127, };



