
#include "LED.h"
#include "LED_7Seg.h"
#include <string.h>

#include <ctype.h>

uint8_t LED_DisplayBuffer[LED_COUNT];

LEDElement_t LED_DisplayInformation[LED_COUNT];

void LED_Init(void)
{
	memset((void*) &LED_DisplayInformation, 0, sizeof(LEDElement_t) * LED_COUNT);
}

void LED_SetLEDBrightness(uint8_t bufferIndex, uint8_t index, uint32_t brightness)
{
	uint32_t newInterval;
	LED_DisplayInformation[index].brightness = brightness;
	newInterval = (LED_DisplayInformation[index].brightness * BRIGHTNESS_MULT_FACTOR) / MAX_LED_BRIGHTNESS;
	newInterval = BRIGHTNESS_MULT_FACTOR - newInterval;
	LED_DisplayInformation[index].interval = newInterval;

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

void LED_7SegmentWriteCode(uint8_t index, uint16_t* code)
{
	uint8_t i;
	for (i = 0; i < LED_7SEG_SEGMENT_COUNT; i++)
	{
		LED_SetLEDBrightness(0, (index * LED_7SEG_LED_COUNT) + i, code[i]);
	}
}

//Number is between 0 and 9 and | LED_7SEG_DP to obtain DP.
//To clear the segment, write a 10
void LED_7Segment_Write(uint8_t index, uint8_t number)
{

	if (number & LED_7SEG_DP)
	{
		LED_SetLEDBrightness(0, (index * LED_7SEG_LED_COUNT) + LED_7SEG_SEGMENT_COUNT, MAX_LED_BRIGHTNESS);
	}

	number = number & 0x0F;

	if (number < LED_7SEG_MAX)
	{
		uint16_t* segmentMap = (uint16_t*) LED_7SEG_DIGITS[number];

		LED_7SegmentWriteCode(index, segmentMap);

	}
	else
	{
		LED_7SegmentWriteCode(index, (uint16_t*) LED_7SEG_NULL);
	}
}

void LED_7Segment_WriteChar(uint8_t index, uint8_t ch)
{

	if (isdigit(ch))
	{
		ch = ch - '0';
		LED_7SegmentWriteCode(index, (uint16_t*) LED_7SEG_DIGITS[ch]);
	}
	else
	{

		switch(ch)
		{
			case '-':
			{
				LED_7SegmentWriteCode(index, (uint16_t*) LED_7SEG_CHAR_DASH);
				break;
			}

			default:
			{
				ch = toupper(ch) - 'A';
				if (ch < 26)
				{
					if (LED_7SEG_CHARS[ch])
					{
						LED_7SegmentWriteCode(index, (uint16_t*) LED_7SEG_CHARS[ch]);
					}
				}
				else
				{
					LED_7SegmentWriteCode(index, (uint16_t*) LED_7SEG_NULL);
				}
				break;
			}
		}


	}
	return;

}

//From 0 up to 999.
void LED_7Segment_WriteNumber(uint16_t number)
{

	uint8_t hundreds = number / 100;
	uint8_t tens = number - (hundreds * 100);
	uint8_t ones;

	ones = tens;

	tens = tens / 10;

	ones = ones - (tens * 10);

	LED_7Segment_Write(0, hundreds);
	LED_7Segment_Write(1, tens);
	LED_7Segment_Write(2, ones);
}

//Counts up
void LED_CountUpRoutine(uint8_t column, uint16_t count)
{
	if (column >= MAX_LINE_COLUMNS)
	{
		return;
	}

	LEDElement_t* displayInfo = (LEDElement_t*) &LED_DisplayInformation[column * LEDS_PER_COLUMN];
	uint8_t i;
	uint16_t ledData = 0;

	for (i = 0; i < LEDS_PER_COLUMN; i++)
	{

		if (displayInfo[i].brightness >= count)
		{
			if (displayInfo[i].brightness)
			{
				ledData |= (1 << i);
			}
		}
		else
		{
			ledData &= ~(1 << i);
		}
	}

	//Need to invert the LED data because a 0 represents ON, and vice versa.
	ledData = ~ledData;
	LED_SetData(ledData);
}

void LED_TimerRoutine(uint8_t column)
{

	if (column >= MAX_LINE_COLUMNS)
	{
		return;
	}

	LEDElement_t* displayInfo = (LEDElement_t*) &LED_DisplayInformation[column * LEDS_PER_COLUMN];
	uint8_t i;
	uint16_t ledData = 0;

	for (i = 0; i < LEDS_PER_COLUMN; i++)
	{
		displayInfo[i].brightnessError += displayInfo[i].interval;

		if (displayInfo[i].brightnessError >= BRIGHTNESS_MULT_FACTOR)
		{
			displayInfo[i].brightnessError = displayInfo[i].brightnessError - BRIGHTNESS_MULT_FACTOR;
			ledData &= ~(1 << i);
		}
		else
		{
			if (displayInfo[i].brightness)
			{
				ledData |= (1 << i);
			}
		}
	}

	//Need to invert the LED data because a 0 represents ON, and vice versa.
	ledData = ~ledData;
	LED_SetData(ledData);
}

uint8_t LED_PAD_SEQUENCE[] = { LED_PAD0_R, LED_PAD0_G, LED_PAD1_R, LED_PAD1_G, LED_PAD2_R, LED_PAD2_G, LED_PAD3_R, LED_PAD3_G, LED_PAD4_R, LED_PAD4_G, LED_PAD5_R, LED_PAD5_G, LED_PAD6_R, LED_PAD6_G,
		LED_PAD7_R, LED_PAD7_G, LED_PAD8_R, LED_PAD8_G, LED_PAD9_R, LED_PAD9_G, LED_PAD10_R, LED_PAD10_G, LED_PAD11_R, LED_PAD11_G, LED_PAD12_R, LED_PAD12_G, LED_PAD13_R, LED_PAD13_G, LED_PAD14_R,
		LED_PAD14_G, LED_PAD15_R, LED_PAD15_G, };

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

	int32_t newData = 0;

	//DrvGPIO_SetPortMask(LED_DATA_PORT,~((1<<LED_DATA_LINE0) | (1<<LED_DATA_LINE1) | (1<<LED_DATA_LINE2) | (1<<LED_DATA_LINE3) |
	//								    (1<<LED_DATA_LINE4) | (1<<LED_DATA_LINE5) | (1<<LED_DATA_LINE6) | (1<<LED_DATA_LINE7)) );

	newData = DrvGPIO_GetPortDoutBits(LED_DATA_PORT) & (~LED_DATA_LINE_MASK);

	newData |= (((data & 0x01) ? 1 : 0) << LED_DATA_LINE0);
	newData |= (((data & 0x02) ? 1 : 0) << LED_DATA_LINE1);
	newData |= (((data & 0x04) ? 1 : 0) << LED_DATA_LINE2);
	newData |= (((data & 0x08) ? 1 : 0) << LED_DATA_LINE3);
	newData |= (((data & 0x10) ? 1 : 0) << LED_DATA_LINE4);
	newData |= (((data & 0x20) ? 1 : 0) << LED_DATA_LINE5);
	newData |= (((data & 0x40) ? 1 : 0) << LED_DATA_LINE6);
	newData |= (((data & 0x80) ? 1 : 0) << LED_DATA_LINE7);

	//DrvGPIO_SetPortBits(LED_DATA_PORT, newData);

	GPIO_T * tGPIO;
	tGPIO = (GPIO_T *) (LED_DATA_GPIO_PORT);
	tGPIO->DOUT &= (~LED_DATA_LINE_MASK);
	tGPIO->DOUT |= newData;

}

