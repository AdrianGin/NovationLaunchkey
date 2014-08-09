

#include <string.h>
#include <stdint.h>

#include "USB_Audio.h"

#include "LED.h"





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
	{USBAudio_BulkInAckCallback, &USBAudio_Device},/* EP address 1 In ACK callback */
	{USBAudio_BulkOutAckCallback, &USBAudio_Device},                             /* EP address 1 Out ACK callback */
	{NULL, NULL},                             /* EP address 2 In ACK callback */
	{USBAudio_BulkOutAckCallback, &USBAudio_Device},                              /* EP address 2 Out ACK callback */
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
	{REQ_STANDARD, SET_CONFIGURATION, USBAudio_CtrlSetupSetConfiguration, 0, 0, &USBAudio_Device},
};



//register to USB driver
S_DRVUSB_CLASS USBClass =
{
	(void *)&USBAudio_Device,
	//USBAudio_StartCallBack,
	0,
	0
};


/* To handle the data transfer size > maximum packet size */
static const uint8_t * gpu8UsbBuf = 0;
static uint32_t gu32BytesInUsbBuf = 0;
static int8_t gIsOverRequest = 0;

volatile uint8_t g_UsbInReady = 0;

int32_t USBAudio_Open(void)
{
	int32_t i32Ret = 0;

	USBAudio_Device.device = (void *)DrvUSB_InstallClassDevice(&USBClass);

	i32Ret = DrvUSB_InstallCtrlHandler(USBAudio_Device.device, g_asCtrlCallbackEntry,
					sizeof(g_asCtrlCallbackEntry) / sizeof(g_asCtrlCallbackEntry[0]));

	return i32Ret;
}

void USBAudio_Close(void)
{

}

//uint8_t inputBuffer[MAX_PACKET_SIZE_BULK_IN] = {;

const uint8_t testData[4] = {0x0F, 0xFE, 0x00, 0x00};


void USBAudio_BulkInAckCallback(void* pVoid)
{
	 g_UsbInReady = 0;
}

void USBAudio_BulkOutAckCallback(void* pVoid)
{

	uint32_t  len;
	uint8_t*  dataPtr;
	dataPtr = DrvUSB_GetOutData(BULK_OUT_EP_NUM,&len);
	USBMIDI_ProcessOutData(dataPtr, len);
}



void USBAudio_StartCallBack(void * pVoid)
{
    USBAudio_Reset((USBAUDIO_DEVICE *)pVoid);
    USBAudio_Start((USBAUDIO_DEVICE *)pVoid);
}

void USBAudio_Reset(USBAUDIO_DEVICE *psDevice)
{
    DrvUSB_Reset(USB_AUDIO_EP);
}

void USBAudio_Start(USBAUDIO_DEVICE *psDevice)
{
    //DrvUSB_DataIn(USB_AUDIO_EP, psDevice->pu8Report, psDevice->u32ReportSize);
    //DrvUSB_DataOutTrigger(USB_AUDIO_EP, MAX_PACKET_SIZE_BULK_IN);
}


void USBAudio_PrepareDescriptors(const uint8_t *pu8Descriptor, uint32_t u32DescriptorSize, uint32_t u32RequestSize, uint32_t u32MaxPacketSize)
{

    gu32BytesInUsbBuf = u32RequestSize;
    gpu8UsbBuf = pu8Descriptor;

    if(u32RequestSize > u32DescriptorSize)
    {
        gu32BytesInUsbBuf = u32DescriptorSize;
        gIsOverRequest = 1;
    }


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

static uint32_t Minimum(uint32_t a, uint32_t b)
{
	if (a > b)
		return b;
	else
		return a;
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


static uint8_t buffer[USB_STRING_DESC_MAX_LEN*2 + 2];

void USBAudio_CtrlSetupGetDescriptor(void * pVoid)
{
	USBAUDIO_DEVICE *psDevice = (USBAUDIO_DEVICE *) pVoid;
	S_DRVUSB_DEVICE *pUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;
	uint16_t u16Len;

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
		uint32_t transferLen = Minimum(u16Len, LEN_DEVICE);
		USBAudio_PrepareDescriptors(DeviceDescriptor, transferLen, u16Len, MAX_PACKET_SIZE_CTRL);
		_DRVUSB_TRIG_EP(1,0x00);
		break;
	}

	// Get Configuration Descriptor
	case DESC_CONFIG:
	{
		uint32_t transferLen = Minimum(u16Len, CONFIG_DESCRIPTOR_LENGTH);
		USBAudio_PrepareDescriptors(ConfigDescriptor, transferLen , u16Len, MAX_PACKET_SIZE_CTRL);
		break;
    }
    // Get String Descriptor
	case DESC_STRING:
	{
		// Get Language
		if (pUsbDevice->au8Setup[2] == 0)
		{
			USBAudio_PrepareDescriptors(StringLang, 4, u16Len, MAX_PACKET_SIZE_CTRL);
			_DRVUSB_TRIG_EP(1,0x00);
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

			case 4:
				USB_DescriptorGetString(JackDesc, buffer);
				USBAudio_PrepareDescriptors((const uint8_t *)buffer, buffer[0], u16Len, MAX_PACKET_SIZE_CTRL);
				break;


			default:
				printNumber(55555);
				printNumber(pUsbDevice->au8Setup[2]);
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



void USBAudio_CtrlSetupSetConfiguration(void* pVoid)
{
	S_DRVUSB_DEVICE *pDrvDevice = &gsUsbDevice;
	S_DRVUSB_CLASS *psUsbClass = pDrvDevice->psUsbClass;
	int32_t bIsDeviceConfigure;

	bIsDeviceConfigure = psUsbClass->pfnCompare ? psUsbClass->pfnCompare(pDrvDevice->au8Setup[2]) : 1;

	if (pDrvDevice->au8Setup[2] == 0)
	{
		// USB address state.
		DrvUSB_SetUsbState(eDRVUSB_ADDRESS);
		pDrvDevice->u8UsbConfiguration = pDrvDevice->au8Setup[2];
		DrvUSB_DataIn(0, NULL, 0);

	}
	else if(bIsDeviceConfigure)
	{
		// USB configured state.
		DrvUSB_SetUsbState(eDRVUSB_CONFIGURED);

		// Call USB class's start function
		if(psUsbClass->pfnStart)
			psUsbClass->pfnStart(pVoid);

		pDrvDevice->u8UsbConfiguration = pDrvDevice->au8Setup[2];

		DrvUSB_DataIn(0, NULL, 0);

		/* Reset bulk in/out endpoint */
		DrvUSB_DataOutTrigger(BULK_OUT_EP_NUM, MAX_PACKET_SIZE_BULK_OUT);
		DrvUSB_DataIn(BULK_IN_EP_NUM, 0, 0);

		LED_SetLEDBrightness(0, LED_PAD2_R, MAX_LED_BRIGHTNESS);
	}
	else
	{
		LED_SetLEDBrightness(0, LED_PAD1_R, MAX_LED_BRIGHTNESS);
		// Not support. Reply STALL.
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

