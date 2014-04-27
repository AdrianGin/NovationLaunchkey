#ifndef HARDWARE_SPECIFIC_H
#define HARDWARE_SPECIFIC_H

#ifdef __cplusplus
extern "C" {
#endif


#include "NUC1xx.h"
#include "DrvSys.h"
#include "DrvGPIO.h"

#define KEYBOARD_VAR (61)

#if (KEYBOARD_VAR == 61)
#define PAD_COUNT    	(16)
#define SLIDER_COUNT 	(8)
#define POT_COUNT    	(8)
#define PB_MOD_COUNT	(3)
#endif






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


#define LED_DATA_LINE_MASK ((1<<LED_DATA_LINE0) | (1<<LED_DATA_LINE1) | (1<<LED_DATA_LINE2) | (1<<LED_DATA_LINE3) | \
   					        (1<<LED_DATA_LINE4) | (1<<LED_DATA_LINE5) | (1<<LED_DATA_LINE6) | (1<<LED_DATA_LINE7))

#define MAX_LINE_COLUMNS	(8)
#define MAX_ADC_COLUMNS		(8)
#define MAX_SW_COLUMNS		(6)

#define ADC_INPUTS_PER_COL  (5)
#define ADC_INPUT_COUNT (ADC_INPUTS_PER_COL*MAX_ADC_COLUMNS)

#define MUX_LINE_PORT	(E_GPC)
#define MUX_LINE0_PIN	(6)
#define MUX_LINE1_PIN	(7)
#define MUX_LINE2_PIN	(8)

#define MUX_LINE_PIN_MASK ((1<<MUX_LINE0_PIN) | (1<<MUX_LINE1_PIN) | (1<<MUX_LINE2_PIN))

#define MUX_ADC_PORT	(E_GPC)
#define MUX_ADC0_PIN	(9)
#define MUX_ADC1_PIN	(10)
#define MUX_ADC2_PIN	(11)

#define MUX_ADC_PIN_MASK ((1<<MUX_ADC0_PIN) | (1<<MUX_ADC1_PIN) | (1<<MUX_ADC2_PIN))

#define ADC_INPUT_PORT	 (E_GPA)


#define ADC_INPUT0_PIN (0)
#define ADC_INPUT1_PIN (1)
#define ADC_INPUT2_PIN (2)
#define ADC_INPUT3_PIN (3)
#define ADC_INPUT4_PIN (4)

#define ADC_INPUT_PIN_MASK ((1<<ADC_INPUT0_PIN) | (1<<ADC_INPUT1_PIN) | (1<<ADC_INPUT2_PIN) | (1<<ADC_INPUT3_PIN) | (1<<ADC_INPUT4_PIN))


#define SWITCH_INPUT_PORT	(E_GPC)
#define SWITCH_INPUT0_PIN (0)
#define SWITCH_INPUT1_PIN (1)
#define SWITCH_INPUT2_PIN (2)
#define SWITCH_INPUT3_PIN (3)

#define SWITCH_INPUT_PIN_MASK ( (1<<SWITCH_INPUT0_PIN) | (1<<SWITCH_INPUT1_PIN) | (1<<SWITCH_INPUT2_PIN) | (1<<SWITCH_INPUT3_PIN))


void SystemClockInit(void);
void printMsg(uint8_t* buffer);
void printNumber(uint16_t number);

//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
