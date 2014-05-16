

#include "Test_BinarySearch.h"


typedef struct
{
	uint16_t  testInput;
	uint16_t  testSize;
	const uint16_t*  testArray;
	uint16_t  testOutput;
} binarySearchTestElement_t;


const uint16_t TEST_ARRAY1[] = {0};
const uint16_t TEST_ARRAY2[] = {65535};
const uint16_t TEST_ARRAY3[] = {0, 65535};
const uint16_t TEST_ARRAY4[] = {0, 1, 65535};
const uint16_t TEST_ARRAY5[] = {0, 65534, 65535};
const uint16_t TEST_ARRAY6[] = {0, 1, 65534, 65535};
const uint16_t TEST_ARRAY7[] = {0, 1, 500, 65534, 65535};

const uint16_t TEST_ARRAY8[] = {0, 0, 0, 0, 0};
const uint16_t TEST_ARRAY9[] = {0, 0, 0, 0, 1};


const uint16_t TEST_ARRAY10[] = {32, 32, 32, 32, 32};
const uint16_t TEST_ARRAY10a[] = {31, 32, 32, 32, 32};
const uint16_t TEST_ARRAY11[] = {32, 32, 32, 32, 33};

//incorrectly sorted array
const uint16_t TEST_ARRAY12[] = {33, 30, 20, 10, 0};
const uint16_t TEST_ARRAY13[] = {33, 30, 20, 10, 64};


