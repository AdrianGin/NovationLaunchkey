

#include <string.h>
#include <stdint.h>

#include "USB_Audio.h"



typedef struct
{
	S_DRVUSB_VENDOR_INFO sVendorInfo;

	const uint8_t *au8DeviceDescriptor;
	const uint8_t *au8ConfigDescriptor;
	const uint8_t *pu8IntInEPDescriptor;
	const uint8_t *pu8ReportDescriptor;

	void * *device;

} USBAUDIO_DEVICE;

static const uint8_t * gpu8UsbBuf = 0;
static uint32_t gu32BytesInUsbBuf = 0;
static int8_t gIsOverRequest = 0;

USBAUDIO_DEVICE USBAudio_Device;

/* Endpoint description */
S_DRVUSB_EP_CTRL sEpDescription[] =
{
	{CTRL_EP_NUM   | EP_INPUT , MAX_PACKET_SIZE_CTRL   , NULL},   	/* EP Id 0, EP Addr 0, input , max packe size = 8 */
	{CTRL_EP_NUM   | EP_OUTPUT, MAX_PACKET_SIZE_CTRL   , NULL},   	/* EP Id 1, EP Addr 0, output, max packe size = 8 */
	{BULK_IN_EP_NUM | EP_INPUT , MAX_PACKET_SIZE_BULK_IN , NULL},   /* EP Id 2, EP Addr 1, input , max packe size =  64 */
	{BULK_OUT_EP_NUM | EP_OUTPUT , MAX_PACKET_SIZE_BULK_OUT , NULL},   /* EP Id 3, EP Addr 1, output , max packe size =  64 */
	{0x00,  0, NULL}
};

/* bus event call back */
S_DRVUSB_EVENT_PROCESS g_sBusOps[6] =
{
	{NULL, NULL},                               /* attach event callback */
	{NULL, NULL},                               /* detach event callback */
	{DrvUSB_BusResetCallback, &USBAudio_Device},  /* bus reset event callback */
	{DrvUSB_BusSuspendCallback, &USBAudio_Device},/* bus suspend event callback */
	{DrvUSB_BusResumeCallback, &USBAudio_Device},                               /* bus resume event callback */
	{DrvUSB_CtrlSetupAck, &USBAudio_Device},      /* setup event callback */
};

/* USB event call back */
S_DRVUSB_EVENT_PROCESS g_sUsbOps[12] =
{
	{DrvUSB_CtrlDataInAck   , &USBAudio_Device},/* ctrl pipe0 (EP address 0) In ACK callback */
	{DrvUSB_CtrlDataOutAck  , &USBAudio_Device},/* ctrl pipe0 (EP address 0) Out ACK callback */
	{NULL      , &USBAudio_Device},/* EP address 1 In ACK callback */
	{NULL, NULL},                             /* EP address 1 Out ACK callback */
	{NULL, NULL},                             /* EP address 2 In ACK callback */
	{NULL, NULL},                             /* EP address 2 Out ACK callback */
	{NULL, NULL},                             /* EP address 3 In ACK callback */
	{NULL, NULL},                             /* EP address 3 Out ACK callback */
	{NULL, NULL},                             /* EP address 4 In ACK callback */
	{NULL, NULL},                             /* EP address 4 Out ACK callback */
	{NULL, NULL},                             /* EP address 5 In ACK callback */
	{NULL, NULL},                             /* EP address 5 Out ACK callback */
};

