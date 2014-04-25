

#include "ADC.h"
#include <stdlib.h>

uint8_t FSR_Processing(uint8_t index, uint16_t sample)
{
	ADC_FilteredElement_t* ele = &ADC_Filtered[index];

	int32_t signedValue;
	uint8_t changedFlag = 0;

	signedValue = sample;
	if( signedValue < 100 )
	{
		signedValue = 0;
	}


	//Need to cutoff the bottom;
	if(  (signedValue < (ADC_MAX_VAL-(ADC_NOM_STEP_SIZE/2))) &&
		 (signedValue > (ADC_MIN_VAL+(ADC_NOM_STEP_SIZE/2))) )
	{

		if( abs(signedValue - ele->lastValue) >= (ADC_NOM_STEP_SIZE/2) )
		{
			ele->filteredVal = (((int32_t)(signedValue-(ADC_NOM_STEP_SIZE/2)) * ADC_MULT_FACTOR) ) / ADC_STEP_SIZE;
			ele->filteredVal = ele->filteredVal + 1;
			changedFlag = 1;
		}
	}
	else
	{

		if( abs(signedValue - ele->lastValue) >= (ADC_NOM_STEP_SIZE/4) )
		{
			ele->filteredVal = (signedValue) / ADC_NOM_STEP_SIZE;
			changedFlag = 1;
		}
	}

	return changedFlag;
}




