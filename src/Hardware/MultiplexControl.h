#ifndef MULTIPLEX_CONTROL_H
#define MULTIPLEX_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#include "NUC1xx.h"
#include "DrvSys.h"
#include "DrvGPIO.h"

#include "HardwareSpecific.h"



uint8_t MUX_GetCurrentColumn(void);
void MUX_GPIO_Init(void);

void MUX_ActivateLineColumn(uint8_t column);
void MUX_ActivateADCColumn(uint8_t column);


//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
