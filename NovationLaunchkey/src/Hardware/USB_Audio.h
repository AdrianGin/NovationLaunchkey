#ifndef USB_AUDIO_H
#define USB_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "HardwareSpecific.h"
#include "DrvUSB.h"
#include "USB_Descriptors.h"


void USBAudio_PrepareDescriptors(const uint8_t *pu8Descriptor, uint32_t u32DescriptorSize, uint32_t u32RequestSize, uint32_t u32MaxPacketSize);
void USBAudio_CtrlGetDescriptorOut(void * pVoid);
void USBAudio_CtrlGetDescriptorIn(void * pVoid);
void USBAudio_CtrlSetupGetDescriptor(void * pVoid);
void DrvUSB_BusSuspendCallback(void * pVoid);
void DrvUSB_BusResumeCallback(void * pVoid);

#ifdef __cplusplus
}
#endif

#endif
