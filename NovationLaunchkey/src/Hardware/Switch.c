

#include "Switch.h"
#include "MultiplexControl.h"

volatile uint32_t SwitchRawStates = 0;

void Switch_GPIO_Init(void)
{
	DrvGPIO_InitFunction(E_FUNC_GPIO);
	DrvGPIO_Open(SWITCH_INPUT_PORT, SWITCH_INPUT0_PIN, E_IO_INPUT);
	DrvGPIO_Open(SWITCH_INPUT_PORT, SWITCH_INPUT0_PIN, E_IO_INPUT);
	DrvGPIO_Open(SWITCH_INPUT_PORT, SWITCH_INPUT0_PIN, E_IO_INPUT);
	DrvGPIO_Open(SWITCH_INPUT_PORT, SWITCH_INPUT0_PIN, E_IO_INPUT);
}

uint8_t Switch_ReadState(void)
{
	int32_t portState;
	portState = DrvGPIO_GetPortBits(SWITCH_INPUT_PORT);
	portState &= SWITCH_INPUT_PIN_MASK;
	return (uint8_t)(portState & 0xFF);
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

	uint8_t logicalIndex = column*SWITCHES_PER_COLUMN;

	for( i = 0; i < SWITCHES_PER_COLUMN; i++ )
	{
		logicalIndex = logicalIndex + i;
		SwitchRawStates &= ~(1<<i);

		if( switchStates & (1<<i) )
		{
			SwitchRawStates |= (1<<i);
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