binarySearchTestElement_t TestCases[] =
{
		{65535, 1, 	0, 0},

		//{0}
		{0, 	1,	TEST_ARRAY1, 0},
		{500, 	1,	TEST_ARRAY1, 0},

		//{65535};
		{0,   	1,	TEST_ARRAY2, 0},
		{500, 	1,	TEST_ARRAY2, 0},
		{65535, 1,	TEST_ARRAY2, 0},

		//{0, 65535};
		{0, 	2,	TEST_ARRAY3, 0},
		{8080, 	2,	TEST_ARRAY3, 0},
		{65535, 2,	TEST_ARRAY3, 1},
		
		//{0, 1, 65535};
		{0, 	3,	TEST_ARRAY4, 0},
		{1, 	3,	TEST_ARRAY4, 1},
		{200, 	3,	TEST_ARRAY4, 1},
		{65534, 3,	TEST_ARRAY4, 1},
		{65535, 3,	TEST_ARRAY4, 2},

		//{0, 65534, 65535};
		{0, 	3,	TEST_ARRAY5, 0},
		{1, 	3,	TEST_ARRAY5, 0},
		{65534, 3,	TEST_ARRAY5, 1},
		{65535, 3,	TEST_ARRAY5, 2},

		//{0, 1, 65534, 65535};
		{0, 	4,	TEST_ARRAY6, 0},
		{1, 	4,	TEST_ARRAY6, 1},
		{100, 	4,	TEST_ARRAY6, 1},
		{65534, 4,	TEST_ARRAY6, 2},
		{65535, 4,	TEST_ARRAY6, 3},

		//{0, 1, 500, 65534, 65535};
		{0, 	5,	TEST_ARRAY7, 0},
		{1, 	5,	TEST_ARRAY7, 1},
		{100, 	5,	TEST_ARRAY7, 1},
		{499, 	5,	TEST_ARRAY7, 1},
		{500, 	5,	TEST_ARRAY7, 2},
		{501, 	5,	TEST_ARRAY7, 2},
		{65534, 5,	TEST_ARRAY7, 3},
		{65535, 5,	TEST_ARRAY7, 4},

		//const uint16_t TEST_ARRAY8[] = {0, 0, 0, 0, 0};
		{0, 	5,	TEST_ARRAY8, 0},
		{1, 	5,	TEST_ARRAY8, BSEARCH_GREATER_THAN_LARGEST},
		{100, 	5,	TEST_ARRAY8, BSEARCH_GREATER_THAN_LARGEST},
		{499, 	5,	TEST_ARRAY8, BSEARCH_GREATER_THAN_LARGEST},
		{500, 	5,	TEST_ARRAY8, BSEARCH_GREATER_THAN_LARGEST},
		{501, 	5,	TEST_ARRAY8, BSEARCH_GREATER_THAN_LARGEST},
		{65534, 5,	TEST_ARRAY8, BSEARCH_GREATER_THAN_LARGEST},
		{65535, 5,	TEST_ARRAY8, BSEARCH_GREATER_THAN_LARGEST},

		//const uint16_t TEST_ARRAY9[] = {0, 0, 0, 0, 1};
		{0, 	5,	TEST_ARRAY9, 0},
		{1, 	5,	TEST_ARRAY9, 4},
		{2, 	5,	TEST_ARRAY9, BSEARCH_GREATER_THAN_LARGEST},
		{65535, 5,	TEST_ARRAY9, BSEARCH_GREATER_THAN_LARGEST},

		//const uint16_t TEST_ARRAY10[] = {32, 32, 32, 32, 32};
		{0, 	5,	TEST_ARRAY10, BSEARCH_LESS_THAN_SMALLEST},
		{1, 	5,	TEST_ARRAY10, BSEARCH_LESS_THAN_SMALLEST},
		{31, 	5,	TEST_ARRAY10, BSEARCH_LESS_THAN_SMALLEST},
		{32, 	5,	TEST_ARRAY10, 0},
		{33, 	5,	TEST_ARRAY10, BSEARCH_GREATER_THAN_LARGEST},
		{65535, 5,	TEST_ARRAY10, BSEARCH_GREATER_THAN_LARGEST},

		//const uint16_t TEST_ARRAY10a[] = {31, 32, 32, 32, 32};
		{0, 	5,	TEST_ARRAY10a, BSEARCH_LESS_THAN_SMALLEST},
		{1, 	5,	TEST_ARRAY10a, BSEARCH_LESS_THAN_SMALLEST},
		{31, 	5,	TEST_ARRAY10a, 0},
		{32, 	5,	TEST_ARRAY10a, 1},
		{33, 	5,	TEST_ARRAY10a, BSEARCH_GREATER_THAN_LARGEST},
		{65535, 5,	TEST_ARRAY10a, BSEARCH_GREATER_THAN_LARGEST},

		//const uint16_t TEST_ARRAY11[] = {32, 32, 32, 32, 33};
		{0, 	5,	TEST_ARRAY11, BSEARCH_LESS_THAN_SMALLEST},
		{1, 	5,	TEST_ARRAY11, BSEARCH_LESS_THAN_SMALLEST},
		{31, 	5,	TEST_ARRAY11, BSEARCH_LESS_THAN_SMALLEST},
		{32, 	5,	TEST_ARRAY11, 0},
		{33, 	5,	TEST_ARRAY11, 4},
		{34, 	5,	TEST_ARRAY11, BSEARCH_GREATER_THAN_LARGEST},
		{65535, 5,	TEST_ARRAY11, BSEARCH_GREATER_THAN_LARGEST},

		//incorrectly sorted array
		//const uint16_t TEST_ARRAY12[] = {33, 30, 20, 10, 0};
		{0, 	5,	TEST_ARRAY12, BSEARCH_LESS_THAN_SMALLEST},
		{1, 	5,	TEST_ARRAY12, BSEARCH_LESS_THAN_SMALLEST},
		{31, 	5,	TEST_ARRAY12, BSEARCH_GREATER_THAN_LARGEST},
		{32, 	5,	TEST_ARRAY12, BSEARCH_GREATER_THAN_LARGEST},
		{65535, 5,	TEST_ARRAY12, BSEARCH_GREATER_THAN_LARGEST},

		//const uint16_t TEST_ARRAY13[] = {33, 30, 20, 10, 64};
		{0, 	5,	TEST_ARRAY13, BSEARCH_LESS_THAN_SMALLEST},
		{1, 	5,	TEST_ARRAY13, BSEARCH_LESS_THAN_SMALLEST},
		{31, 	5,	TEST_ARRAY13, 3},
		{32, 	5,	TEST_ARRAY13, 3},
		{64, 	5,	TEST_ARRAY13, 4},
		{65535, 5,	TEST_ARRAY13, BSEARCH_GREATER_THAN_LARGEST},

};



int Test_BinarySearch(void)
{
    int i;
    int failedCount = 0;
    
    for( i = 0; i < sizeof(TestCases)/sizeof(binarySearchTestElement_t); i++)
    {
    	uint16_t outputVal;
    	uint16_t size = TestCases[i].testSize;
    	outputVal = findElement_binarySearch( TestCases[i].testInput, TestCases[i].testArray, size );
    	
    	printf("\nPerforming Test Case[%d], Size: %d, Input:%d, Output:%d\n", i, size, TestCases[i].testInput, TestCases[i].testOutput);
    	
    	if( outputVal != TestCases[i].testOutput )
    	{
            printf("Test_BinarySearch Case[%d] Failed! Expected Output: %d, Received: %d\n", i, TestCases[i].testOutput, outputVal);
            failedCount++;
        }
        else
        {
            printf("Test_BinarySearch Case[%d] Success!\n", i);
        }
    }
    
    printf("Test_BinarySearch Summary: Performed: %d, Success: %d :: Failed: %d\n", i, i-failedCount, failedCount);

    if( failedCount )
    {
        return TEST_FAILED;
    }
    
    return TEST_SUCCESS;
}
