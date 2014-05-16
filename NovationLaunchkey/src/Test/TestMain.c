

#include "binarySearch.h"

#include "Test_BinarySearch.h"
#include "TestDefines.h"



int main(void)
{
	int ret = Test_BinarySearch();
    if( ret != TEST_SUCCESS )
    {
    	printf("Test_BinarySearch Failed! Error Code %d\n", ret);
    }
    else
    {
   	    printf("Test_BinarySearch Passed!");
    }
    
    return 0;
}
