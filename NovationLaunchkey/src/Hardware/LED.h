#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif


#include "NUC1xx.h"
#include "DrvSys.h"
#include "DrvGPIO.h"

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



void LED_GPIO_Init(void);
void LED_SetData(uint16_t data);


//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
