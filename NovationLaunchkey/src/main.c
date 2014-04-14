

#include "HardwareSpecific.h"

#include "ADC.h"
#include "LED.h"
#include "MultiplexControl.h"
#include "UART.h"
#include "Timer.h"

int main(void)
{

	//Initialise Hardware
	SystemClockInit();
	


	MUX_GPIO_Init();
	LED_GPIO_Init();
	UART_Init();

	STR_UART_T sParam;
	/* UART Setting */
	sParam.u32BaudRate 		= 31250;
	sParam.u8cDataBits 		= DRVUART_DATABITS_8;
	sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
	sParam.u8cParity 		= DRVUART_PARITY_NONE;
	sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	/* Set UART Configuration */
	if( DrvUART_Open(UART_PORT1,&sParam) == E_SUCCESS)
	{
		DrvUART_EnableInt(UART_PORT1, DRVUART_THREINT, UART_INT_HANDLE);
	}


	LED_Init();
   
	Timer_Init();
	ADC_Init();
   
   uint8_t i;
   uint8_t j = 0;

//   LED_SetLEDBrightness(0, LED_PAD0_G, MAX_LED_BRIGHTNESS/10);
//   LED_SetLEDBrightness(0, LED_PAD1_G, MAX_LED_BRIGHTNESS/4);
//   LED_SetLEDBrightness(0, LED_PAD2_G, MAX_LED_BRIGHTNESS/2);
//   LED_SetLEDBrightness(0, LED_PAD3_G, MAX_LED_BRIGHTNESS);
//
//   LED_SetLEDBrightness(0, LED_PAD4_R, MAX_LED_BRIGHTNESS/10);
//   LED_SetLEDBrightness(0, LED_PAD5_R, MAX_LED_BRIGHTNESS/4);
//   LED_SetLEDBrightness(0, LED_PAD6_R, MAX_LED_BRIGHTNESS/2);
//   LED_SetLEDBrightness(0, LED_PAD7_R, MAX_LED_BRIGHTNESS);
//
//   LED_SetLEDBrightness(0, LED_PAD8_R, MAX_LED_BRIGHTNESS/10);
//   LED_SetLEDBrightness(0, LED_PAD9_R, MAX_LED_BRIGHTNESS/4);
//   LED_SetLEDBrightness(0, LED_PAD10_R, MAX_LED_BRIGHTNESS/2);
//   LED_SetLEDBrightness(0, LED_PAD11_R, MAX_LED_BRIGHTNESS);
//
//   LED_SetLEDBrightness(0, LED_PAD8_G, MAX_LED_BRIGHTNESS/10);
//   LED_SetLEDBrightness(0, LED_PAD9_G, MAX_LED_BRIGHTNESS/4);
//   LED_SetLEDBrightness(0, LED_PAD10_G, MAX_LED_BRIGHTNESS/2);
//   LED_SetLEDBrightness(0, LED_PAD11_G, MAX_LED_BRIGHTNESS);
//
//   LED_SetLEDBrightness(0, LED_PAD12_G, MAX_LED_BRIGHTNESS/20);
//   LED_SetLEDBrightness(0, LED_PAD13_G, MAX_LED_BRIGHTNESS/10);
//   LED_SetLEDBrightness(0, LED_PAD14_G, MAX_LED_BRIGHTNESS/5);
//   LED_SetLEDBrightness(0, LED_PAD15_G, MAX_LED_BRIGHTNESS);

   LED_SetLEDBrightness(0, LED_PAD0_G, MAX_LED_BRIGHTNESS/200);
   LED_SetLEDBrightness(0, LED_PAD1_G, MAX_LED_BRIGHTNESS/150);
   LED_SetLEDBrightness(0, LED_PAD2_G, MAX_LED_BRIGHTNESS/110);
   LED_SetLEDBrightness(0, LED_PAD3_G, MAX_LED_BRIGHTNESS/100);

   LED_SetLEDBrightness(0, LED_PAD4_G, MAX_LED_BRIGHTNESS/90);
   LED_SetLEDBrightness(0, LED_PAD5_G, MAX_LED_BRIGHTNESS/80);
//   LED_SetLEDBrightness(0, LED_PAD6_G, MAX_LED_BRIGHTNESS/70);


   LED_SetLEDBrightness(0, LED_PAD8_G, MAX_LED_BRIGHTNESS/50);
   LED_SetLEDBrightness(0, LED_PAD9_G, MAX_LED_BRIGHTNESS/40);
   LED_SetLEDBrightness(0, LED_PAD10_G, MAX_LED_BRIGHTNESS/30);
   LED_SetLEDBrightness(0, LED_PAD11_G, MAX_LED_BRIGHTNESS/20);

   LED_SetLEDBrightness(0, LED_PAD12_G, MAX_LED_BRIGHTNESS/10);
   LED_SetLEDBrightness(0, LED_PAD13_G, MAX_LED_BRIGHTNESS/5);
   LED_SetLEDBrightness(0, LED_PAD14_G, MAX_LED_BRIGHTNESS/2);
   LED_SetLEDBrightness(0, LED_PAD15_G, MAX_LED_BRIGHTNESS);



   uint8_t column = 0;
   uint16_t adcSample;

   uint16_t adcCounter = 0;

   while(1)
   {
      MUX_ActivateLineColumn(column);
      uint16_t i;

      //for( i = 0; i < MAX_LED_BRIGHTNESS; i ++ )
      {
         LED_TimerRoutine(column);
      }
      column++;
      if( column >= MAX_LINE_COLUMNS )
      {
         column = 0;
      }
      DrvSYS_Delay(16);
      LED_Blank();
      DrvSYS_Delay(2);

      if( adcCounter++ >= 200 )
      {

			adcSample = ADC_GetSample(ADC_KNOB_7);
			adcSample = adcSample >> (ADC_OUTPUT_RES-8);
			LED_7Segment_WriteNumber(adcSample);

			DrvSYS_Delay(200);
			//DrvADC_StartConvert();

			adcCounter = 0;
      }

   }

	return 0;
}



















