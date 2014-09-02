#ifndef _CONTROL_SURFACE_MAP_H
#define _CONTROL_SURFACE_MAP_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "HAL_MIDI.h"
#include "HAL_ADC.h"
#include "ADCEvents.h"


//Corresponds to moving the MIDI STATUS byte down.
#define CM_MIDISTATUS_BITSHIFT (4)
#define CM_MIDISTATUS_OFFSET	(0x10) //Such that 0 represents undef, 1 is Note Off, 2 is Note On etc..

typedef enum
{
	CM_STATUSBYTE = 0,
	CM_CONTROLVAL,
	CM_MIN,
	CM_MAX,
	CM_CHANNEL,
	CM_LSB,
	CM_VARIABLE_COUNT,
} eCM_Parameters;

enum
{
	INFO_INVALID = 0,
	SINGLE_MSG_DB1,
	SINGLE_MSG_DB2,
	TRIPLE_MSG,
};

typedef struct
{
	//statusBytes is packed such that
	struct
	{
		uint8_t globalMIDIchanFlag : 1;
		uint8_t midiStatus : 3;
		uint8_t midiChannel : 4;
	} statusBytes;

	//uint8_t statusBytes; //includes type and channel
	uint8_t controlVal;
	uint8_t LSB;
	uint8_t min;
	uint8_t max;
} ControlSurfaceMap_t;

typedef struct
{
	uint8_t type;
} ButtonMap_t;

uint8_t ControlMap_TransformPotFaderInput(const ControlSurfaceMap_t** const map, MIDIMsg_t* msg, uint8_t* msgCount, ADCEvent_t* event);
uint8_t ControlMap_EditPotFaderParameter(ControlSurfaceMap_t** map, eCM_Parameters parameter, ADCEvent_t* event);

uint8_t ControlMap_TransformInput(const ControlSurfaceMap_t** const map, MIDIMsg_t* msg, uint8_t* msgCount, uint8_t index, uint8_t value);
uint8_t ControlMap_EditParameter(ControlSurfaceMap_t** map, eCM_Parameters parameter, uint8_t index, uint8_t value);





#ifdef __cplusplus
}
#endif

#endif
