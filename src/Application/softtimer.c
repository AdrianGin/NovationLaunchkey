#include <stdint.h>
#include <stdlib.h>

#include "Softtimer.h"
#include "TimerCallbacks.h"

/* Holds functions for the timers and is an example of implementation*/

void RunAndExecuteTimers(SoftTimer_16* softTimer, uint8_t count)
{
    uint8_t i;
    for (i = 0; i < count; i++)
    {
        if (SoftTimerInterrupt(softTimer[i]))
        {
            SoftTimerReset(softTimer[i]);
            SoftTimerCallback(softTimer[i]);
        }
    }
}




