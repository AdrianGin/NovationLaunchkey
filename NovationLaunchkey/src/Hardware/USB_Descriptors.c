


#include <string.h>
#include <stdint.h>

#include "USB_Descriptors.h"

const uint8_t VendorStringDesc[]  = "AG Funds Ltd.";
const uint8_t ProductStringDesc[]  = "AG Launchkey X";

const uint8_t SerialStringDesc[]  = "2.0A";
const uint8_t JackDesc[]  =         "AG Launchkey X";

const uint8_t StringLang[] =
{
	4,				// bLength
	DESC_STRING,	// bDescriptorType
	0x09, 0x04
};



const uint8_t DeviceDescriptor[] =
{
	LEN_DEVICE,		// bLength
	DESC_DEVICE,	// bDescriptorType
	0x00, 0x02,		// bcdUSB
	0x00,			// bDeviceClass
	0x00,			// bDeviceSubClass
	0x00,			// bDeviceProtocol
	MAX_PACKET_SIZE_CTRL,	// bMaxPacketSize0
	// idVendor
	USB_VID & 0x00FF,
	(USB_VID & 0xFF00) >> 8,
	// idProduct
	USB_PID & 0x00FF,
	(USB_PID & 0xFF00) >> 8,
	0x00, 0x00,		// bcdDevice
	0x01,			// iManufacture
	0x02,			// iProduct
	0x03,			// iSerialNumber
	0x01			// bNumConfigurations
};

