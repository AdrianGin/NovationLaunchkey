

#include "UART.h"
#include <string.h>
#include <stdint.h>

#include "LED.h"

#define UART_TX_BUFFER_SIZE (32)
#define UART_TX_BUFFER_MASK  (UART_TX_BUFFER_SIZE-1)

#define UARTPORT			(UART_PORT1)
#define UARTPORT_DEF		(UART1)

volatile uint8_t UART_TxBuf[UART_TX_BUFFER_SIZE];
uint8_t UART_TxWritePtr;
volatile uint8_t UART_TxReadPtr;



uint8_t UART_TxQueuedBytes(void)
{
	if(UART_TxWritePtr >= UART_TxReadPtr)
	{
		return UART_TxWritePtr - UART_TxReadPtr;
	}
	else
	{
		return UART_TX_BUFFER_SIZE - (UART_TxReadPtr - UART_TxWritePtr);
	}

}

/*---------------------------------------------------------------------------------------------------------*/
/* UART Callback function                                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/
void UART_INT_HANDLE(uint32_t u32IntStatus)
{
	if(u32IntStatus & DRVUART_THREINT)
	{
		if( UART_TxQueuedBytes() )
		{
			UARTPORT_DEF->DATA = UART_TxBuf[UART_TxReadPtr];
			UART_TxReadPtr = (UART_TxReadPtr + 1) & UART_TX_BUFFER_MASK;
		}
		else
		{
			DrvUART_DisableInt(UARTPORT, DRVUART_THREINT);
		}
	}
}



void UART_Init(void)
{
	/* Set UART Pin */
	DrvSYS_SelectIPClockSource(E_SYS_UART_CLKSRC,0);
	DrvGPIO_InitFunction(E_FUNC_UART1_RX_TX);
}



void UART_TxBuffer(uint8_t* buffer, uint8_t size)
{

	while(size--)
	{
		UART_TxByte( *buffer++ );
	}
}

void UART_TxByte(uint8_t byte)
{

	//while( UART_TxQueuedBytes() == UART_TX_BUFFER_MASK );

	UART_TxBuf[UART_TxWritePtr] = byte;
	UART_TxWritePtr = (UART_TxWritePtr + 1) & UART_TX_BUFFER_MASK;
	DrvUART_EnableInt(UARTPORT, (DRVUART_THREINT), UART_INT_HANDLE);
	
}
