


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*

Copyright (c) 2011 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

/* Ring buffer implementation */

#include "voidbuffer.h"
#include "TestDefines.h"

typedef struct
{
	uint8_t var8;
	uint16_t var16;
	uint32_t var32;
} TestStruct_t;

#define TEST_ARRAY_SIZE (16)

#define TEST_CASE_COUNT (TEST_ARRAY_SIZE*2)

TestStruct_t VB_TestCases[TEST_CASE_COUNT];

TestStruct_t TestArray[TEST_ARRAY_SIZE];
VoidBuffer_t TestBuffer =
{
		.memPtrArray = (void*)TestArray,
		.bufferSize = TEST_ARRAY_SIZE,
		.elementSize = sizeof(TestStruct_t),
};


int VB_EmptyTest(VoidBuffer_t* buf)
{
	VoidBuffer_Clear(buf);
	if( VoidBuffer_isEmpty(buf) != VOIDBUFFER_EMPTY)
	{
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return TEST_FAILED;
	}
	if( VoidBuffer_Len(buf) != 0 )
	{
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return TEST_FAILED;
	}
	return TEST_SUCCESS;
}


//Fills the buffer up to the size and more
int VB_LengthTest(VoidBuffer_t* buf)
{
	uint8_t testStatus = TEST_SUCCESS;
	//Test that it can be emptied first.
	if( VB_EmptyTest(buf) != TEST_SUCCESS )
	{
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
	}

	if( VoidBuffer_Len(buf) != 0 )
	{
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		testStatus = TEST_FAILED;
	}

	int i;
	for( i = 0; i < buf->bufferSize; i++ )
	{
		uint8_t len;
		len = VoidBuffer_PushData(buf, (void*)&VB_TestCases[0]);

		if( i < (buf->bufferSize - 1) )
		{
			if( VoidBuffer_Len(buf) != len )
			{
				printf("BufferLength = %d, expected %d\n", VoidBuffer_Len(buf), i+1);
				printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
				testStatus = TEST_FAILED;
			}

			if( VoidBuffer_Len(buf) != (i+1) )
			{
				printf("BufferLength = %d, expected %d\n", VoidBuffer_Len(buf), i+1);
				printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
				testStatus = TEST_FAILED;
			}
		}

		if( i == (buf->bufferSize - 1) )
		{
			if( len != VOIDBUFFER_OVERFLOW)
			{
				printf("Len = %d, expected %d\n", len, VOIDBUFFER_OVERFLOW);
				printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
				testStatus = TEST_FAILED;
			}
		}
		VB_VerifyTestCase(buf, &VB_TestCases[0]);
	}
	return testStatus;
}

//Fills & empties the buffer 16 times.
int VB_PopNPushTest(VoidBuffer_t* buf)
{
	uint8_t testStatus = TEST_SUCCESS;
	//Test that it can be emptied first.
	int i;
	for( i = 0; i < buf->bufferSize; i++ )
	{
		uint8_t len;
		len = VoidBuffer_PushData(buf, (void*)&VB_TestCases[i]);

		if( len == VOIDBUFFER_OVERFLOW )
		{
			printf("Overflow Len = %d, expected %d\n", len, 1);
			printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
			testStatus = TEST_FAILED;
		}

		TestStruct_t* tmp;
		tmp = VoidBuffer_PopData(buf);
		VB_MatchTestStructs(tmp, &VB_TestCases[i]);
	}
	return testStatus;
}

//Ensures that VOIDBUFFER_OVERFLOW is returned when there is no data.
int VB_EmptyPopTest(VoidBuffer_t* buf)
{
	uint8_t res;
	VoidBuffer_Clear(buf);
	res = VoidBuffer_PopData(buf);

	if( res != VOIDBUFFER_NO_DATA )
	{
		printf("Pop Test failed!\n");
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return TEST_FAILED;
	}

	return TEST_SUCCESS;
}


//Fills the buffer to the max, and then empties it.
int VB_FillThenEmpty(VoidBuffer_t* buf)
{
	VoidBuffer_Clear(buf);

	uint8_t testStatus = TEST_SUCCESS;
	//Test that it can be emptied first.
	int i;
	for( i = 0; i < (buf->bufferSize - 1); i++ )
	{
		uint8_t len;
		len = VoidBuffer_PushData(buf, (void*)&VB_TestCases[i+2]);

		if( len == VOIDBUFFER_OVERFLOW )
		{
			printf("Overflow Len = %d, expected %d\n", len, 1);
			printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
			testStatus = TEST_FAILED;
		}
	}

	for( i = 0; i < buf->bufferSize - 1; i++ )
	{
		TestStruct_t* tmp;
		tmp = VoidBuffer_PopData(buf);

		if( tmp == VOIDBUFFER_NO_DATA)
		{
			printf("PopData Failed = %d\n", tmp);
			printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
			testStatus = TEST_FAILED;
		}

		VB_MatchTestStructs(tmp, &VB_TestCases[i+2]);
	}

	return testStatus;
}

int VB_VerifyTestCase(VoidBuffer_t* buf, TestStruct_t* compare)
{
	TestStruct_t* ptr;
	ptr = VoidBuffer_PeekData(buf);
	if( memcmp(ptr, compare, sizeof(TestStruct_t)) != 0 )
	{
		printf("Buffer ReadBack: %d, %d, %d\n", ptr->var8, ptr->var16, ptr->var32);
		printf("Compare ReadBack: %d, %d, %d\n", compare->var8, compare->var16, compare->var32);
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return TEST_FAILED;
	}
	return TEST_SUCCESS;
}


int VB_MatchTestStructs(TestStruct_t* compare, TestStruct_t* compare2)
{
	TestStruct_t* ptr;
	if( memcmp(compare, compare2, sizeof(TestStruct_t)) != 0 )
	{
		printf("Compare ReadBack: %d, %d, %d\n", compare->var8, compare->var16, compare->var32);
		printf("Compare2 ReadBack: %d, %d, %d\n", compare2->var8, compare2->var16, compare2->var32);
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return TEST_FAILED;
	}
	return TEST_SUCCESS;
}


void VB_InitTestCase()
{
	uint8_t i;
	for(i = 0; i < TEST_CASE_COUNT; i++ )
	{
		VB_TestCases[i].var8 = i;
		VB_TestCases[i].var16 = i+100;
		VB_TestCases[i].var32 = i+1000;
	}


}


uint8_t VB_ClearBuffer(VoidBuffer_t* buf)
{
	VoidBuffer_Clear(buf);
	if( VoidBuffer_isEmpty(buf) != VOIDBUFFER_EMPTY )
	{
		printf("Clear Failed\n");
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return TEST_FAILED;
	}

	return TEST_SUCCESS;
}

int main(void)
{

	VB_ClearBuffer(&TestBuffer);

	VB_InitTestCase();
	if( VB_LengthTest(&TestBuffer) != TEST_SUCCESS )
	{
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return 1;
	}


	VB_ClearBuffer(&TestBuffer);
	if( VB_PopNPushTest(&TestBuffer) != TEST_SUCCESS )
	{
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return 1;
	}

	if( VB_EmptyPopTest(&TestBuffer) != TEST_SUCCESS )
	{
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return 1;
	}

	if( VB_FillThenEmpty(&TestBuffer) != TEST_SUCCESS )
	{
		printf("Test Failed Line: %d, File %s\n", __LINE__, __FILE__);
		return 1;
	}

	printf("Test Passed!\n");


	return 0;

}

