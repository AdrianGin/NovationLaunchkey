//Implements a state machine, see modes page in TechDiagram.ppt
//based on inputs

#include "ModeManager.h"
#include "Mode_OctaveTranspose.h"

static uint8_t handle_SWInput(MM_Input_t* input)
{

	return 0;
}

static uint8_t handle_ADCInput(MM_Input_t* input)
{

	return 0;
}

static uint8_t handle_KeyBoardInput(MM_Input_t* input)
{

	return 0;
}

uint8_t OctaveTranspose_Mode(MM_Input_t* input)
{
	uint8_t ret;

	switch (input->class)
	{
		case eSW_INPUT:
		{
			ret = handle_SWInput(input);
			break;
		}


		case eADC_INPUT:
		{
			ret = handle_ADCInput(input);
			break;
		}

		case eKEYBOARD_INPUT:
		{
			ret = handle_KeyBoardInput(input);
			break;
		}

		default:
		{
			break;
		}

	}

	return ret;
}

//MM_States_t AppStates[] =
//{
//		{MM_MODIFY_OCTAVE, {MM_SW_INPUT, } },
//		{},
//};

