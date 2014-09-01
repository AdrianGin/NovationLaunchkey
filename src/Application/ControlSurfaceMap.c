
#include "ControlSurfaceMap.h"
#include "MIDICodes.h"


#include "App_GlobalSettings.h"
#include "Rescale.h"
#include "App_Data.h"
#include "Mode_Remap.h"



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
				rs.yMin = RT_CC;
				rs.yMax = RT_NOTEON;

				//Make it channel independent.
				newVal = Rescale_Apply(&rs, newVal);
				mapElement->statusBytes.midiStatus = (newVal);

				//newVal = StatusTypeLookup[newVal];
				break;
			}

			case CM_CONTROLVAL:

				mapElement->controlVal = newVal;
				break;

			case CM_LSB:
				mapElement->LSB = newVal;
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


typedef enum {
	CM_NPRN_TYPE = NON_REGISTERED_PARAMETER_LSB,
	CM_PRN_TYPE = REGISTERED_PARAMETER_LSB,
} eCM_PNMsg;


//This function expects the channel to be pre/post populated.
void ControlMap_PreparePNMsg(MIDIMsg_t* msg, eCM_PNMsg type, uint8_t channel)
{
	uint8_t statusByte;
	statusByte = MIDI_CONTROL_CHANGE;

	msg[0].status = statusByte | channel;
	msg[1].status = statusByte | channel;
	msg[2].status = statusByte | channel;

	msg[0].data1 = type+1;
	msg[1].data1 = type;
	msg[2].data1 = DATA_ENTRY_MSB;
}

void ControlMap_PopulatePNMsg(MIDIMsg_t* msg, uint8_t msb, uint8_t lsb, uint8_t data)
{
	msg[0].data2 = msb;
	msg[1].data2 = lsb;
	msg[2].data2 = data;
}


//Takes an ADC event, Surface map and populates the passed in MIDI Msg.
//returns which byte (Data1 or Data2) is largely affected,
//eg. Note Ons are Note Numbers, PC's are PC, as the value affected
//is not always Data2.
uint8_t ControlMap_TransformADCInput(const ControlSurfaceMap_t** const map, ADCEvent_t* event, MIDIMsg_t* msg, uint8_t* msgCount)
{
	uint8_t ret = INFO_INVALID;

	*msgCount = 0;
	Rescale_t rs;
	rs.xMin = 0;
	rs.xMax = ADC_MAX_VALUE;

	if( (event->index >= ADC_KNOB_0)  && (event->index <= ADC_SLIDER_7) )
	{
		uint16_t scaledVal;
		uint8_t mapOffset = event->index - ADC_KNOB_0;
		RT_REMAP_TYPES mapType;
		uint8_t channel;
		ControlSurfaceMap_t* mapElement = (ControlSurfaceMap_t*)&map[mapOffset];

		mapType = mapElement->statusBytes.midiStatus;


		msg->status = StatusTypeLookup[mapType];

		//Apply MIDI channel
		if( mapElement->statusBytes.globalMIDIchanFlag )
		{
			channel = AppGlobal_GetMIDIChannel();
		}
		else
		{
			channel = mapElement->statusBytes.midiChannel;
		}

		//DATA BYTE 1
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

		switch( mapType )
		{
			case RT_NRPN:
				ControlMap_PreparePNMsg(msg, CM_NPRN_TYPE, channel);
				ControlMap_PopulatePNMsg(msg, mapElement->controlVal, mapElement->LSB, scaledVal);
				*msgCount = 3;
				ret = TRIPLE_MSG;
				break;

			case RT_RPN:
				ControlMap_PreparePNMsg(msg, CM_PRN_TYPE, channel);
				ControlMap_PopulatePNMsg(msg, mapElement->controlVal, mapElement->LSB, scaledVal);
				*msgCount = 3;
				ret = TRIPLE_MSG;
				break;

			default:
			{
				*msgCount = 1;
				uint8_t statusByte = msg->status & MIDI_MSG_TYPE_MASK;

				msg->status |= channel;

				//Note on's are swapped around compared to CCs
				//As the 'Value' will be used for Velocity.
				//Some MIDI messages are single value types too.
				if( ((statusByte) == MIDI_NOTE_OFF) ||
					((statusByte) == MIDI_NOTE_ON) ||
					((statusByte) == MIDI_PROGRAM_CHANGE) ||
					((statusByte == MIDI_CHANNEL_PRESSURE)))
				{
					msg->data1 = scaledVal;
					msg->data2 = mapElement->controlVal;
					ret = SINGLE_MSG_DB1;
				}
				else
				{
					msg->data1 = mapElement->controlVal;
					if( CurrentADCMap == (ControlSurfaceMap_t**)&DefaultADCMap[0] )
					{
						msg->data1 = GENERAL_PURPOSE_CONTROLLER_1 + mapOffset;
					}
					msg->data2 = (uint8_t)scaledVal;
					ret = SINGLE_MSG_DB2;
				}
				break;
			}
		}


	}


	return ret;
}








