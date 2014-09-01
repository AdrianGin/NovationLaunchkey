
#include "ControlSurfaceMap.h"
#include "MIDICodes.h"


#include "App_GlobalSettings.h"
#include "Rescale.h"





uint8_t ControlMap_EditADCParameter(ControlSurfaceMap_t** map, eCM_Parameters parameter, ADCEvent_t* event)
{
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

				mapElement->statusBytes.midiStatus = (newVal - MIDI_NOTE_OFF + CM_MIDISTATUS_OFFSET) >> CM_MIDISTATUS_BITSHIFT;
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

			case CM_CHANNEL:
				rs.xMin = 0;
				rs.xMax = MIDI_MAX_DATA;
				rs.yMin = 0;
				rs.yMax = MIDI_MAX_CHANNELS;

				//Make it channel independent.
				newVal = Rescale_Apply(&rs, newVal);

				if( newVal == 0)
				{
					mapElement->statusBytes.globalMIDIchanFlag = 1;
					mapElement->statusBytes.midiChannel = newVal;
				}
				else
				{
					mapElement->statusBytes.globalMIDIchanFlag = 0;
					mapElement->statusBytes.midiChannel = newVal - 1;
				}



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

		msg->status = (mapElement->statusBytes.midiStatus);
		if( msg->status == 0 )
		{
			msg->status = MIDI_CONTROL_CHANGE;
		}
		else
		{
			msg->status = (msg->status << CM_MIDISTATUS_BITSHIFT) - CM_MIDISTATUS_OFFSET + MIDI_NOTE_OFF;
		}


		if( mapElement->statusBytes.globalMIDIchanFlag )
		{
			msg->status |= AppGlobal_GetMIDIChannel();
		}
		else
		{
			msg->status |= mapElement->statusBytes.midiChannel;
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

		scaledVal = Rescale_Apply(&rs, event->value);



		//Note on's are swapped around compared to CCs
		//As the 'Value' will be used for Velocity.
		//Some MIDI messages are single value types too.
		uint8_t statusByte = msg->status & MIDI_MSG_TYPE_MASK;

		if( ((statusByte) == MIDI_NOTE_OFF) ||
			((statusByte) == MIDI_NOTE_ON) ||
			((statusByte) == MIDI_PROGRAM_CHANGE) ||
			((statusByte == MIDI_CHANNEL_PRESSURE)))
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








