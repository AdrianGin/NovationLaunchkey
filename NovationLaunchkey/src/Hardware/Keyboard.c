

#include "Keyboard.h"
#include "MultiplexControl.h"

Keyboard_KeyInformation_t Keyboard_Info[NUMBER_OF_KEYS];
volatile uint16_t Keyboard_Timer = 0;

inline void Keyboard_IncrementTimer(void)
{
	Keyboard_Timer++;
}

//gets the time difference between oldTime and current time.
inline uint16_t Keyboard_DeltaTime(uint16_t oldTime)
{
	if( Keyboard_Timer >= oldTime )
	{
		return Keyboard_Timer - oldTime;
	}
	else
	{
		return (MAX_KB_TIMER - oldTime) + Keyboard_Timer + 1;
	}
}


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
//In the format of BR0:MK0, BR1:MK1 bitmap.
volatile uint32_t Keyboard_RawBRMKStateMap[BYTES_PER_KEYMAP*2];

inline uint32_t Keyboard_GetRawBRMKStateMap(uint8_t index)
{
	return Keyboard_RawBRMKStateMap[index];
}

//0 for key 0, 1 for key 1 etc...
inline uint8_t Keyboard_GetRawKeyState(uint32_t* brmkBitmap, uint8_t logicalIndex)
{
	uint8_t index;
	uint8_t bitIndex;
	uint32_t state;

	index = logicalIndex / (BITS_PER_KEYMAP/2);
	bitIndex = (logicalIndex - (index*(BITS_PER_KEYMAP/2)));

	state = brmkBitmap[index] >> (bitIndex*2);
	return (uint8_t)(state & 0x03);
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





const kbSM_t KB_StateMachine[] =
{
		//Normal keypress flow.
		{KB_WAIT_FOR_BR, TOP_CONTACT  	 	 , Keyboard_StartTimer,		KB_INITIAL_CONTACT},
		//A very strong hit, didn't even detect the first contact
		{KB_WAIT_FOR_BR, BOTH_CONTACTS	 	 , Keyboard_MaxVelocity, 	KB_INITIAL_CONTACT},
		//Released before a hit is made
		{KB_INITIAL_CONTACT, NO_CONTACT      , Keyboard_StopTimer,		KB_WAIT_FOR_BR},
		//Final hit is complete - send NoteOn
		{KB_INITIAL_CONTACT, BOTH_CONTACTS   , Keyboard_SendOnVelocity,	KB_WAIT_FOR_MK_OFF},
		//First stage of release
		{KB_WAIT_FOR_MK_OFF, TOP_CONTACT	 , Keyboard_StartTimer,		KB_FINAL_CONTACT},
		//Key is played before fully released,
		//Play a Note off, then play Note On again, but use different response curve
		{KB_FINAL_CONTACT, BOTH_CONTACTS	 , Keyboard_SendQuickOn,		KB_WAIT_FOR_MK_OFF},
		//Full release is made, send a Note Off with Velocity
		{KB_FINAL_CONTACT, NO_CONTACT	     , Keyboard_SendOffVelocity,		KB_WAIT_FOR_BR},
};



void Keyboard_ExecuteState(uint8_t keyIndex, uint8_t action)
{
	uint8_t i;

	Keyboard_KeyInformation_t* info = &Keyboard_Info[keyIndex];

	for( i = 0 ; i < sizeof(KB_StateMachine)/sizeof(kbSM_t); i++)
	{
		if( info->keyState == KB_StateMachine[i].currentState )
		{
			if( action == KB_StateMachine[i].inputAction )
			{
				KB_StateMachine[i].fnPtr(keyIndex);
				info->keyState = KB_StateMachine[i].newState;
			}
		}
	}
}

void Keyboard_StartTimer(uint8_t keyIndex)
{
	Keyboard_KeyInformation_t* info = &Keyboard_Info[keyIndex];
	info->timer = Keyboard_Timer;
}

void Keyboard_StopTimer(uint8_t keyIndex)
{
	Keyboard_KeyInformation_t* info = &Keyboard_Info[keyIndex];
	info->timer = Keyboard_Timer;
}

void Keyboard_MaxVelocity(uint8_t keyIndex)
{
	printNumber(keyIndex);
	printNumber(127);
}

void Keyboard_SendOnVelocity(uint8_t keyIndex)
{
	Keyboard_KeyInformation_t* info = &Keyboard_Info[keyIndex];
	uint16_t deltaTime;

	deltaTime = Keyboard_DeltaTime(info->timer);

	printNumber(keyIndex);
	printNumber(deltaTime);


}

void Keyboard_SendQuickOn(uint8_t keyIndex)
{
	printNumber(keyIndex);
	printNumber(1);
}

void Keyboard_SendOffVelocity(uint8_t keyIndex)
{
	Keyboard_KeyInformation_t* info = &Keyboard_Info[keyIndex];
	uint16_t deltaTime;
	deltaTime = Keyboard_DeltaTime(info->timer);

	printNumber(keyIndex);
	printNumber(deltaTime);
}




uint8_t Keyboard_DetermineNewState(uint8_t keyIndex, uint8_t oldState, uint8_t newState)
{
	if( keyIndex >= NUMBER_OF_KEYS)
	{
		return 0;
	}

	Keyboard_ExecuteState(keyIndex, newState);

	return 1;
}



uint8_t Keyboard_ProcessKeyMap(void)
{

	//Obtain all the changes since the last ProcessState.
	uint8_t i, j;
	//remembers the old state;
	static uint32_t KeyMapBRMK[BYTES_PER_KEYMAP*2];

	for( j = 0 ; j < (BYTES_PER_KEYMAP*2) ; j++ )
	{
		uint32_t newKeyMap = Keyboard_GetRawBRMKStateMap(j);

		if( (KeyMapBRMK[j] != newKeyMap)  )
		{
			uint32_t keyChangeMap = KeyMapBRMK[j] ^ newKeyMap;
			for( i = 0 ; (i < BITS_PER_KEYMAP) && (keyChangeMap != 0); i=i+2 )
			{
				uint8_t changedFlag = FALSE;
				//Check the BR bit
				if( keyChangeMap & (1<<(i)) )
				{
					keyChangeMap &= ~(1<<(i));
					changedFlag = TRUE;
				}

				//Check the MK bit
				if( keyChangeMap & (1<<(i+1)) )
				{
					keyChangeMap &= ~(1<<(i+1));
					changedFlag = TRUE;
				}

				if( changedFlag )
				{
					uint8_t keyIndex = ((j*BITS_PER_KEYMAP) + (i) ) >> 1;
					Keyboard_DetermineNewState(keyIndex, \
											   Keyboard_GetRawKeyState(KeyMapBRMK,keyIndex), \
											   Keyboard_GetRawKeyState((uint32_t*)Keyboard_RawBRMKStateMap,keyIndex) );
				}

			}
		}

		KeyMapBRMK[j] = newKeyMap;
	}
	//Each key will have it's own state. We need to work out which state we're in.
	return 1;
	//Determine what we need to do.
}



