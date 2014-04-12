

#include "LED.h"


uint8_t LED_DisplayBuffer[LED_COUNT];



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

	int32_t newData = 0;

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
