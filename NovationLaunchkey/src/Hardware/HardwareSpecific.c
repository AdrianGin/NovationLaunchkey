


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



void printMsg(uint8_t* buffer)
{
	UART_TxByte(0xF0);
	UART_TxBuffer(buffer, strlen(buffer) );
	UART_TxByte(0xF7);
}

//Setup GPIO



