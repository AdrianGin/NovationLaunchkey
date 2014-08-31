

#include "Test_Rescale.h"
#include <stdio.h>

typedef struct
{
	uint16_t  testInput;
	uint16_t  testSize;
	const Rescale_t*  testArray;
	uint16_t  testOutput;
} RescaleTestElement_t;

//Positive Gradient
static const Rescale_t TEST_ARRAY1 = {0, 255, 0, 255};
static const Rescale_t TEST_ARRAY2 = {0, 255, 0, 127};
static const Rescale_t TEST_ARRAY3 = {0, 255, 0, 128};

//Compression
static const Rescale_t TEST_ARRAY4 = {0, 255, 127, 255};

//Pass Thru
static const Rescale_t TEST_ARRAY5 = {127, 255, 127, 255};

//Expansion
static const Rescale_t TEST_ARRAY6 = {127, 255, 0, 255};

//Translation
static const Rescale_t TEST_ARRAY7 = {127, 255, 0, 128};



//Negative Gradient
static const Rescale_t TEST_ARRAY8 = {0, 255, 255, 0};

//Not supported
static const Rescale_t TEST_ARRAY9 = {255, 0, 255, 0};

//Negative Compressed Gradient
static const Rescale_t TEST_ARRAY10 = {0, 255, 127, 0};

//Negative Compressed Gradient + Offset
static const Rescale_t TEST_ARRAY11 = {0, 255, 127, 64};

//Negative Expanded Gradient
static const Rescale_t TEST_ARRAY12 = {0, 127, 255, 0};

//Negative Expanded Gradient + Offset
static const Rescale_t TEST_ARRAY13 = {0, 127, 127, 64};

//Negative Expanded Gradient (with input offset)
static const Rescale_t TEST_ARRAY14 = {127, 255, 255, 0};

//Negative Expanded Gradient + Offset (input offset)
static const Rescale_t TEST_ARRAY15 = {127, 255, 127, 64};

//Zero Output
static const Rescale_t TEST_ARRAY16 = {0, 255, 0, 0};

//Compression
static const Rescale_t TEST_ARRAY17 = {0, 255, 64, 127};