/*ctrl pipe call back.                                                                  */
/*it will be call by DrvUSB_CtrlSetupAck, DrvUSB_CtrlDataInAck and DrvUSB_CtrlDataOutAck*/
/*if in ack handler and out ack handler is 0, default handler will be called            */
S_DRVUSB_CTRL_CALLBACK_ENTRY g_asCtrlCallbackEntry[] =
{   //request type,command     ,setup ack handler         , in ack handler      ,out ack handler,  parameter
	{REQ_STANDARD, SET_ADDRESS, DrvUSB_CtrlSetupSetAddress, DrvUSB_CtrlDataInSetAddress, 0, &USBAudio_Device},
	{REQ_STANDARD, CLEAR_FEATURE, DrvUSB_CtrlSetupClearSetFeature, 0, 0, &USBAudio_Device},
	{REQ_STANDARD, SET_FEATURE, DrvUSB_CtrlSetupClearSetFeature, 0, 0, &USBAudio_Device},
	{REQ_STANDARD, GET_CONFIGURATION, DrvUSB_CtrlSetupGetConfiguration, 0, 0, &USBAudio_Device},
	{REQ_STANDARD, GET_STATUS, DrvUSB_CtrlSetupGetStatus, 0, 0, &USBAudio_Device},
	{REQ_STANDARD, GET_INTERFACE, DrvUSB_CtrlSetupGetInterface, 0, 0, &USBAudio_Device},
	{REQ_STANDARD, SET_INTERFACE, DrvUSB_CtrlSetupSetInterface, 0, 0, &USBAudio_Device},
	{REQ_STANDARD, GET_DESCRIPTOR, USBAudio_CtrlSetupGetDescriptor, USBAudio_CtrlGetDescriptorIn, 0/*HID_CtrlGetDescriptorOut*/, &USBAudio_Device},
	{REQ_STANDARD, SET_CONFIGURATION, DrvUSB_CtrlSetupSetConfiguration, 0, 0, &USBAudio_Device},
};



void USBAudio_PrepareDescriptors(const uint8_t *pu8Descriptor, uint32_t u32DescriptorSize, uint32_t u32RequestSize, uint32_t u32MaxPacketSize)
{

    gu32BytesInUsbBuf = u32RequestSize;
    if(u32RequestSize > u32DescriptorSize)
    {
        gu32BytesInUsbBuf = u32DescriptorSize;
        gIsOverRequest = 1;
    }
    gpu8UsbBuf = pu8Descriptor;

	if(gu32BytesInUsbBuf < u32MaxPacketSize)
	{
	    DrvUSB_DataIn(0, gpu8UsbBuf, gu32BytesInUsbBuf);
	    gpu8UsbBuf = 0;
	    gu32BytesInUsbBuf = 0;
	}
	else
	{
		DrvUSB_DataIn(0, gpu8UsbBuf, u32MaxPacketSize);
		gpu8UsbBuf += u32MaxPacketSize;
		gu32BytesInUsbBuf -= u32MaxPacketSize;
    }

}


void USBAudio_CtrlGetDescriptorOut(void * pVoid)
{
    gu32BytesInUsbBuf = 0;
    gpu8UsbBuf = 0;
    gIsOverRequest = 0;
}

void USBAudio_CtrlGetDescriptorIn(void * pVoid)
{
    uint32_t u32Len;

    if(gpu8UsbBuf)
    {

        if(gu32BytesInUsbBuf == 0)
        {
            /* Zero packet */
    		DrvUSB_DataIn(0, gpu8UsbBuf, 0);
    		gpu8UsbBuf = 0;
        }
        else
        {
            u32Len = Minimum(gu32BytesInUsbBuf, MAX_PACKET_SIZE_CTRL);
    		DrvUSB_DataIn(0, gpu8UsbBuf, u32Len);
    		gpu8UsbBuf += u32Len;
    		gu32BytesInUsbBuf -= u32Len;

    		if(gu32BytesInUsbBuf == 0)
    		{
                if(u32Len < MAX_PACKET_SIZE_CTRL)
                {
                    /* This should be last IN packet due to it is less than UAC_MAX_PACKET_SIZE_EP0 */
                    gpu8UsbBuf = 0;
                }
                else
                {
                    if(!gIsOverRequest)
                    {
    		            /* This should be the last IN packet because there is no more data to
                           transfer and it is not over request transfer */
                        gpu8UsbBuf = 0;
                    }
                 }
            }

        }
    }
    else
    {
  	    /* The EP id 1 should always be used as control (OUT) endpoint */
		_DRVUSB_TRIG_EP(1,0x00);
    }
}


