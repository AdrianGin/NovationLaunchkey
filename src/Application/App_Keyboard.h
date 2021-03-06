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



#ifndef _APP_KEYBOARD
#define _APP_KEYBOARD

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

enum {
	APP_KB_KEY_IS_OFF = 0,
	APP_KB_KEY_IS_ON
};


uint8_t KeyUtil_TimeToVel(uint8_t keyIndex, uint16_t time);

uint8_t KeyUtil_IsBlack(uint8_t keyIndex);

uint8_t KB_ApplyOctaveTranspose(uint8_t rawNote, int8_t octave, int8_t transpose);
int8_t KB_GetCurrentOctave(void);
int8_t KB_GetCurrentTranspose(void);
void KB_SetOctave(int8_t newOctave);
void KB_SetTranspose(int8_t newTranspose);


#ifdef __cplusplus
}
#endif

#endif
