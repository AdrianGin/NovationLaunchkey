#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif


#include "DrvSys.h"
#include "DrvUART.h"
#include "DrvGPIO.h"
#include <stdint.h>

void UART_INT_HANDLE(uint32_t u32IntStatus);

void UART_Init(void);
uint8_t UART_TxQueuedBytes(void);
void UART_TxBuffer(uint8_t* buffer, uint8_t size);
void UART_TxByte(uint8_t byte);

//Setup GPIO

#ifdef __cplusplus
}
#endif

#endif
