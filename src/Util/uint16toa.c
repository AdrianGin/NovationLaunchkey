/* File:   uint16toa.c
   Author: M. P. Hayes, UCECE
   Date:   15 May 2007
   Descr: 
*/

#include <stdint.h>

void uint16toa(uint16_t num, char *str, uint8_t leading_zeroes)
{
   uint16_t d;
   uint8_t i;
   const uint16_t powers[] = {10000, 1000, 100, 10, 1, 0};

   if(num == 0)
   {
      *str++ = '0';
	   *str = '\0';
      return;      
   }

	for (i = 0; (d = powers[i]); i++)
   {
		uint8_t q;
	
		q = num / d;
		if (leading_zeroes || q)
		{
	   	*str++ = '0' + q;
	   	num -= q * d;
	   	leading_zeroes = 1;
		}
	}
	
	*str = '\0';
}

/** Like uint16toa, but with leading zero's and 9999 is the largest
 * number input */
void float_4dp(uint16_t num, char *str)
{
   uint16_t d;
   uint8_t i;
   uint16_t const powers[] = {1000, 100, 10, 1, 0};

	for (i = 0; (d = powers[i]); i++)
   {
		uint8_t q;
	
		q = num / d;
	   *str++ = '0' + q;
	   num -= q * d;
	}
	
	*str = '\0';
}
