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



#ifndef _SOFTTIMERS_H
#define _SOFTTIMERS_H

#include <stdint.h>

#define SOFTTIMER_ENABLED (1)
#define SOFTTIMER_DISABLED (0)

typedef struct _softTimer8 {
    uint8_t timeCompare;
    uint8_t timerCounter;
    uint8_t timerEnable;
    void (*timerCallback)(void);
} SoftTimer_8;

typedef struct _softTimer16 {
    uint16_t timeCompare;
    uint16_t timerCounter;
    uint8_t timerEnable;
    void (*timerCallback)(void);
} SoftTimer_16;

typedef struct _softTimer32 {
    uint32_t timeCompare;
    uint32_t timerCounter;
    uint8_t timerEnable;
    void (*timerCallback)(void);
} SoftTimer_32;


#define SoftTimerDec(x)          x.timerCounter--
#define SoftTimerReset(x)        x.timerCounter = x.timeCompare
#define SoftTimerInterrupt(x)    (x.timerEnable && ((--x.timerCounter) == 0))
#define SoftTimerCallback(x)     if(x.timerCallback) (void)(x.timerCallback())


#define SoftTimerStart(x)        SoftTimerReset(x); x.timerEnable = SOFTTIMER_ENABLED
#define SoftTimerStop(x)         x.timerEnable = SOFTTIMER_DISABLED
#define SoftTimerIsEnabled(x)    (x.timerEnable)

void RunAndExecuteTimers(SoftTimer_16* softTimer, uint8_t count);

#endif
