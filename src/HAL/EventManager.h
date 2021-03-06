/*

Copyright (c) 2011 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/



#ifndef _EVENT_MANAGER
#define _EVENT_MANAGER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "KeyboardEvents.h"
#include "SwitchEvents.h"
#include "ADCEvents.h"

#include "HAL_MIDI.h"
#include "USBMIDI.h"
#include "USB_Audio.h"


typedef enum
{
	eSW_INPUT,
	eADC_INPUT,
	eKEYBOARD_INPUT,
	eMIDI_INPUT,
	eINVALID_INPUT,
} eINPUT_TYPES;


#define HAS_EVENT (1)

uint8_t EM_ProcessKeyboard(void);
uint8_t EM_ProcessADC(void);
uint8_t EM_ProcessButton(void);
uint8_t EM_ProcessMIDI(void);

void EM_RegisterCallBack(eINPUT_TYPES inputType, void (*fnPtr)(void*) );

#ifdef __cplusplus
}
#endif

#endif
