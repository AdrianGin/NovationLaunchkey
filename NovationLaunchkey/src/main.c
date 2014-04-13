

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
   LED_SetLEDBrightness(0, LED_PAD6_G, MAX_LED_BRIGHTNESS/70);
   LED_SetLEDBrightness(0, LED_PAD7_G, MAX_LED_BRIGHTNESS/60);

   LED_SetLEDBrightness(0, LED_PAD8_G, MAX_LED_BRIGHTNESS/50);
   LED_SetLEDBrightness(0, LED_PAD9_G, MAX_LED_BRIGHTNESS/40);
   LED_SetLEDBrightness(0, LED_PAD10_G, MAX_LED_BRIGHTNESS/30);
   LED_SetLEDBrightness(0, LED_PAD11_G, MAX_LED_BRIGHTNESS/20);

   LED_SetLEDBrightness(0, LED_PAD12_G, MAX_LED_BRIGHTNESS/10);
   LED_SetLEDBrightness(0, LED_PAD13_G, MAX_LED_BRIGHTNESS/5);
   LED_SetLEDBrightness(0, LED_PAD14_G, MAX_LED_BRIGHTNESS/2);
   LED_SetLEDBrightness(0, LED_PAD15_G, MAX_LED_BRIGHTNESS);


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
      if( column >= 8 )
      {
         column = 0;
      }
      DrvSYS_Delay(16);
      LED_Blank();
      DrvSYS_Delay(2);
   }

	return 0;
}



