const uint8_t ConfigDescriptor[CONFIG_DESCRIPTOR_LENGTH] =
{
	LEN_CONFIG,		// bLength
	DESC_CONFIG,	// bDescriptorType
	// wTotalLength
	CONFIG_DESCRIPTOR_LENGTH & 0x00FF,
	(CONFIG_DESCRIPTOR_LENGTH & 0xFF00) >> 8,
	0x02,			// bNumInterfaces
	0x01,			// bConfigurationValue
	0x00,			// iConfiguration
	0x80,			// bmAttributes
	USB_MAX_POWER,			// MaxPower

	LEN_INTERFACE,		// bLength
	DESC_INTERFACE,	// bDescriptorType
	0x00,			// bInterfaceNumber
	0x00,			// bAlternateSetting
	0x00,			// bNumEndpoints
	USB_AUDIO_CLASS,			// bInterfaceClass
	USB_AUDIO_CONTROL,			// bInterfaceSubClass
	0x00,			// bInterfaceProtocol
	0x00,			// iInterface

	LEN_INTERFACE,			    			// bLength
	CLASS_SPECIFIC_INTERFACE,				// bDescriptorType
	CLASS_HEADER_SUBTYPE,					// bDescriptorSubType
	CLASS_REVISION & 0x00FF,				// bcdADC
	(CLASS_REVISION & 0xFF00) >> 8,			// bcdADC
	LEN_INTERFACE & 0x00FF,			// wTotalLength
	(LEN_INTERFACE & 0xFF00) >> 8,			// wTotalLength
	AC_STREAMING_COUNT,			// bInCollection
	1,			// baInterface(Nr)

	LEN_INTERFACE,		// bLength
	DESC_INTERFACE,	// bDescriptorType
	0x01,			// bInterfaceNumber
	0x00,			// bAlternateSetting
	0x02,			// bNumEndpoints
	USB_AUDIO_CLASS,			// bInterfaceClass
	MIDI_STREAMING_SUBCLASS,			// bInterfaceSubClass
	0x00,			// bInterfaceProtocol
	0x00,			// iInterface

	MIDI_STREAMING_CLASS_LEN,			    			// bLength
	CLASS_SPECIFIC_INTERFACE,				// bDescriptorType
	CLASS_HEADER_SUBTYPE,					// bDescriptorSubType
	CLASS_REVISION & 0x00FF,				// bcdADC
	(CLASS_REVISION & 0xFF00) >> 8,			// bcdADC
	MS_CLASS_LENGTH & 0x00FF,					// wTotalLength
	(MS_CLASS_LENGTH & 0xFF00) >> 8,			// wTotalLength

		MS_MIDI_IN_DESC_LEN,
		CLASS_SPECIFIC_INTERFACE,
		MS_MIDI_IN_JACK_SUBTYPE,
		MS_MIDI_EMBEDDED_TYPE,
		1,	//bJackID
		4,	//iJack can be used to specify the name of the MIDI Port

		MS_MIDI_IN_DESC_LEN,
		CLASS_SPECIFIC_INTERFACE,
		MS_MIDI_IN_JACK_SUBTYPE,
		MS_MIDI_EXTERNAL_TYPE,
		2,	//bJackID
		4,	//iJack can be used to specify the name of the MIDI Port

		MS_MIDI_OUT_DESC_LEN,
		CLASS_SPECIFIC_INTERFACE,
		MS_MIDI_OUT_JACK_SUBTYPE,
		MS_MIDI_EMBEDDED_TYPE,
		0x03,	//bJackID
		0x01,  	//bNrInputPins
		0x02,	//BaSourceID(1)
		0x01, 	//BaSourcePin(1)
		4,		//iJack can be used to specify the name of the MIDI Port

		MS_MIDI_OUT_DESC_LEN,
		CLASS_SPECIFIC_INTERFACE,
		MS_MIDI_OUT_JACK_SUBTYPE,
		MS_MIDI_EXTERNAL_TYPE,
		0x04,	//bJackID
		0x01,  	//bNrInputPins
		0x01,	//BaSourceID(1)
		0x01, 	//BaSourcePin(1)
		4,		//iJack can be used to specify the name of the MIDI Port

		MS_MIDI_BULK_OUT_LEN,
		DESC_ENDPOINT,								// bDescriptorType
		EP_OUTPUT | BULK_OUT_EP_NUM,				//bEndpointAddress
		EP_BULK,									//bmAttributes
		MAX_PACKET_SIZE_BULK_OUT & 0x00FF,			//wMaxPacketSize
		(MAX_PACKET_SIZE_BULK_OUT & 0xFF00) >> 8, 	//wMaxPacketSize
		0, 											//bInterval
		0, //bRefresh
		0, //bSyncAddress

		MS_MIDI_CS_BULK_OUT_LEN,
		CLASS_SPECIFIC_ENDPOINT,								// bDescriptorType
		MS_GENERAL,				//bDescriptorSubtype
		MS_EMBEDDED_IN_PORT_COUNT,									//bNumEmbMIDIJack
		1,	//Jack IDs of MIDI IN Embeddeds

		MS_MIDI_BULK_IN_LEN,
		DESC_ENDPOINT,								// bDescriptorType
		EP_INPUT | BULK_IN_EP_NUM,				//bEndpointAddress
		EP_BULK,									//bmAttributes
		MAX_PACKET_SIZE_BULK_IN & 0x00FF,			//wMaxPacketSize
		(MAX_PACKET_SIZE_BULK_IN & 0xFF00) >> 8, 	//wMaxPacketSize
		0, 											//bInterval
		0, //bRefresh
		0, //bSyncAddress

		MS_MIDI_CS_BULK_IN_LEN,
		CLASS_SPECIFIC_ENDPOINT,								// bDescriptorType
		MS_GENERAL,				//bDescriptorSubtype
		MS_EMBEDDED_OUT_PORT_COUNT,									//bNumEmbMIDIJack
		3,						//Jack IDs of MIDI OUT Embeddeds

};


/*******************************************************************************
 * converts a Cstring to a USB type string descriptor
*******************************************************************************/
void USB_DescriptorGetString(const uint8_t* strDesc, uint8_t* strBuf)
{
	uint8_t i, len;
	for (i = 0, len = 2; strDesc[i] != 0; i++)
	{
		strBuf[len++] = strDesc[i];
		strBuf[len++] = 0;
	}
	strBuf[0] = len;
	strBuf[1] = DESC_STRING;
}






