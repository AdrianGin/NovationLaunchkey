

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
volatile uint32_t Keyboard_RawBRStateMap[BYTES_PER_KEYMAP];
volatile uint32_t Keyboard_RawMKStateMap[BYTES_PER_KEYMAP];

//In the format of BR0:MK0, BR1:MK1 bitmap.
volatile uint32_t Keyboard_RawBRMKStateMap[BYTES_PER_KEYMAP*2];

inline uint32_t Keyboard_GetRawBRMKStateMap(uint8_t index)
{
	return Keyboard_RawBRMKStateMap[index];
}

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

inline void Keyboard_SetMapBit(uint32_t* bitmap, uint8_t index, uint8_t bitIndex)
{
	bitmap[index] |= (uint32_t)(1<<bitIndex);
}

inline void Keyboard_ClrMapBit(uint32_t* bitmap, uint8_t index, uint8_t bitIndex)
{
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

		uint8_t index;
		uint8_t bitIndex;
		index = logicalIndex / BITS_PER_KEYMAP;
		bitIndex = (logicalIndex - (index * BITS_PER_KEYMAP));


		index = logicalIndex / (BITS_PER_KEYMAP/2);
		bitIndex = (logicalIndex - (index*(BITS_PER_KEYMAP/2)));

		Keyboard_RawBRMKStateMap[index] &= ~((0x03) << (2*bitIndex));

		if( keyboardState & (1<<(i*2)) )
		{
			Keyboard_RawBRMKStateMap[index] |= ((0x01) << (2*bitIndex));
		}

		if( keyboardState & (1<<((i*2)+1)) )
		{
			Keyboard_RawBRMKStateMap[index] |= ((0x02) << (2*bitIndex));
		}
	}
}

//Returns 0 to NUMBER_OF_KEYS KEY COUNT, based on the index and bit index
inline uint8_t Keyboard_GetKeyIndex(uint8_t byteIndex, uint8_t bitIndex)
{
	return (byteIndex*BITS_PER_KEYMAP) + bitIndex;
}



typedef enum
{
	KB_WAIT_FOR_BR = 0,
	KB_INITIAL_CONTACT,
	KB_WAIT_FOR_MK_OFF,
	KB_FINAL_CONTACT,
} KEYBOARD_STATES;

typedef struct
{
	union {
		uint8_t SWITCHSTATE;
		struct {
			uint32_t	MK	:1;
			uint32_t	BR	:1;
		} SS;
	};

	uint16_t timer;
} Keyboard_KeyInformation_t;

Keyboard_KeyInformation_t Keyboard_Info[NUMBER_OF_KEYS];

uint8_t Keyboard_DetermineNewState(uint8_t keyIndex, uint8_t oldState, uint8_t newState)
{


}

uint8_t Keyboard_ProcessKeyMap(void)
{

	//Obtain all the changes since the last ProcessState.
	uint8_t i, j;
	static uint32_t KeyMapBR[BYTES_PER_KEYMAP];
	static uint32_t KeyMapMK[BYTES_PER_KEYMAP];

	for( j = 0 ; j < BYTES_PER_KEYMAP; j++ )
	{
		uint32_t newKeyMapBR = Keyboard_GetStateMap(j);
		uint32_t newKeyMapMK = Keyboard_GetStateMap(j+BYTES_PER_KEYMAP);

		if( (KeyMapBR[j] != newKeyMapBR) || (KeyMapMK[j] != newKeyMapMK)  )
		{
			uint32_t keyChangeMapBR = KeyMapBR[j] ^ newKeyMapBR;
			uint32_t keyChangeMapMK = KeyMapMK[j] ^ newKeyMapMK;
			//KeyMap[j] = newKeyMap;

//			for( i = 0 ; (i < BITS_PER_KEYMAP) && keyChangeMap != 0; i++ )
//			{
//				if( keyChangeMap & (1<<i) )
//				{
//					keyChangeMap &= ~(1<<i);
//					uint8_t keyIndex = Keyboard_GetKeyIndex(j%BYTES_PER_KEYMAP, i);
//				}
//			}
		}





	}
	//Each key will have it's own state. We need to work out which state we're in.

	//Determine what we need to do.
}



