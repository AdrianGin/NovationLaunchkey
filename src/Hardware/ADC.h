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
#include "HAL_ADC.h"



typedef struct
{
	 union {
        uint32_t ADC_STATUS;
        struct {
        	uint32_t	ADC0	:1;
        	uint32_t	ADC1	:1;
        	uint32_t	ADC2	:1;
        	uint32_t	ADC3	:1;
        	uint32_t	ADC4	:1;
        	uint32_t	ADC5	:1;
        	uint32_t	ADC6	:1;
        	uint32_t	ADC7	:1;

        	uint32_t	ADC8	:1;
        	uint32_t	ADC9	:1;
        	uint32_t	ADC10	:1;
        	uint32_t	ADC11	:1;
        	uint32_t	ADC12	:1;
        	uint32_t	ADC13	:1;
        	uint32_t	ADC14	:1;
        	uint32_t	ADC15	:1;

        	uint32_t	ADC16	:1;
        	uint32_t	ADC17	:1;
        	uint32_t	ADC18	:1;
        	uint32_t	ADC19	:1;
        	uint32_t	ADC20	:1;
        	uint32_t	ADC21	:1;
        	uint32_t	ADC22	:1;
        	uint32_t	ADC23	:1;

        	uint32_t	ADC24	:1;
        	uint32_t	ADC25	:1;
        	uint32_t	ADC26	:1;
        	uint32_t	ADC27	:1;
        	uint32_t	ADC28	:1;
        	uint32_t	ADC29	:1;
        	uint32_t	ADC30	:1;
        	uint32_t	ADC31	:1;
        } ADC_GROUP1;
    };

	union {
		uint32_t ADC_STATUS2;
		struct {
			uint32_t	ADC32	:1;
			uint32_t	ADC33	:1;
			uint32_t	ADC34	:1;
			uint32_t	ADC35	:1;
			uint32_t	ADC36	:1;
			uint32_t	ADC37	:1;
			uint32_t	ADC38	:1;
			uint32_t	ADC39	:1;
		} ADC_GROUP2;
	};

} ADC_ChangeStatus_t;

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