// y = Ax + B
// Formula on Calculator goes: (Val-xMin)/(xRange) * (yRange)
static RescaleTestElement_t TestCases[] =
{
		{0,   1, 	&TEST_ARRAY1, 0},
		{1,   1, 	&TEST_ARRAY1, 1},
		{255, 1, 	&TEST_ARRAY1, 255},
		{127, 1, 	&TEST_ARRAY1, 127},

		{0,   1, 	&TEST_ARRAY2, 0},
		{1,   1, 	&TEST_ARRAY2, 0},
		{2,   1, 	&TEST_ARRAY2, 0},
		{3,   1, 	&TEST_ARRAY2, 1},
		{255, 1, 	&TEST_ARRAY2, 127},
		{127, 1, 	&TEST_ARRAY2, 63},

		{0,   1, 	&TEST_ARRAY3, 0},
		{1,   1, 	&TEST_ARRAY3, 0},
		{2,   1, 	&TEST_ARRAY3, 1},
		{3,   1, 	&TEST_ARRAY3, 1},
		{255, 1, 	&TEST_ARRAY3, 128},
		{127, 1, 	&TEST_ARRAY3, 63},

		{0,   1, 	&TEST_ARRAY4, 127},
		{1,   1, 	&TEST_ARRAY4, 127},
		{2,   1, 	&TEST_ARRAY4, 128},
		{3,   1, 	&TEST_ARRAY4, 128},
		{255, 1, 	&TEST_ARRAY4, 255},
		{127, 1, 	&TEST_ARRAY4, 190}, //Note down rounding


		{0,   1, 	&TEST_ARRAY5, 127},
		{1,   1, 	&TEST_ARRAY5, 127},
		{2,   1, 	&TEST_ARRAY5, 127},
		{3,   1, 	&TEST_ARRAY5, 127},
		{255, 1, 	&TEST_ARRAY5, 255},
		{127, 1, 	&TEST_ARRAY5, 127},
		{180, 1, 	&TEST_ARRAY5, 180},


		{0,   1, 	&TEST_ARRAY6, 0},
		{1,   1, 	&TEST_ARRAY6, 0},
		{2,   1, 	&TEST_ARRAY6, 0},
		{3,   1, 	&TEST_ARRAY6, 0},
		{255, 1, 	&TEST_ARRAY6, 255},
		{127, 1, 	&TEST_ARRAY6, 0},
		{179, 1, 	&TEST_ARRAY6, 103},
		{180, 1, 	&TEST_ARRAY6, 105},
		{191, 1, 	&TEST_ARRAY6, 127},


		{0,   1, 	&TEST_ARRAY7, 0},
		{1,   1, 	&TEST_ARRAY7, 0},
		{2,   1, 	&TEST_ARRAY7, 0},
		{3,   1, 	&TEST_ARRAY7, 0},
		{255, 1, 	&TEST_ARRAY7, 128},
		{127, 1, 	&TEST_ARRAY7, 0},
		{128, 1, 	&TEST_ARRAY7, 1},
		{179, 1, 	&TEST_ARRAY7, 52},
		{180, 1, 	&TEST_ARRAY7, 53},
		{191, 1, 	&TEST_ARRAY7, 64},

		{0,   1, 	&TEST_ARRAY8, 255},
		{1,   1, 	&TEST_ARRAY8, 254},
		{255, 1, 	&TEST_ARRAY8, 0},
		{127, 1, 	&TEST_ARRAY8, 128},

//		{0,   1, 	&TEST_ARRAY9, 0},
//		{1,   1, 	&TEST_ARRAY9, 1},
//		{255, 1, 	&TEST_ARRAY9, 255},
//		{127, 1, 	&TEST_ARRAY9, 127},

		{0,   1, 	&TEST_ARRAY10, 127},
		{1,   1, 	&TEST_ARRAY10, 126},
		{2,   1, 	&TEST_ARRAY10, 126},
		{3,   1, 	&TEST_ARRAY10, 125},
		{255, 1, 	&TEST_ARRAY10, 0},
		{127, 1, 	&TEST_ARRAY10, 63},
		{128, 1, 	&TEST_ARRAY10, 63},
		{179, 1, 	&TEST_ARRAY10, 37},
		{180, 1, 	&TEST_ARRAY10, 37},
		{191, 1, 	&TEST_ARRAY10, 31},

		{0,   1, 	&TEST_ARRAY11, 127},
		{1,   1, 	&TEST_ARRAY11, 126},
		{2,   1, 	&TEST_ARRAY11, 126},
		{3,   1, 	&TEST_ARRAY11, 126},
		{255, 1, 	&TEST_ARRAY11, 64},
		{127, 1, 	&TEST_ARRAY11, 95},
		{128, 1, 	&TEST_ARRAY11, 95},
		{179, 1, 	&TEST_ARRAY11, 82},
		{180, 1, 	&TEST_ARRAY11, 82},
		{191, 1, 	&TEST_ARRAY11, 79},

		{0,   1, 	&TEST_ARRAY12, 255},
		{1,   1, 	&TEST_ARRAY12, 252},
		{2,   1, 	&TEST_ARRAY12, 250},
		{3,   1, 	&TEST_ARRAY12, 248},
		{64,   1, 	&TEST_ARRAY12, 126},
		{255, 1, 	&TEST_ARRAY12, 0},
		{127, 1, 	&TEST_ARRAY12, 0},
		{128, 1, 	&TEST_ARRAY12, 0},
		{179, 1, 	&TEST_ARRAY12, 0},
		{180, 1, 	&TEST_ARRAY12, 0},
		{191, 1, 	&TEST_ARRAY12, 0},

		{0,   1, 	&TEST_ARRAY13, 127},
		{1,   1, 	&TEST_ARRAY13, 126},
		{2,   1, 	&TEST_ARRAY13, 126},
		{3,   1, 	&TEST_ARRAY13, 125},
		{64,   1, 	&TEST_ARRAY13, 95},
		{255, 1, 	&TEST_ARRAY13, 64},
		{127, 1, 	&TEST_ARRAY13, 64},
		{128, 1, 	&TEST_ARRAY13, 64},
		{179, 1, 	&TEST_ARRAY13, 64},
		{180, 1, 	&TEST_ARRAY13, 64},
		{191, 1, 	&TEST_ARRAY13, 64},

		{0,   1, 	&TEST_ARRAY14, 255},
		{1,   1, 	&TEST_ARRAY14, 255},
		{2,   1, 	&TEST_ARRAY14, 255},
		{3,   1, 	&TEST_ARRAY14, 255},
		{64,   1, 	&TEST_ARRAY14, 255},
		{255, 1, 	&TEST_ARRAY14, 0},
		{127, 1, 	&TEST_ARRAY14, 255},
		{128, 1, 	&TEST_ARRAY14, 253},
		{179, 1, 	&TEST_ARRAY14, 151},
		{180, 1, 	&TEST_ARRAY14, 149},
		{191, 1, 	&TEST_ARRAY14, 127},

		{0,   1, 	&TEST_ARRAY15, 127},
		{1,   1, 	&TEST_ARRAY15, 127},
		{2,   1, 	&TEST_ARRAY15, 127},
		{3,   1, 	&TEST_ARRAY15, 127},
		{64,   1, 	&TEST_ARRAY15, 127},
		{255, 1, 	&TEST_ARRAY15, 64},
		{127, 1, 	&TEST_ARRAY15, 127},
		{128, 1, 	&TEST_ARRAY15, 126},
		{179, 1, 	&TEST_ARRAY15, 101},
		{180, 1, 	&TEST_ARRAY15, 100},
		{191, 1, 	&TEST_ARRAY15, 95},

		{0,   1, 	&TEST_ARRAY16, 0},
		{1,   1, 	&TEST_ARRAY16, 0},
		{2,   1, 	&TEST_ARRAY16, 0},
		{3,   1, 	&TEST_ARRAY16, 0},
		{64,   1, 	&TEST_ARRAY16, 0},
		{255, 1, 	&TEST_ARRAY16, 0},
		{127, 1, 	&TEST_ARRAY16, 0},
		{128, 1, 	&TEST_ARRAY16, 0},
		{179, 1, 	&TEST_ARRAY16, 0},
		{180, 1, 	&TEST_ARRAY16, 0},
		{191, 1, 	&TEST_ARRAY16, 0},

		{0,   1, 	&TEST_ARRAY17, 64},
		{1,   1, 	&TEST_ARRAY17, 64},
		{255, 1, 	&TEST_ARRAY17, 127},
		{127, 1, 	&TEST_ARRAY17, 95},

};

