#include <stdint.h>
#include "binarySearch.h"

//returns the address of 'element' inside the sorted array. sortedArray must be sorted from smallest to largest
//size is the number of elements inside sortedArray
//if the element cannot be found, the closest match rounded down will be returned.
//if the sortedArray is 0,0,0 and we wish to find 1, it will return BSEARCH_GREATER_THAN_LARGEST.
//if the sortedArray is 1,1,1 and we wish to find 0, it will return BSEARCH_LESS_THAN_SMALLEST.
//if the sortedArray is 1,4,4,4 and we wish to find 4, it will return 1 (the least)
uint16_t findElement_binarySearch(uint16_t element, uint16_t* sortedArray, uint16_t size)
{
	//start searching from the middle element of sortedArray
	if( size == 1 )
	{
		return 0;
	}

	if( sortedArray == 0)
	{
		return 0;
	}

	uint16_t min = 0;
	uint16_t max = size;
	uint16_t midPoint;


	for( midPoint = ((max+min) / 2) ;  max > min; midPoint = ((max+min) / 2) )
	{
		if( element > sortedArray[midPoint] )
		{
			min = midPoint + 1;
		}
		else
		{
            max = midPoint;
		}
	}
	
	if( (min >= size) || (max >= size) )
	{
		return BSEARCH_GREATER_THAN_LARGEST;
	}

	if( (min == max)  )
	{
		if( (sortedArray[min] == element) )
		{
			return min;
		}

		if( (element > sortedArray[min]) )
		{
			return BSEARCH_GREATER_THAN_LARGEST;
		}

		if(min && (element > sortedArray[min-1]) )
		{
			return min - 1;
		}
    }
    
	if( element < sortedArray[min] )
	{
		return BSEARCH_LESS_THAN_SMALLEST;
	}

	return BSEARCH_ERROR;
}





