

#include "LED.h"
#include <string.h>


const uint8_t LED_LINE_MAP[] = {
		LED_DATA_LINE0,
		LED_DATA_LINE1,
		LED_DATA_LINE2,
		LED_DATA_LINE3,
		LED_DATA_LINE4,
		LED_DATA_LINE5,
		LED_DATA_LINE6,
		LED_DATA_LINE7,
};

uint8_t LED_DisplayBuffer[LED_COUNT];

LEDElement_t LED_DisplayInformation[LED_COUNT];

void LED_Init(void)
{
   memset((void*)&LED_DisplayInformation, 0 , sizeof(LEDElement_t)*LED_COUNT );
}

void LED_SetLEDBrightness(uint8_t bufferIndex, uint8_t index, uint16_t brightness)
{
//	uint32_t newInterval;

	LED_DisplayInformation[index].brightness = brightness;

//	newInterval = (LED_DisplayInformation[index].brightness * BRIGHTNESS_MULT_FACTOR) / MAX_LED_BRIGHTNESS;
//	newInterval = BRIGHTNESS_MULT_FACTOR - newInterval;
//	LED_DisplayInformation[index].interval = newInterval;

}

void LED_Blank(void)
{
   LED_SetData(0xFF);
}

//Column will be already loaded.
//We need to load into the LED Data lines the correct information.
//The brightness information needs to be loaded too.

//Instead of checking brightness > setBrightness, it introduces flickering.
//We need to alternate between MAX and MIN brightnesses so they converge
//
// eg, 255 = 1111111111
// 	   128 = 1010101010
//
// instead of:
//	   128 = 1111100000 etc..
//

 uint8_t LED_7SEG_DIGIT_0[] = {
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
0
};

 uint8_t LED_7SEG_DIGIT_1[] = {
0,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
0,
0,
0,
0
};

 uint8_t LED_7SEG_DIGIT_2[] = {
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
0,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
0,
MAX_LED_BRIGHTNESS
};

 uint8_t LED_7SEG_DIGIT_3[] = {
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
0,
0,
MAX_LED_BRIGHTNESS
};

 uint8_t LED_7SEG_DIGIT_4[] = {
0,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
0,
0,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS
};


 uint8_t LED_7SEG_DIGIT_5[] = {
MAX_LED_BRIGHTNESS,
0,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
0,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS
};


 uint8_t LED_7SEG_DIGIT_6[] = {
MAX_LED_BRIGHTNESS,
0,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS
};


 uint8_t LED_7SEG_DIGIT_7[] = {
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
0,
0,
0,
0
};


 uint8_t LED_7SEG_DIGIT_8[] = {
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS
};

 uint8_t LED_7SEG_DIGIT_9[] = {
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS,
0,
MAX_LED_BRIGHTNESS,
MAX_LED_BRIGHTNESS
};


uint8_t* LED_7SEG_DIGITS[] = {
LED_7SEG_DIGIT_0,
LED_7SEG_DIGIT_1,
LED_7SEG_DIGIT_2,
LED_7SEG_DIGIT_3,
LED_7SEG_DIGIT_4,
LED_7SEG_DIGIT_5,
LED_7SEG_DIGIT_6,
LED_7SEG_DIGIT_7,
LED_7SEG_DIGIT_8,
LED_7SEG_DIGIT_9,
};



void LED_7Segment_Write(uint8_t index, uint8_t number)
{

	if( number & LED_7SEG_DP )
	{
		LED_SetLEDBrightness(0, (index*LED_7SEG_LED_COUNT) +LED_7SEG_SEGMENT_COUNT ,MAX_LED_BRIGHTNESS);
	}

	number = number & 0x0F;

	if( number < LED_7SEG_MAX )
	{
		uint8_t i;
		uint8_t* segmentMap = LED_7SEG_DIGITS[number];

		for( i = 0; i < LED_7SEG_SEGMENT_COUNT; i++)
		{
			if( segmentMap[i])
			{
				LED_SetLEDBrightness(0, (index*LED_7SEG_LED_COUNT)+i , segmentMap[i] );
			}
		}
	}
}


