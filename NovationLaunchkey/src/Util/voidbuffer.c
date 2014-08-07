


#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

/* Ring buffer implementation */

#include "voidbuffer.h"


//Retrieves the data but doesn't remove it from the queue.
void* VoidBuffer_PeekData(VoidBuffer_t* buf)
{
    //buffer not empty?
    if (buf->readPtr != buf->writePtr)
    {
		void* tmp;
		tmp = (void*)((uint32_t)buf->memPtrArray + (uint32_t)(buf->readPtr * buf->elementSize));
        //get byte from buffer, update read position and return
        return tmp;
    } else
    {

        return VOIDBUFFER_NO_DATA; /* This is really trying to get a nonexistant byte */
    }

}


/* Critical means disable interrupts on entry and restore interrupt
 * state on exit */
void* VoidBuffer_PopData(VoidBuffer_t* buf, void* dest)
{

    void* ptr = VoidBuffer_PeekData(buf);

    if( dest && ptr )
    {
    	memcpy(dest, ptr, buf->elementSize);
    }

    if( ptr != VOIDBUFFER_NO_DATA)
    {
    	buf->readPtr = (buf->readPtr + 1) & (buf->bufferSize - 1);
    }

    return ptr;
}


/* returns the size of the buf
 * Note that this is not multithread safe.
 */
uint8_t VoidBuffer_PushData(VoidBuffer_t* buf, void* data)
{
    //is there space in the buffer?
    uint8_t bufferMask = (buf->bufferSize - 1);

    if ( ((buf->writePtr + 1) & bufferMask) != (buf->readPtr & bufferMask))
    {
        //wrap around write position
        //write the character
		  void* tmp;
			tmp = (void*)((uint32_t)buf->memPtrArray + (uint32_t)(buf->writePtr * buf->elementSize));
    	memcpy(tmp, data, buf->elementSize);
        //update size info
    	buf->writePtr = (buf->writePtr + 1) & bufferMask;
        return VoidBuffer_Len(buf);
    } 

    return (uint8_t)(VOIDBUFFER_OVERFLOW);
    
}

uint8_t VoidBuffer_Len(VoidBuffer_t* buf)
{
    if( buf->readPtr > buf->writePtr )
    {
        return buf->writePtr + buf->bufferSize - buf->readPtr;
    }
    return buf->writePtr - buf->readPtr;
}

uint8_t VoidBuffer_isEmpty(VoidBuffer_t* buf)
{
   if( buf->writePtr == buf->readPtr )
   {
	   return VOIDBUFFER_EMPTY;
   }

   return !VOIDBUFFER_EMPTY;
}


void VoidBuffer_Clear(VoidBuffer_t* buf)
{
    buf->readPtr = 0;
    buf->writePtr = 0;
}

