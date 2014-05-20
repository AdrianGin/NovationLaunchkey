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

#include "HAL_KB.h"
#include <stdint.h>
#include "binarySearch.h"


const uint16_t WHITE_KEYMAP[] =
{
		40,
		80,
		84,
		88,
		92,
		96,
		100,
		104,
		108,
		112,
		116,
		120,
		124,
		128,
		132,
		136,
		140,
		144,
		148,
		152,
		156,
		160,
		164,
		168,
		172,
		176,
		180,
		184,
		188,
		192,
		196,
		200,
		204,
		208,
		212,
		216,
		220,
		224,
		228,
		232,
		236,
		240,
		244,
		248,
		252,
		256,
		260,
		264,
		268,
		272,
		276,
		280,
		284,
		288,
		292,
		296,
		300,
		304,
		308,
		312,
		316,
		320,
		324,
		328,
		332,
		336,
		340,
		344,
		348,
		352,
		356,
		360,
		364,
		368,
		372,
		376,
		380,
		384,
		388,
		392,
		396,
		400,
		404,
		408,
		412,
		416,
		420,
		424,
		428,
		432,
		436,
		440,
		444,
		448,
		452,
		456,
		460,
		464,
		480,
		500,
		520,
		540,
		560,
		580,
		600,
		620,
		640,
		660,
		680,
		700,
		720,
		740,
		760,
		780,
		800,
		820,
		840,
		860,
		880,
		900,
		920,
		940,
		960,
		980,
		1010,
		1050,
		1100,

};

const uint16_t BLACK_KEYMAP[];

static uint8_t HAL_KB_CurrentOctave = 0;
static uint8_t HAL_KB_CurrentTranspose = 0;


inline uint8_t HAL_KB_GetCurrentOctave(void)
{
	return HAL_KB_CurrentOctave;
}

inline uint8_t HAL_KB_GetCurrentTranspose(void)
{
	return HAL_KB_CurrentTranspose;
}

//convert the hardwareKey Index into a MIDI Keyboard Keynote
inline uint8_t HAL_KB_ConvertKeyIndex2MIDIKey(uint8_t keyIndex)
{
	return keyIndex + KB_FIRST_KEY;
}

//receives the Hardware KeyIndex, 0 = First HardwareKey.
uint8_t HAL_KB_IsBlack(uint8_t keyIndex)
{
	uint8_t halKeyIndex;
	halKeyIndex = HAL_KB_ConvertKeyIndex2MIDIKey(keyIndex);
	//remove octave offset.
	halKeyIndex = halKeyIndex % NOTE_COUNT;
	switch( halKeyIndex )
	{
		case    MN_C:
		case    MN_D:
		case    MN_E:
		case    MN_F:
		case    MN_G:
		case    MN_A:
		case    MN_B:
			return 0;
	}
	return 1;
}

//receives the hardwareKey Index and returns the velocity.
uint8_t HAL_KB_TimeToVel(uint8_t keyIndex, uint16_t time)
{
	
	uint16_t ret;
	uint8_t velocity;

	if( HAL_KB_IsBlack(keyIndex) )
	{
		ret = findElement_binarySearch(time, (uint16_t*)&WHITE_KEYMAP, 127 );
	}
	else
	{
		ret = findElement_binarySearch(time, (uint16_t*)&WHITE_KEYMAP, 127 );
	}

	switch( ret )
	{
		case  BSEARCH_GREATER_THAN_LARGEST:
			ret = MIDI_MAX_VELOCITY-1;
			break;
		case BSEARCH_LESS_THAN_SMALLEST:
			ret = 0;
			break;
		default:
			break;
	}

	velocity = MIDI_MAX_VELOCITY - ret;
	return velocity;
}














