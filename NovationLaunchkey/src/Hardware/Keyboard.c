

#include "Keyboard.h"
#include "MultiplexControl.h"

void Keyboard_GPIO_Init(void)
{
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_MK0, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_BR0, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_MK1, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_BR1, E_IO_INPUT);

	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_MK3, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_BR3, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_MK4, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_BR4, E_IO_INPUT);

	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_MK5, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_BR5, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_MK6, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_BR6, E_IO_INPUT);

	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_MK7, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT, KEYBOARD_BR7, E_IO_INPUT);

	DrvGPIO_Open(KEYBOARD_INPUT_PORT2, KEYBOARD_MK2, E_IO_INPUT);
	DrvGPIO_Open(KEYBOARD_INPUT_PORT2, KEYBOARD_BR2, E_IO_INPUT);
}


uint16_t Keyboard_ReadRawState(void)
{
	int32_t portState;

	uint16_t kbState = 0;

	portState = DrvGPIO_GetPortBits(KEYBOARD_INPUT_PORT);
	portState &= KEYBOARD_INPUT_MASK;

	kbState = portState;

	portState = DrvGPIO_GetPortBits(KEYBOARD_INPUT_PORT2);
	portState &= KEYBOARD_INPUT_MASK2;

	kbState |= ((portState >> KEYBOARD_MK2) << (KEYBOARD_BR1+1));

	return (kbState);
}


//Use 1 bit for each key
//BR makes first aka the Top contact,
//MK is the final contact, the bottom.
uint32_t Keyboard_RawBRStateMap[BYTES_PER_KEYMAP];
uint32_t Keyboard_RawMKStateMap[BYTES_PER_KEYMAP];

//Indexes 0-2 are for BR, 3-5 are MK
uint32_t Keyboard_GetStateMap(uint8_t index)
{
	if( index >= (2*BYTES_PER_KEYMAP) )
	{
		return 0xFFFFFFFF;
	}

	if( index < BYTES_PER_KEYMAP )
	{
		return Keyboard_RawBRStateMap[index];
	}
	else
	{
		return Keyboard_RawMKStateMap[index-BYTES_PER_KEYMAP];
	}
}

void Keyboard_SetMapBit(uint32_t* bitmap, uint8_t bit)
{
	uint8_t index;
	uint8_t bitIndex;

	index = bit / BITS_PER_KEYMAP;
	bitIndex = (bit - (index * BITS_PER_KEYMAP));

	bitmap[index] |= (uint32_t)(1<<bitIndex);
}

void Keyboard_ClrMapBit(uint32_t* bitmap, uint8_t bit)
{
	uint8_t index;
	uint8_t bitIndex;

	index = bit / BITS_PER_KEYMAP;
	bitIndex = (bit - (index * BITS_PER_KEYMAP));

	bitmap[index] &= ~(uint32_t)(1<<bitIndex);
}


//Turns the keyboard raw state into a keyboard map.
void Keyboard_ProcessRawState(uint16_t keyboardState)
{
	uint8_t column = MUX_GetCurrentColumn();
	uint8_t i;

	if( column >= MAX_LINE_COLUMNS )
	{
		return;
	}

	uint8_t logicalIndex;

	for( i = 0; i < (KEYS_PER_COLUMN) ; i++ )
	{
		logicalIndex = (column) + (i*(KEYS_PER_COLUMN));

		Keyboard_ClrMapBit( (uint32_t*)Keyboard_RawMKStateMap, logicalIndex);
		Keyboard_ClrMapBit( (uint32_t*)Keyboard_RawBRStateMap, logicalIndex);

		if( keyboardState & (1<<(i*2)) )
		{
			Keyboard_SetMapBit( (uint32_t*)Keyboard_RawBRStateMap, logicalIndex);
		}

		if( keyboardState & (1<<((i*2)+1)) )
		{
			Keyboard_SetMapBit( (uint32_t*)Keyboard_RawMKStateMap, logicalIndex);
		}
	}
}

//Returns 0 to NUMBER_OF_KEYS KEY COUNT, based on the index and bit index
inline uint8_t Keyboard_GetKeyIndex(uint8_t byteIndex, uint8_t bitIndex)
{
	return (byteIndex*BITS_PER_KEYMAP) + bitIndex;
}



uint8_t Keyboard_ProcessKeyMap(void)
{

	//Obtain all the changes since the last ProcessState.

	//Each key will have it's own state. We need to work out which state we're in.

}



