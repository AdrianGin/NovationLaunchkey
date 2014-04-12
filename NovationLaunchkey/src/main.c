

#include "HardwareSpecific.h"

#include "LED.h"
#include "MultiplexControl.h"


int main(void)
{

	//Initialise Hardware
	SystemClockInit();
	


   MUX_GPIO_Init();
   LED_GPIO_Init();

   
   uint8_t i;
   uint8_t j;


//   while(1)
//   {
//	   MUX_ActivateLineColumn(4);
//
//
//	   //DrvGPIO_SetPortBits(LED_DATA_PORT, ~(1<<LED_DATA_LINE0) );
//
//	   DrvGPIO_ClrBit(LED_DATA_PORT, LED_DATA_LINE0);
//      DrvGPIO_ClrBit(LED_DATA_PORT, LED_DATA_LINE1);
//
//
//	   DrvSYS_Delay(1000 * 1000);
//   }


   while(1)
   {

      for( i = 0; i < PAD_COUNT * 2 ; i ++ )
      {
         //MUX_ActivateLineColumn(j);
         uint8_t column;
         uint8_t row;

         column = LED_PAD_SEQUENCE[i] / LEDS_PER_COLUMN;
         row =    LED_PAD_SEQUENCE[i] & (LEDS_PER_COLUMN - 1);
         
         MUX_ActivateLineColumn(column);
         LED_SetData( ~(1<<row) );
         DrvSYS_Delay(10*1000);

      }
     }
      
   {

      for( j = 0; j < MAX_LINE_COLUMNS; j++ )
      {
         MUX_ActivateLineColumn(j);

         for(i = 0; i < 8; i++ )
         {
            LED_SetData( ~(1<<i) );
            DrvSYS_Delay(10*1000);
         }
      }
   }

	return 0;
}



















