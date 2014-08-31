
#include "ControlSurfaceMap.h"
#include "MIDICodes.h"



#include "Rescale.h"



static ControlSurfaceMap_t CurrentMap;



//Takes an ADC event, Surface map and populates the passed in MIDI Msg.
uint8_t ControlMap_TransformADCInput(const ControlSurfaceMap_t** const map, ADCEvent_t* event, MIDIMsg_t* msg)
{
	uint8_t ret = 0;

	Rescale_t rs;
	rs.xMin = 0;
	rs.xMax = ADC_MAX_VALUE;

	if( (event->index >= ADC_KNOB_0)  && (event->index <= ADC_SLIDER_7) )
	{
		uint16_t newVal;
		uint8_t mapOffset = event->index - ADC_KNOB_0;
		ControlSurfaceMap_t* mapElement = (ControlSurfaceMap_t*)&map[mapOffset];

		msg->status = mapElement->statusBytes;

		if( msg->status == 0 )
		{
			msg->status = MIDI_CONTROL_CHANGE;
		}

		msg->data1 = mapElement->controlVal;
		if( msg->data1 == 0 )
		{
			msg->data1 = GENERAL_PURPOSE_CONTROLLER_1 + mapOffset;
		}

		//For uninitialised output map
		if( mapElement->min == mapElement->max )
		{
			rs.yMin = 0;
			rs.yMax = MIDI_MAX_DATA;
		}
		else
		{
			rs.yMin = mapElement->min;
			rs.yMax = mapElement->max;
		}

		newVal = Rescale_Apply(&rs, event->value);

		msg->data2 = (uint8_t)newVal;


		ret = 1;
	}


	return ret;
}








