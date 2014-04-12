#ifndef HARDWARE_SPECIFIC_H
#define HARDWARE_SPECIFIC_H

#ifdef __cplusplus
extern "C" {
#endif


#include "NUC1xx.h"
#include "DrvSys.h"

#define PAD_COUNT    (16)
#define SLIDER_COUNT (9)
#define POT_COUNT    (8)


void SystemClockInit(void);

//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
