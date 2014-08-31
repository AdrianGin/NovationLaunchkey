#ifndef _CONTROL_SURFACE_MAP_H
#define _CONTROL_SURFACE_MAP_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "HAL_MIDI.h"
#include "HAL_ADC.h"
#include "ADCEvents.h"


#define POT_COUNT 		(8)
#define FADER_COUNT 	(9)
#define MUTE_SOLO_COUNT (9)

typedef struct
{
	uint8_t statusBytes; //includes type and channel
	uint8_t controlVal;
	uint8_t min;
	uint8_t max;
} ControlSurfaceMap_t;

uint8_t ControlMap_TransformADCInput(const ControlSurfaceMap_t** const map, ADCEvent_t* event, MIDIMsg_t* msg);



#ifdef __cplusplus
}
#endif

#endif
