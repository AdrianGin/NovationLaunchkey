

#include "ADC.h"
#include "MultiplexControl.h"

#include "LED.h"

const uint8_t ADC_SCANMAP[] = {ADC_INPUT0_PIN, ADC_INPUT1_PIN, ADC_INPUT2_PIN, ADC_INPUT3_PIN, ADC_INPUT4_PIN};

const uint8_t ADC_LOOKUP[] = 
{
		ADC_PAD_0,
		ADC_PAD_1,
		ADC_PAD_14,
		ADC_PAD_2,
		ADC_PAD_13,
		ADC_PAD_3,
		ADC_PAD_12,
		ADC_PAD_4,

		ADC_PAD_11,
		ADC_PAD_5,
		ADC_PAD_10,
		ADC_PAD_6,
		ADC_PAD_9,
		ADC_PAD_7,
		ADC_PAD_8,
		ADC_PAD_15,

		ADC_KNOB_2,
		ADC_KNOB_1,
		ADC_KNOB_0,
		ADC_KNOB_3,
		ADC_KNOB_4,
		ADC_KNOB_7,
		ADC_KNOB_5,
		ADC_KNOB_6,


		ADC_SLIDER_5,
		ADC_SLIDER_6,
		ADC_SLIDER_7,
		ADC_SLIDER_4,
		ADC_SLIDER_3,
		ADC_SLIDER_0,
		ADC_SLIDER_2,
		ADC_SLIDER_1,

		ADC_VOLUME,
		ADC_PITCHBEND,
		ADC_MODULATION,
		ADC_INVALID0,
		ADC_INVALID1,
		ADC_INVALID2,
		ADC_INVALID3,
		ADC_INVALID4,

};

volatile uint16_t RawADC[ADC_INPUT_COUNT];

typedef struct
{
	uint8_t 	writePtr;
	uint8_t		ready;
	uint16_t 	averages[ADC_OVERSAMPLING_COUNT];
	uint16_t	value;
	uint16_t    runningAverage;

} ADC_FilteredElement_t;

ADC_FilteredElement_t ADC_Filtered[ADC_INPUT_COUNT];

uint8_t ADCColumn;
uint8_t ADCStatus;

uint8_t ADC_GetCurrentColumn(void)
{
	return ADCColumn;
}

void ADC_SetNextColumn(void)
{
	ADCColumn++;
	if( ADCColumn >= MAX_ADC_COLUMNS )
	{
		ADCColumn = 0;
	}
	//Set the new column
	MUX_ActivateADCColumn(ADCColumn);
}

uint8_t ADC_IsFinishedSample(void)
{
	return ADCStatus;
}

/* ADC interrupt callback function */
void ADC_IntCallback(uint32_t u32UserData)
{
	uint8_t adcColumn = ADC_GetCurrentColumn();

	uint8_t i;
	for(i = 0; i < ADC_INPUTS_PER_COL ; i++)
	{
		int32_t adcSample = DrvADC_GetConversionData(ADC_SCANMAP[i]);
		uint8_t adcLookup = ADC_LOOKUP[(i * MAX_ADC_COLUMNS) + adcColumn];
		//RawADC[adcLookup] = (uint16_t)adcSample;

		ADC_ApplyFilter(adcLookup, adcSample);
	}

	ADCStatus = 1;
}


void ADC_Init(void)
{

	DrvGPIO_InitFunction(E_FUNC_ADC0);
	DrvGPIO_InitFunction(E_FUNC_ADC1);
	DrvGPIO_InitFunction(E_FUNC_ADC2);
	DrvGPIO_InitFunction(E_FUNC_ADC3);
	DrvGPIO_InitFunction(E_FUNC_ADC4);


	/* The maximum ADC clock rate is 16MHz */
	DrvADC_Open(ADC_SINGLE_END, ADC_SINGLE_CYCLE_OP, ADC_INPUT_PIN_MASK , EXTERNAL_12MHZ, 10);
	DrvADC_EnableADCInt(ADC_IntCallback, 0);
	MUX_ActivateADCColumn(0);

}

void ADC_StartConversion(void)
{
	ADCStatus = 0;
	DrvADC_StartConvert();
}



//u8ChannelNum is between 0 and ADC_INPUT_COUNT;
uint16_t ADC_GetSample(uint8_t u8ChannelNum)
{

	if(u8ChannelNum < ADC_INPUT_COUNT)
	{
		return ADC_Filtered[u8ChannelNum].value;
	}
	return 0;
}


void ADC_ApplyFilter(uint8_t index, uint16_t sample)
{
	ADC_FilteredElement_t* ele = &ADC_Filtered[index];

	//Do a running average, remove the oldest sample
	//recalculate the average
	uint8_t oldestWritePtr = (ele->writePtr + 1)  & ADC_OVERSAMPLING_MASK;

	ele->averages[ele->writePtr & ADC_OVERSAMPLING_MASK] = sample;

	ele->writePtr = oldestWritePtr;

	ele->runningAverage = ele->runningAverage - ele->averages[oldestWritePtr] + sample;

	if( (!ele->ready) && (oldestWritePtr == 0) )
	{
		ele->ready = ADC_READY;

	}

	if( ele->ready )
	{

		//tempValue is a 12bit value;
		uint16_t tempValue = ele->runningAverage >> (ADC_EFFECTIVE_RES-ADC_OUTPUT_RES);


		if( index == ADC_KNOB_7 )
		{
			//printNumber(tempValue);
			//printNumber(ele->runningAverage);

		}


		//Clamp the Max and Min
		if(ele->runningAverage >= ADC_MAX_VAL )
		{
			ele->value = ADC_OUTPUT_RANGE - 1;
		}

		if(ele->runningAverage <= ADC_MIN_VAL)
		{
			ele->value = 0;
		}

		uint16_t hiThreshold;
		uint16_t loThreshold;

		hiThreshold = ((tempValue+1) * ADC_STEP_SIZE);
		loThreshold = ((tempValue-1) * ADC_STEP_SIZE);

		//Use this to maximise the endpoints
		if(tempValue == (ADC_OUTPUT_RANGE - 2))
		{
			hiThreshold = ADC_MAX_VAL;
		}

		if(tempValue == (1))
		{
			loThreshold = ADC_MIN_VAL;
		}

		//Protect the endpoints from overflowing
		if( tempValue == 0 )
		{
			loThreshold = 0;
		}
		if( tempValue == (ADC_OUTPUT_RANGE - 1))
		{
			hiThreshold = ((1<<ADC_EFFECTIVE_RES)-1);
		}

		if( (ele->runningAverage > hiThreshold) )
		{
			ele->value = tempValue + 1;
		}

		if( (ele->runningAverage < loThreshold))
		{
			ele->value = tempValue - 1;
		}


	}


}