void Test_RescalePrintData(Rescale_t* filter)
{

	uint32_t grad;
	int32_t offset;
	uint16_t newVal;

	uint16_t dy = filter->yMax - filter->yMin;
	uint16_t dx = filter->xMax - filter->xMin;

	grad = (dy * RESACLE_MULT_FACTOR) / dx;
	offset = (filter->yMax * RESACLE_MULT_FACTOR) - (filter->xMax * grad);

	printf("\nRescale Element Grad=%d, Offset=%d\n", grad, offset );
}

int Test_Rescale(void)
{
    int i;
    int failedCount = 0;
    
    for( i = 0; i < sizeof(TestCases)/sizeof(RescaleTestElement_t); i++)
    {
    	Test_RescalePrintData((Rescale_t*)TestCases[i].testArray);

    	uint16_t outputVal;
    	outputVal = Rescale_Apply( (Rescale_t*)TestCases[i].testArray, TestCases[i].testInput);
    	
    	printf("Performing Test Case[%d], Input:%d, Output:%d\n", i, TestCases[i].testInput, TestCases[i].testOutput);
    	
    	if( outputVal != TestCases[i].testOutput )
    	{
            printf("Test_Rescale Case[%d] Failed! Expected Output: %d, Received: %d\n", i, TestCases[i].testOutput, outputVal);
            failedCount++;
        }
        else
        {
            printf("Test_Rescale Case[%d] Success!\n", i);
        }
    }
    
    printf("Test_Rescale Summary: Performed: %d, Success: %d :: Failed: %d\n", i, i-failedCount, failedCount);

    if( failedCount )
    {
        return TEST_FAILED;
    }
    
    return TEST_SUCCESS;
}
