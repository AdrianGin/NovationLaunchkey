


#include "NUC1xx.h"
#include "DrvSys.h"

#include "UART.h"
#include <string.h>

//Setup Clocks

void SystemClockInit(void)
{
   UNLOCKREG();

   DrvSYS_SetOscCtrl(E_SYS_XTL12M, ENABLE);
   DrvSYS_Delay(5000);
   DrvSYS_Open(50000000);



   /* Enable PLL */
   //DrvSYS_SetPLLMode(0);

   /* Switch to PLL clock */
  // DrvSYS_SelectHCLKSource(2);

   /* Update system core clock */
   //SystemCoreClockUpdate();
}


void NVIC_Config(void)
{
	NVIC_SetPriority(TMR0_IRQn,  2);
	NVIC_SetPriority(ADC_IRQn,   3);
	NVIC_SetPriority(UART1_IRQn, 1);
}


void printMsg(uint8_t* buffer)
{
	UART_TxByte(0xF0);
	UART_TxBuffer(buffer, strlen((const char*)buffer) );
	UART_TxByte(0xF7);
}

void printNumber(uint16_t number)
{
	uint16_t temp;
	uint16_t divisor = 10000;

	UART_TxByte(0xF0);

	while(divisor)
	{
		UART_TxByte(number / divisor );
		if( number >= divisor )
		{
			temp = (number / divisor);
			number = number - (temp * divisor);
		}
		divisor = divisor / 10;

	}
	
	UART_TxByte(0xF7);	
}

//Setup GPIO



