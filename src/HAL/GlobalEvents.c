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

#include "GlobalEvents.h"

#include "App_Keyboard.h"
#include <stdlib.h>


//Each button must remember the Application Mode of when it was pressed (done in EventManager)
uint32_t ButtonStates;

inline void SetButtonState(uint8_t index)
{
	ButtonStates |= (1<<index);
}

inline void ClearButtonState(uint8_t index)
{
	ButtonStates &= ~(1<<index);
}

inline uint8_t GetButtonState(uint8_t index)
{
	if( index >= SW_COUNT )
	{
		return SWITCH_INVALID;
	}
	return ((ButtonStates & (1<<index)) != 0);
}


void ProcessButtonOn(uint8_t inputIndex)
{
	SetButtonState(inputIndex);
	switch(inputIndex)
	{
		case 	SW_TRACK_LEFT:
		break;

		case 	SW_TRACK_RIGHT:
		break;

		case 	SW_OCTAVE_DOWN:
		break;

		case 	SW_OCTAVE_UP:
		break;

		default:
		break;
	}
}


void GE_ProcessButtonOff(uint8_t inputIndex)
{
	SetButtonState(inputIndex);
	switch(inputIndex)
	{
		case 	SW_TRACK_LEFT:
		break;

		case 	SW_TRACK_RIGHT:
		break;

		case 	SW_OCTAVE_DOWN:
			KB_SetOctave(KB_GetCurrentOctave() - 1 );

			DispMan_Print7Seg( abs(KB_GetCurrentOctave()), 0);

			if( GetButtonState(SW_OCTAVE_UP) )
			{

			}
		break;

		case 	SW_OCTAVE_UP:
			KB_SetOctave(KB_GetCurrentOctave() + 1 );
			DispMan_Print7Seg( abs(KB_GetCurrentOctave()), 0);
			if( GetButtonState(SW_OCTAVE_DOWN) )
			{

			}
		break;

		default:
		break;
	}



}

uint8_t GlobEvents_ProcessButton(uint8_t inputIndex, uint8_t value)
{
	if( value )
	{
		ProcessButtonOn(inputIndex);
	}
	else
	{
		GE_ProcessButtonOff(inputIndex);
	}
	
	return 0;
}













