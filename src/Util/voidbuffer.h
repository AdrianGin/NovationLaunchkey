

#ifndef __VOIDBUFFER_H
#define __VOIDBUFFER_H

#include <stdint.h>
#include <stdlib.h>

#define VOIDBUFFER_OVERFLOW	(0)
#define VOIDBUFFER_NO_DATA  (NULL)
#define VOIDBUFFER_EMPTY    (0)

/* Size has to be a size of power of 2 */
typedef struct
{

    void* memPtrArray; //memPtrsArray = uint8_t array[sizeof(void*) * 10];
    uint16_t readPtr;
    uint16_t writePtr;
    uint8_t bufferSize;
    uint8_t elementSize;
} VoidBuffer_t;

//Retrieves the data but doesn't remove it from the queue.
void* VoidBuffer_PeekData(VoidBuffer_t* buf);

/* Critical means disable interrupts on entry and restore interrupt
 * state on exit */
void* VoidBuffer_PopData(VoidBuffer_t* buf, void* dest);
/* returns the size of the buf */
uint8_t VoidBuffer_PushData(VoidBuffer_t* buf, void* data);

uint8_t VoidBuffer_Len(VoidBuffer_t* buf);
uint8_t VoidBuffer_isEmpty(VoidBuffer_t* buf);
void VoidBuffer_Clear(VoidBuffer_t* buf);



#endif


