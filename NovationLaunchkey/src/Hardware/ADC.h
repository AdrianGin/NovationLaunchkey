#ifndef ADC_H
#define ADC_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#include "NUC1xx.h"
#include "DrvSys.h"
#include "DrvGPIO.h"

#include "DrvADC.h"

#include "HardwareSpecific.h"



typedef enum
{
	ADC_PAD_0 = 0,
	ADC_PAD_1	,
	ADC_PAD_2	,
	ADC_PAD_3	,
	ADC_PAD_4	,
	ADC_PAD_5	,
	ADC_PAD_6	,
	ADC_PAD_7	,
	ADC_PAD_8	,
	ADC_PAD_9	,
	ADC_PAD_10	,
	ADC_PAD_11	,
	ADC_PAD_12	,
	ADC_PAD_13	,
	ADC_PAD_14	,
	ADC_PAD_15	,

	ADC_KNOB_0	,
	ADC_KNOB_1	,
	ADC_KNOB_2	,
	ADC_KNOB_3	,
	ADC_KNOB_4	,
	ADC_KNOB_5	,
	ADC_KNOB_6	,
	ADC_KNOB_7	,

	ADC_SLIDER_0	,
	ADC_SLIDER_1	,
	ADC_SLIDER_2	,
	ADC_SLIDER_3	,
	ADC_SLIDER_4	,
	ADC_SLIDER_5	,
	ADC_SLIDER_6	,
	ADC_SLIDER_7	,

	ADC_VOLUME,
	ADC_PITCHBEND,
	ADC_MODULATION,
	ADC_INVALID0,
	ADC_INVALID1,
	ADC_INVALID2,
	ADC_INVALID3,
	ADC_INVALID4,

} E_ADC_INDEX;




#define ADC_RESOLUTION (12)
#define ADC_OVERSAMPLING_RES (0)
#define ADC_EFFECTIVE_RES (ADC_RESOLUTION+ADC_OVERSAMPLING_RES)

#define ADC_MULT_FACTOR	(100)

#define ADC_OUTPUT_RES	(8)
#define ADC_OUTPUT_RANGE (1<<ADC_OUTPUT_RES)

#define ADC_MAX_VAL		(4096)
#define ADC_MIN_VAL		(0)
#define ADC_RANGE		(ADC_MAX_VAL-ADC_MIN_VAL)

#define ADC_NOM_STEP_SIZE	(ADC_RANGE / (ADC_OUTPUT_RANGE) )

#define ADC_STEP_SIZE	(((ADC_RANGE-ADC_NOM_STEP_SIZE)*ADC_MULT_FACTOR) / (ADC_OUTPUT_RANGE-2))

#define ADC_THRESHOLD	(ADC_STEP_SIZE/2)


#define ADC_OVERSAMPLING_COUNT (1<<ADC_OVERSAMPLING_RES)
#define ADC_OVERSAMPLING_MASK (ADC_OVERSAMPLING_COUNT-1)
#define ADC_READY (1)


typedef struct
{
	uint16_t lastValue;
	uint16_t filteredVal;
} ADC_FilteredElement_t;


extern ADC_FilteredElement_t ADC_Filtered[];

void ADC_Init(void);
void ADC_IntCallback(uint32_t u32UserData);
uint16_t ADC_GetSample(uint8_t u8ChannelNum);
uint16_t ADC_GetRawSample(uint8_t u8ChannelNum);

void ADC_StartConversion(void);
uint8_t ADC_GetCurrentColumn(void);
void ADC_SetNextColumn(void);
uint8_t ADC_IsFinishedSampling(void);
void ADC_ApplyFilter(uint8_t index, uint16_t sample);

uint8_t ADC_GenericProcessing(uint8_t index, uint16_t sample);

void ADC_ClearChangeFlag(uint8_t index);
uint8_t ADC_GetChangeFlag(uint8_t index);


#ifdef __cplusplus
}
#endif

#endif
