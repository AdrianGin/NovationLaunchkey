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

#include "GenericEvents.h"


uint8_t GenericEvents_AddEvent(VoidBuffer_t* queue, void* event)
{
	return VoidBuffer_PushData( (VoidBuffer_t*)queue, (void*) event);
}

uint8_t GenericEvents_GetEvent(VoidBuffer_t* queue, void* event)
{
	void* eventPtr;
	eventPtr = VoidBuffer_PopData((VoidBuffer_t*)queue, event);

	if( eventPtr != VOIDBUFFER_NO_DATA )
	{
		return HAS_EVENT;
	}
	else
	{
		return !HAS_EVENT;
	}

}






















