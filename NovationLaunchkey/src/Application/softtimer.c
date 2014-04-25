#include <stdint.h>
#include <stdlib.h>

#include "Softtimer.h"
#include "TimerCallbacks.h"

/* Holds functions for the timers and is an example of implementation*/


void RunCriticalTimer(void)
{
    uint8_t i;
    for (i = 0; i < TIMER1_COUNT; i++)
    {
        if (SoftTimerInterrupt(SoftTimer1[i]))
        {
            SoftTimerReset(SoftTimer1[i]);
            SoftTimerCallback(SoftTimer1[i]);
        }
    }
}




