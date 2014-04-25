#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif


#include "NUC1xx.h"
#include "DrvSys.h"
#include "DrvGPIO.h"
#include "HardwareSpecific.h"

#define LEDS_PER_COLUMN (8)
#define LED_BRIGHTNESS_RES (4)
#define MAX_LED_BRIGHTNESS ((1<<LED_BRIGHTNESS_RES)-1)
#define BRIGHTNESS_MULT_FACTOR (1000)

#define LED_7SEG_MAX (10)
#define LED_7SEG_SEGMENT_COUNT (7)
#define LED_7SEG_DP (0x80)
#define LED_7SEG_LED_COUNT (8)

typedef enum
{
	LED_STATE_ON,
	LED_STATE_BLANK,
	LED_STATE_COUNT

} LED_STATES_t;

/*

7 Segment index
 ___A___
|     	|
F		B
|___G___|
|		|
E		C
|___D___| DOT

 */




typedef enum
{
	LED_7SEG0_A = 0,
	LED_7SEG0_B,
	LED_7SEG0_C,
	LED_7SEG0_D,
	LED_7SEG0_E,
	LED_7SEG0_F,
	LED_7SEG0_G,
	LED_7SEG0_DOT,

	LED_7SEG1_A,
	LED_7SEG1_B,
	LED_7SEG1_C,
	LED_7SEG1_D,
	LED_7SEG1_E,
	LED_7SEG1_F,
	LED_7SEG1_G,
	LED_7SEG1_DOT,

	LED_7SEG2_A,
	LED_7SEG2_B,
	LED_7SEG2_C,
	LED_7SEG2_D,
	LED_7SEG2_E,
	LED_7SEG2_F,
	LED_7SEG2_G,
	LED_7SEG2_DOT,

	LED_INCONTROL_TOP,
	LED_INCONTROL_MID,
	LED_INCONTROL_BOT,
	LED_MASTER,

	LED_CIRCLE_PAD0_G,
	LED_CIRCLE_PAD0_R,
	LED_CIRCLE_PAD1_G,
	LED_CIRCLE_PAD1_R,

	LED_PAD3_G,
	LED_PAD3_R,
	LED_PAD4_G,
	LED_PAD4_R,
	LED_PAD13_G,
	LED_PAD13_R,
	LED_PAD8_G,
	LED_PAD8_R,

	LED_PAD2_G,
	LED_PAD2_R,
	LED_PAD7_G,
	LED_PAD7_R,
	LED_PAD14_G,
	LED_PAD14_R,
	LED_PAD10_G,
	LED_PAD10_R,

	LED_PAD1_G,
	LED_PAD1_R,
	LED_PAD6_G,
	LED_PAD6_R,
	LED_PAD15_G,
	LED_PAD15_R,
	LED_PAD11_G,
	LED_PAD11_R,

	LED_PAD0_G,
	LED_PAD0_R,
	LED_PAD5_G,
	LED_PAD5_R,
	LED_PAD12_G,
	LED_PAD12_R,
	LED_PAD9_G,
	LED_PAD9_R,

   LED_COUNT

} E_LED_INDEX;


typedef struct
{
   uint8_t index;
   uint16_t brightness;
   uint32_t brightnessError;
   uint32_t interval;
} LEDElement_t;

typedef struct
{
   uint16_t brightnessHi;
   uint16_t brightnessLo;
} LEDBrightnessElement_t;

extern uint8_t LED_PAD_SEQUENCE[];
extern const uint8_t LED_LINE_MAP[];

void LED_Init(void);
void LED_GPIO_Init(void);
void LED_SetData(uint16_t data);
void LED_TimerRoutine(uint8_t column);
void LED_SetLEDBrightness(uint8_t bufferIndex, uint8_t index, uint32_t brightness);
void LED_Blank(void);


void LED_7Segment_Write(uint8_t index, uint8_t number);
void LED_7Segment_WriteNumber(uint16_t number);

//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