/*************************************************************************/
/*                                                                       */
/* DESCRIPTION                                                           */
/*      setup ACK handler for Get descriptor command                     */
/*                                                                       */
/* INPUTS                                                                */
/*      pVoid     parameter passed by DrvUSB_RegisterCtrl                */
/*                                                                       */
/* OUTPUTS                                                               */
/*      None                                                             */
/*                                                                       */
/* RETURN                                                                */
/*      0           Success                                              */
/*		Otherwise	error												 */
/*                                                                       */
/*************************************************************************/
void USBAudio_CtrlSetupGetDescriptor(void * pVoid)
{
	USBAUDIO_DEVICE *psDevice = (USBAUDIO_DEVICE *) pVoid;
	S_DRVUSB_DEVICE *pUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;
	uint16_t u16Len;
	uint8_t buffer[MAX_PACKET_SIZE_CTRL];

	u16Len = 0;
	u16Len = pUsbDevice->au8Setup[7];
	u16Len <<= 8;
	u16Len += pUsbDevice->au8Setup[6];

	gIsOverRequest = 0;
	gu32BytesInUsbBuf = 0;
	gpu8UsbBuf = 0;
	switch (pUsbDevice->au8Setup[3])
	{
		// Get Device Descriptor
	case DESC_DEVICE:
	{
		USBAudio_PrepareDescriptors(DeviceDescriptor, LEN_DEVICE, u16Len, MAX_PACKET_SIZE_CTRL);

	    /* Prepare the OUT to avoid HOST stop data phase without all data transfered. */
		_DRVUSB_TRIG_EP(1,0x00);

		break;
	}

	// Get Configuration Descriptor
	case DESC_CONFIG:
	{
		USBAudio_PrepareDescriptors(ConfigDescriptor, ConfigDescriptor[2], u16Len, MAX_PACKET_SIZE_CTRL);
		break;
    }
    // Get String Descriptor
	case DESC_STRING:
	{
		// Get Language
		if (pUsbDevice->au8Setup[2] == 0)
		{
			USBAudio_PrepareDescriptors(StringLang, 4, u16Len, MAX_PACKET_SIZE_CTRL);
		}
		else
		{
			// Get String Descriptor
			switch (pUsbDevice->au8Setup[2])
			{


			case 1:
				USB_DescriptorGetString(VendorStringDesc, buffer);
				USBAudio_PrepareDescriptors((const uint8_t *)buffer, buffer[0], u16Len, MAX_PACKET_SIZE_CTRL);
				break;

			case 2:
				USB_DescriptorGetString(ProductStringDesc, buffer);
				USBAudio_PrepareDescriptors((const uint8_t *)buffer, buffer[0], u16Len, MAX_PACKET_SIZE_CTRL);
 				break;

			case 3:
				USB_DescriptorGetString(SerialStringDesc, buffer);
				USBAudio_PrepareDescriptors((const uint8_t *)buffer, buffer[0], u16Len, MAX_PACKET_SIZE_CTRL);
				break;

			default:
				/* Not support. Reply STALL. */
				DrvUSB_ClrCtrlReadyAndTrigStall();
			}
		}

		break;
	}
	default:
		/* Not support. Reply STALL. */
		DrvUSB_ClrCtrlReadyAndTrigStall();
	}
}


void DrvUSB_BusSuspendCallback(void * pVoid)
{

	/* Note!! We should not power down or idel in Handler mode */
	DrvUSB_SetUsbState((E_DRVUSB_STATE)((uint32_t)DrvUSB_GetUsbState() | (uint32_t)eDRVUSB_SUSPENDED));

}

void DrvUSB_BusResumeCallback(void * pVoid)
{
	DrvUSB_SetUsbState((E_DRVUSB_STATE)((uint32_t)DrvUSB_GetUsbState() & (uint32_t)(~eDRVUSB_SUSPENDED)));
 }

