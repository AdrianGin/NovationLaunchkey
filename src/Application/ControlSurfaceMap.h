#ifndef _CONTROL_SURFACE_MAP_H
#define _CONTROL_SURFACE_MAP_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "HAL_MIDI.h"
#include "HAL_ADC.h"
#include "ADCEvents.h"



typedef enum
{
	CM_STATUSBYTE = 0,
	CM_CONTROLVAL,
	CM_MIN,
	CM_MAX,
	CM_VARIABLE_COUNT,
} eCM_Parameters;

typedef struct
{
	uint8_t statusBytes; //includes type and channel
	uint8_t controlVal;
	uint8_t min;
	uint8_t max;
} ControlSurfaceMap_t;

uint8_t ControlMap_TransformADCInput(const ControlSurfaceMap_t** const map, ADCEvent_t* event, MIDIMsg_t* msg);
uint8_t ControlMap_EditADCParameter(ControlSurfaceMap_t** map, eCM_Parameters parameter, ADCEvent_t* event);


#ifdef __cplusplus
}
#endif

#endif
