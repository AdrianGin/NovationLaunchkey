#include <stdio.h>

#include "binarySearch.h"

#include "Test_BinarySearch.h"
#include "Test_Rescale.h"
#include "TestDefines.h"


#define TEST_COUNT (2)
int main(void)
{
	int ret[TEST_COUNT];
	ret[0] = Test_BinarySearch();
    if( ret[0] != TEST_SUCCESS )
    {
    	printf("Test_BinarySearch Failed! Error Code %d\n", ret[0]);
    }
    else
    {
   	    printf("Test_BinarySearch Passed!\n");
    }
    
    ret[1] = Test_Rescale();
	if( ret[1] != TEST_SUCCESS )
	{
		printf("Test_Rescale Failed! Error Code %d\n", ret[1]);
	}
	else
	{
		printf("Test_Rescale Passed!\n");
	}

	int i;
	printf("\n\n====TEST SUMMARY====\n");
	for(i = 0; i < TEST_COUNT; i++ )
	{
		if( ret[i] != TEST_SUCCESS)
		{
			printf("Test[%d] Failed!\n", i);
		}
		else
		{
			printf("Test[%d] Passed!\n", i);
		}
	}

    return 0;
}
