
#include "ControlSurfaceMap.h"
#include "MIDICodes.h"



#include "Rescale.h"



static ControlSurfaceMap_t CurrentMap;





uint8_t ControlMap_EditADCParameter(ControlSurfaceMap_t** map, eCM_Parameters parameter, ADCEvent_t* event)
{
	ControlSurfaceMap_t* mapElement;
	uint8_t index = event->index;
	//Turns ADC_MAX_VALUE into MIDI_MAX_DATA
	uint8_t newVal = event->value >> 1;
	Rescale_t rs;


	if( (index >= ADC_KNOB_0)  && (index <= ADC_SLIDER_7) )
	{
		uint8_t mapOffset = index - ADC_KNOB_0;
		ControlSurfaceMap_t* mapElement = (ControlSurfaceMap_t*)&map[mapOffset];

		switch(parameter)
		{
			case CM_STATUSBYTE:
			{
				rs.xMin = 0;
				rs.xMax = MIDI_MAX_DATA;
				rs.yMin = MIDI_NOTE_OFF;
				rs.yMax = MIDI_CHANNEL_PRESSURE;

				//Make it channel independent.
				newVal = Rescale_Apply(&rs, newVal) & MIDI_MSG_TYPE_MASK;

				mapElement->statusBytes = newVal;
				break;
			}

			case CM_CONTROLVAL:

				mapElement->controlVal = newVal;
				break;

			case CM_MIN:
				mapElement->min = newVal;
				break;

			case CM_MAX:
				mapElement->max = newVal;
				break;

			default:
				break;
		}
	}


	return newVal;
}



//Takes an ADC event, Surface map and populates the passed in MIDI Msg.
uint8_t ControlMap_TransformADCInput(const ControlSurfaceMap_t** const map, ADCEvent_t* event, MIDIMsg_t* msg)
{
	uint8_t ret = 0;

	Rescale_t rs;
	rs.xMin = 0;
	rs.xMax = ADC_MAX_VALUE;

	if( (event->index >= ADC_KNOB_0)  && (event->index <= ADC_SLIDER_7) )
	{
		uint16_t scaledVal;
		uint8_t mapOffset = event->index - ADC_KNOB_0;
		ControlSurfaceMap_t* mapElement = (ControlSurfaceMap_t*)&map[mapOffset];

		msg->status = mapElement->statusBytes;

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

		scaledVal = Rescale_Apply(&rs, event->value);

		if( msg->status == 0 )
		{
			msg->status = MIDI_CONTROL_CHANGE;
		}

		//Note on's are swapped around compared to CCs
		//As the 'Value' will be used for Velocity.
		if( ((msg->status & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_OFF) ||
			((msg->status & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON))
		{
			msg->data1 = scaledVal;
			msg->data2 = mapElement->controlVal;
		}
		else
		{
			msg->data1 = mapElement->controlVal;
			if( msg->data1 == 0 )
			{
				msg->data1 = GENERAL_PURPOSE_CONTROLLER_1 + mapOffset;
			}
			msg->data2 = (uint8_t)scaledVal;
		}

		ret = 1;
	}


	return ret;
}








