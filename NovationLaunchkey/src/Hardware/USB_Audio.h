#ifndef USB_AUDIO_H
#define USB_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "HardwareSpecific.h"
#include "DrvUSB.h"
#include "USB_Descriptors.h"

typedef struct
{
	S_DRVUSB_VENDOR_INFO sVendorInfo;
	void * *device;

} USBAUDIO_DEVICE;

extern volatile uint8_t g_UsbInReady;

void USBAudio_StartCallBack(void * pVoid);
void USBAudio_Reset(USBAUDIO_DEVICE *psDevice);
void USBAudio_Start(USBAUDIO_DEVICE *psDevice);
void USBAudio_Close(void);
int32_t USBAudio_Open(void);


void USBAudio_BulkInAckCallback(void* pVoid);
void USBAudio_BulkOutAckCallback(void* pVoid);


void USBAudio_PrepareDescriptors(const uint8_t *pu8Descriptor, uint32_t u32DescriptorSize, uint32_t u32RequestSize, uint32_t u32MaxPacketSize);
void USBAudio_CtrlGetDescriptorOut(void * pVoid);
void USBAudio_CtrlGetDescriptorIn(void * pVoid);
void USBAudio_CtrlSetupGetDescriptor(void * pVoid);

void USBAudio_CtrlSetupSetConfiguration(void* pVoid);

void DrvUSB_BusSuspendCallback(void * pVoid);
void DrvUSB_BusResumeCallback(void * pVoid);

#ifdef __cplusplus
}
#endif

#endif
