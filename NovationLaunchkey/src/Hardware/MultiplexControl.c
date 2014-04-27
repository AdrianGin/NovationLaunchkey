

#include "MultiplexControl.h"

volatile uint8_t PrimaryMuxColumn = 0;

uint8_t MUX_GetCurrentColumn(void)
{
	return PrimaryMuxColumn;
}


void MUX_GPIO_Init(void)
{

	DrvGPIO_InitFunction(E_FUNC_GPIO);

	DrvGPIO_Open(MUX_LINE_PORT, MUX_LINE0_PIN, E_IO_OUTPUT);
	DrvGPIO_Open(MUX_LINE_PORT, MUX_LINE1_PIN, E_IO_OUTPUT);
	DrvGPIO_Open(MUX_LINE_PORT, MUX_LINE2_PIN, E_IO_OUTPUT);

	DrvGPIO_Open(MUX_ADC_PORT, MUX_ADC0_PIN, E_IO_OUTPUT);
	DrvGPIO_Open(MUX_ADC_PORT, MUX_ADC1_PIN, E_IO_OUTPUT);
	DrvGPIO_Open(MUX_ADC_PORT, MUX_ADC2_PIN, E_IO_OUTPUT);

	DrvGPIO_DisableDigitalInputBit(MUX_LINE_PORT, MUX_LINE0_PIN);
	DrvGPIO_DisableDigitalInputBit(MUX_LINE_PORT, MUX_LINE1_PIN);
	DrvGPIO_DisableDigitalInputBit(MUX_LINE_PORT, MUX_LINE2_PIN);

	DrvGPIO_DisableDigitalInputBit(MUX_ADC_PORT, MUX_ADC0_PIN);
	DrvGPIO_DisableDigitalInputBit(MUX_ADC_PORT, MUX_ADC1_PIN);
	DrvGPIO_DisableDigitalInputBit(MUX_ADC_PORT, MUX_ADC2_PIN);

}



void MUX_ActivateLineColumn(uint8_t column)
{

	int32_t newData = 0;
	if(column >= MAX_LINE_COLUMNS )
	{
		return;
	}

	//newData = DrvGPIO_GetPortDoutBits(MUX_LINE_PORT) & (~MUX_LINE_PIN_MASK);

	newData |= ( ((column & 0x01)  ? 1 : 0)  << MUX_LINE0_PIN);
	newData |= ( ((column & 0x02)  ? 1 : 0)  << MUX_LINE1_PIN);
	newData |= ( ((column & 0x04)  ? 1 : 0)  << MUX_LINE2_PIN);

	GPIO_T * tGPIO;
	tGPIO = (GPIO_T *)(MUX_LINE_GPIO_PORT);
	tGPIO->DOUT &= (~MUX_LINE_PIN_MASK);
	tGPIO->DOUT |= newData;

	PrimaryMuxColumn = column;
	//DrvGPIO_SetPortBits(MUX_LINE_PORT, newData);

}



void MUX_ActivateADCColumn(uint8_t column)
{
	int32_t newData = 0;
	if(column >= MAX_ADC_COLUMNS )
	{
		return;
	}

	//DrvGPIO_SetPortMask(MUX_ADC_PORT, ~((1<<MUX_ADC0_PIN) | (1<<MUX_ADC1_PIN) | (1<<MUX_ADC2_PIN)) );

	newData |= ( ((column & 0x01)  ? 1 : 0)  << MUX_ADC0_PIN);
	newData |= ( ((column & 0x02)  ? 1 : 0)  << MUX_ADC1_PIN);
	newData |= ( ((column & 0x04)  ? 1 : 0)  << MUX_ADC2_PIN);

	GPIO_T * tGPIO;
	tGPIO = (GPIO_T *)(MUX_ADC_GPIO_PORT);
	tGPIO->DOUT &= (~MUX_ADC_PIN_MASK);
	tGPIO->DOUT |= newData;


}




