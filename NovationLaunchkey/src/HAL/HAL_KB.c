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
		59,
		79,
		98,
		117,
		137,
		156,
		176,
		195,
		214,
		234,
		253,
		272,
		292,
		311,
		331,
		350,
		369,
		389,
		408,
		427,
		447,
		466,
		486,
		505,
		524,
		544,
		563,
		582,
		602,
		621,
		640,
		660,
		679,
		699,
		718,
		737,
		757,
		776,
		795,
		815,
		834,
		854,
		873,
		892,
		912,
		931,
		950,
		970,
		989,
		1009,
		1028,
		1047,
		1067,
		1086,
		1105,
		1125,
		1144,
		1163,
		1183,
		1202,
		1222,
		1241,
		1260,
		1280,
		1299,
		1318,
		1338,
		1357,
		1377,
		1396,
		1415,
		1435,
		1454,
		1473,
		1493,
		1512,
		1531,
		1551,
		1570,
		1590,
		1609,
		1628,
		1648,
		1667,
		1686,
		1706,
		1725,
		1745,
		1764,
		1783,
		1803,
		1822,
		1841,
		1861,
		1880,
		1900,
		1919,
		1938,
		1958,
		1977,
		1996,
		2016,
		2035,
		2054,
		2074,
		2093,
		2113,
		2132,
		2151,
		2171,
		2190,
		2209,
		2229,
		2248,
		2268,
		2287,
		2306,
		2326,
		2345,
		2364,
		2384,
		2403,
		2423,
		2442,
		2461,
		2481,
		2500,


};

const uint16_t BLACK_KEYMAP[];

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














