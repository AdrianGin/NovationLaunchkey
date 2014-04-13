#ifndef HARDWARE_SPECIFIC_H
#define HARDWARE_SPECIFIC_H

#ifdef __cplusplus
extern "C" {
#endif


#include "NUC1xx.h"
#include "DrvSys.h"
#include "DrvGPIO.h"

#define PAD_COUNT    (16)
#define SLIDER_COUNT (9)
#define POT_COUNT    (8)

#define LED_DATA_PORT	(E_GPA)
typedef enum
{
	LED_DATA_LINE0 = 6,
	LED_DATA_LINE1 = 7,
	LED_DATA_LINE2 = 10,
	LED_DATA_LINE3 = 11,
	LED_DATA_LINE4 = 12,
	LED_DATA_LINE5 = 13,
	LED_DATA_LINE6 = 14,
	LED_DATA_LINE7 = 15,

} E_LED_DATA_LINES;


#define MAX_LINE_COLUMNS	(8)
#define MAX_ADC_COLUMNS		(8)

#define MUX_LINE_PORT	(E_GPC)
#define MUX_LINE0_PIN	(6)
#define MUX_LINE1_PIN	(7)
#define MUX_LINE2_PIN	(8)

#define MUX_ADC_PORT	(E_GPC)
#define MUX_ADC0_PIN	(9)
#define MUX_ADC1_PIN	(10)
#define MUX_ADC2_PIN	(11)

void SystemClockInit(void);

//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
