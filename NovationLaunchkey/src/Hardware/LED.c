

#include "LED.h"


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


	newData |= ((data & 0x01) << LED_DATA_LINE0);
	newData |= ((data & 0x02) << LED_DATA_LINE1);
	newData |= ((data & 0x04) << LED_DATA_LINE2);
	newData |= ((data & 0x08) << LED_DATA_LINE3);
	newData |= ((data & 0x10) << LED_DATA_LINE4);
	newData |= ((data & 0x20) << LED_DATA_LINE5);
	newData |= ((data & 0x40) << LED_DATA_LINE6);
	newData |= ((data & 0x80) << LED_DATA_LINE7);

	DrvGPIO_SetPortBits(LED_DATA_PORT, newData);


}
