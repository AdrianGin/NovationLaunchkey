

#include "HardwareSpecific.h"

#include "LED.h"
#include "MultiplexControl.h"


int main(void)
{

	//Initialise Hardware
	SystemClockInit();
	


   MUX_GPIO_Init();
   LED_GPIO_Init();
   LED_Init();
   
   uint8_t i;
   uint8_t j = 0;


//   while(1)
//   {
//	   MUX_ActivateLineColumn(j++);
//
//
//	   //DrvGPIO_SetPortBits(LED_DATA_PORT, ~(1<<LED_DATA_LINE0) );
//	   for( i = 0 ; i < 8*3; i++ )
//	   {
//		   DrvGPIO_ClrBit(LED_DATA_PORT, LED_LINE_MAP[i]);
//		   DrvSYS_Delay(1000 * 1000);
//	   }
//   }


   LED_SetLEDBrightness(0, LED_PAD0_G, MAX_LED_BRIGHTNESS/10);
   LED_SetLEDBrightness(0, LED_PAD1_G, MAX_LED_BRIGHTNESS/4);
   LED_SetLEDBrightness(0, LED_PAD2_G, MAX_LED_BRIGHTNESS/2);
   LED_SetLEDBrightness(0, LED_PAD3_G, MAX_LED_BRIGHTNESS);

//   LED_SetLEDBrightness(0, LED_7SEG0_A, MAX_LED_BRIGHTNESS/10);
//   LED_SetLEDBrightness(0, LED_7SEG0_B, MAX_LED_BRIGHTNESS/4);
//   LED_SetLEDBrightness(0, LED_7SEG0_C, MAX_LED_BRIGHTNESS/2);
//   LED_SetLEDBrightness(0, LED_7SEG0_D, MAX_LED_BRIGHTNESS);

//   LED_SetLEDBrightness(0, LED_7SEG1_G, MAX_LED_BRIGHTNESS/10);
//   LED_SetLEDBrightness(0, LED_7SEG1_E, MAX_LED_BRIGHTNESS/4);
//   LED_SetLEDBrightness(0, LED_7SEG1_F, MAX_LED_BRIGHTNESS/2);
//   LED_SetLEDBrightness(0, LED_7SEG1_B, MAX_LED_BRIGHTNESS);

   LED_SetLEDBrightness(0, LED_PAD8_G, MAX_LED_BRIGHTNESS/10);
   LED_SetLEDBrightness(0, LED_PAD9_G, MAX_LED_BRIGHTNESS/4);
   LED_SetLEDBrightness(0, LED_PAD10_G, MAX_LED_BRIGHTNESS/2);
   LED_SetLEDBrightness(0, LED_PAD11_G, MAX_LED_BRIGHTNESS);


   LED_7Segment_Write(0, 0x07 );
   LED_7Segment_Write(1, 0x08);
   LED_7Segment_Write(2, 0x09 | LED_7SEG_DP);

   uint8_t column = 0;
   while(1)
   {
      MUX_ActivateLineColumn(column);
      uint16_t i;

      //for( i = 0; i < MAX_LED_BRIGHTNESS; i ++ )
      {
         LED_TimerRoutine(column);
      }
      column++;
      if( column > MAX_LINE_COLUMNS )
      {
         column = 0;
      }
      DrvSYS_Delay(20);
      LED_Blank();
      DrvSYS_Delay(10);
   }

	return 0;
}



















