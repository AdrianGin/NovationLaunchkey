#ifndef MULTIPLEX_CONTROL_H
#define MULTIPLEX_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#include "NUC1xx.h"
#include "DrvSys.h"
#include "DrvGPIO.h"

#define MUX_LINE_PORT	(E_GPC)
#define MUX_LINE0_PIN	(6)
#define MUX_LINE1_PIN	(7)
#define MUX_LINE2_PIN	(8)

#define MUX_ADC_PORT	(E_GPC)
#define MUX_ADC0_PIN	(9)
#define MUX_ADC1_PIN	(10)
#define MUX_ADC2_PIN	(11)


#define MAX_LINE_COLUMNS	(8)
#define MAX_ADC_COLUMNS		(8)

void MUX_GPIO_Init(void);

void MUX_ActivateLineColumn(uint8_t column);
void MUX_ActivateADCColumn(uint8_t column);


//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