void LED_TimerRoutine(uint8_t column)
{
	LEDElement_t* displayInfo = (LEDElement_t*)&LED_DisplayInformation[column * LEDS_PER_COLUMN];
	uint8_t i;
	uint16_t ledData = 0;

	for( i = 0 ; i < LEDS_PER_COLUMN; i++ )
	{

   uint8_t index = (column * LEDS_PER_COLUMN) + i;

   uint32_t newInterval;

	newInterval = (displayInfo[i].brightness * BRIGHTNESS_MULT_FACTOR) / MAX_LED_BRIGHTNESS;
	newInterval = BRIGHTNESS_MULT_FACTOR - newInterval;
	
      //LED_DisplayInformation[index].brightnessError +=   newInterval;

		displayInfo[i].brightnessError += newInterval;

		if( displayInfo[i].brightnessError >= BRIGHTNESS_MULT_FACTOR)
		{
			displayInfo[i].brightnessError = displayInfo[i].brightnessError - BRIGHTNESS_MULT_FACTOR;
			ledData &= ~(1<<i);
		}
		else
		{
         if(  displayInfo[i].brightness )
         {
            ledData |= (1<<i);
         }
		}
	}

	//Need to invert the LED data because a 0 represents ON, and vice versa.
	ledData = ~ledData;
	LED_SetData(ledData);
}


uint8_t LED_PAD_SEQUENCE[] =
{
		LED_PAD0_R, LED_PAD0_G,
		LED_PAD1_R, LED_PAD1_G,
		LED_PAD2_R, LED_PAD2_G,
		LED_PAD3_R, LED_PAD3_G,
		LED_PAD4_R, LED_PAD4_G,
		LED_PAD5_R, LED_PAD5_G,
		LED_PAD6_R, LED_PAD6_G,
		LED_PAD7_R, LED_PAD7_G,
		LED_PAD8_R, LED_PAD8_G,
		LED_PAD9_R, LED_PAD9_G,
		LED_PAD10_R, LED_PAD10_G,
		LED_PAD11_R, LED_PAD11_G,
		LED_PAD12_R, LED_PAD12_G,
		LED_PAD13_R, LED_PAD13_G,
		LED_PAD14_R, LED_PAD14_G,
		LED_PAD15_R, LED_PAD15_G,
};



void LED_GPIO_Init(void)
{
	DrvGPIO_Open(LED_DATA_PORT, LED_DATA_LINE0, E_IO_OUTPUT);
	DrvGPIO_Open(LED_DATA_PORT, LED_DATA_LINE1, E_IO_OUTPUT);
	DrvGPIO_Open(LED_DATA_PORT, LED_DATA_LINE2, E_IO_OUTPUT);
	DrvGPIO_Open(LED_DATA_PORT, LED_DATA_LINE3, E_IO_OUTPUT);
	DrvGPIO_Open(LED_DATA_PORT, LED_DATA_LINE4, E_IO_OUTPUT);
	DrvGPIO_Open(LED_DATA_PORT, LED_DATA_LINE5, E_IO_OUTPUT);
	DrvGPIO_Open(LED_DATA_PORT, LED_DATA_LINE6, E_IO_OUTPUT);
	DrvGPIO_Open(LED_DATA_PORT, LED_DATA_LINE7, E_IO_OUTPUT);
}



void LED_SetData(uint16_t data)
{

	uint32_t newData = 0;

	DrvGPIO_SetPortMask(LED_DATA_PORT,~((1<<LED_DATA_LINE0) | (1<<LED_DATA_LINE1) | (1<<LED_DATA_LINE2) | (1<<LED_DATA_LINE3) |
									    (1<<LED_DATA_LINE4) | (1<<LED_DATA_LINE5) | (1<<LED_DATA_LINE6) | (1<<LED_DATA_LINE7)) );

	newData |= ( ((data & 0x01)  ? 1 : 0)  << LED_DATA_LINE0);
	newData |= ( ((data & 0x02)  ? 1 : 0)  << LED_DATA_LINE1);
	newData |= ( ((data & 0x04)  ? 1 : 0)  << LED_DATA_LINE2);
	newData |= ( ((data & 0x08)  ? 1 : 0)  << LED_DATA_LINE3);
	newData |= ( ((data & 0x10)  ? 1 : 0)  << LED_DATA_LINE4);
	newData |= ( ((data & 0x20)  ? 1 : 0)  << LED_DATA_LINE5);
	newData |= ( ((data & 0x40)  ? 1 : 0)  << LED_DATA_LINE6);
	newData |= ( ((data & 0x80)  ? 1 : 0)  << LED_DATA_LINE7);
	DrvGPIO_SetPortBits(LED_DATA_PORT, newData);

}


