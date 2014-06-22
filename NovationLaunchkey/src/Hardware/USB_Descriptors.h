#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "DrvUSB.h"

#define	MAX_PACKET_SIZE_CTRL        (8)
#define MAX_PACKET_SIZE_BULK_IN		(64)
#define MAX_PACKET_SIZE_BULK_OUT	(64)

/* Define the interrupt In EP number */
#define CTRL_EP_NUM     (0x00)
#define BULK_IN_EP_NUM	(0x01)
#define BULK_OUT_EP_NUM	(0x02)

#define USB_AUDIO_EP	(0x01)

#define USB_VID (0x1234)
#define USB_PID (0x1111)


#define USB_MAX_POWER	(32)

#define USB_AUDIO_CLASS		(1)
#define USB_AUDIO_CONTROL	(1)
#define MIDI_STREAMING_SUBCLASS (0x03)

#define CLASS_SPECIFIC_INTERFACE (0x24)
#define CLASS_SPECIFIC_ENDPOINT  (0x25)
#define CLASS_HEADER_SUBTYPE	 (0x01)
#define CLASS_REVISION			 (0x0100)

#define AC_STREAMING_COUNT	(1)

#define MIDI_STREAMING_CLASS_LEN    (7)
#define MS_GENERAL	(0x01)

#define MS_EMBEDDED_IN_PORT_COUNT	(1)
#define MS_EMBEDDED_OUT_PORT_COUNT	(1)
#define MS_EXTERNAL_IN_PORT_COUNT	(1)
#define MS_EXTERNAL_OUT_PORT_COUNT	(1)


#define MS_MIDI_IN_DESC_LEN		(6)
#define MS_MIDI_OUT_DESC_LEN	(9)

#define MS_MIDI_BULK_OUT_LEN	(9)
#define MS_MIDI_CS_BULK_OUT_LEN	(4+MS_EMBEDDED_IN_PORT_COUNT)

#define MS_MIDI_BULK_IN_LEN		(9)
#define MS_MIDI_CS_BULK_IN_LEN	(4+MS_EMBEDDED_OUT_PORT_COUNT)

#define MS_CLASS_LENGTH	(MIDI_STREAMING_CLASS_LEN + \
						 MS_EMBEDDED_IN_PORT_COUNT  * MS_MIDI_IN_DESC_LEN	* (MS_EMBEDDED_IN_PORT_COUNT+MS_EXTERNAL_IN_PORT_COUNT) + \
						 MS_EMBEDDED_OUT_PORT_COUNT * MS_MIDI_OUT_DESC_LEN	* (MS_EMBEDDED_OUT_PORT_COUNT+MS_EXTERNAL_OUT_PORT_COUNT) + \
						 MS_MIDI_BULK_OUT_LEN 	+ MS_MIDI_CS_BULK_OUT_LEN + \
						 MS_MIDI_BULK_IN_LEN 	+ MS_MIDI_CS_BULK_IN_LEN)



#define CONFIG_DESCRIPTOR_LENGTH	(LEN_CONFIG+ \
									 LEN_INTERFACE+\
									 LEN_INTERFACE+\
									 LEN_INTERFACE+\
									 MS_CLASS_LENGTH)

#define MS_MIDI_IN_JACK_SUBTYPE		(0x02)
#define MS_MIDI_OUT_JACK_SUBTYPE	(0x03)

#define MS_MIDI_EMBEDDED_TYPE		(0x01)
#define MS_MIDI_EXTERNAL_TYPE		(0x01)

#define USB_STRING_DESC_MAX_LEN		(32)

extern const uint8_t DeviceDescriptor[];
extern const uint8_t ConfigDescriptor[];
extern const uint8_t VendorStringDesc[];
extern const uint8_t ProductStringDesc[];

extern const uint8_t SerialStringDesc[];
extern const uint8_t JackDesc[];
extern const uint8_t StringLang[];

void USB_DescriptorGetString(const uint8_t* strDesc, uint8_t* strBuf);

#ifdef __cplusplus
}
#endif

#endif
