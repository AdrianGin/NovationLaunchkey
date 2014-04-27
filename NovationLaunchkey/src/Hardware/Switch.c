

#include "Switch.h"
#include "MultiplexControl.h"



const uint8_t SWITCH_LOOKUP[] = 
{
	SW_MUTE_3,
	SW_MUTE_2,
	SW_MUTE_1,
	SW_MUTE_0,

	SW_MUTE_4,
	SW_MUTE_5,
	SW_MUTE_6,
	SW_MUTE_7,

	SW_MUTE_8,
	SW_INCONTROL_BOT,
	SW_INCONTROL_MID,
	SW_INCONTROL_TOP,

	SW_SCENE_0,
	SW_SCENE_1,
	SW_REWIND,
	SW_FASTFWD,

	SW_STOP,
	SW_PLAY,
	SW_LOOP,
	SW_REC,

	SW_TRACK_LEFT,
	SW_TRACK_RIGHT,
	SW_OCTAVE_DOWN,
	SW_OCTAVE_UP,

};


volatile uint32_t SwitchRawStates = 0;

void Switch_GPIO_Init(void)
{
	DrvGPIO_Open(SWITCH_INPUT_PORT, SWITCH_INPUT0_PIN, E_IO_INPUT);
	DrvGPIO_Open(SWITCH_INPUT_PORT, SWITCH_INPUT1_PIN, E_IO_INPUT);
	DrvGPIO_Open(SWITCH_INPUT_PORT, SWITCH_INPUT2_PIN, E_IO_INPUT);
	DrvGPIO_Open(SWITCH_INPUT_PORT, SWITCH_INPUT3_PIN, E_IO_INPUT);
}

uint8_t Switch_ReadState(void)
{
	int32_t portState;
	portState = DrvGPIO_GetPortBits(SWITCH_INPUT_PORT);
	portState &= SWITCH_INPUT_PIN_MASK;
	return (uint8_t)(portState & SWITCH_INPUT_PIN_MASK);
}

uint32_t Switch_GetSwitchStates(void)
{
	return SwitchRawStates;
}

uint32_t Switch_CheckForChange(void)
{
	uint32_t oldState = SwitchRawStates;
	uint32_t newState;

	Switch_ProcessState( Switch_ReadState() );

	newState = SwitchRawStates;
	return oldState ^ newState;
}

void Switch_ProcessState(uint8_t switchStates)
{
	uint8_t column = MUX_GetCurrentColumn();
	uint8_t i;

	if( column >= MAX_SW_COLUMNS )
	{
		return;
	}

	uint8_t logicalIndex = column*SWITCHES_PER_COLUMN;
	uint8_t resolvedIndex;

	for( i = 0; i < SWITCHES_PER_COLUMN; i++ )
	{
		logicalIndex = (column*SWITCHES_PER_COLUMN) + i;
		resolvedIndex = SWITCH_LOOKUP[logicalIndex];


		SwitchRawStates &= ~(1<<resolvedIndex);

		if( switchStates & (1<<i) )
		{
			SwitchRawStates |= (1<<resolvedIndex);
		}
	}
}


uint8_t Switch_GetState(uint8_t index)
{
	if( index > SW_REC )
	{
		return SWITCH_INVALID;
	}
	return ((SwitchRawStates & (1<<index)) != 0);
	
}
