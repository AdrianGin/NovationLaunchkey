
#ifndef HAL_SWITCH_H
#define HAL_SWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Switch.h"

typedef enum
{
	SW_MUTE_0 = 0,
	SW_MUTE_1,
	SW_MUTE_2,
	SW_MUTE_3,

	SW_MUTE_4,
	SW_MUTE_5,
	SW_MUTE_6,
	SW_MUTE_7,

	SW_MUTE_8,
	SW_INCONTROL_TOP,
	SW_INCONTROL_MID,
	SW_INCONTROL_BOT,

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

	SW_COUNT
} E_SWITCH_INDEX;

#define SWITCH_ON  (1)
#define SWITCH_INVALID (0xFF)
#define SWITCH_OFF (0)

#ifdef __cplusplus
}
#endif



#endif




