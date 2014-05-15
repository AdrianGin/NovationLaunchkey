
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "HardwareSpecific.h"


#define KEYS_PER_COLUMN	(8)
#define MAX_KB_TIMER	(65535)

#define NUMBER_OF_KEYS	(61)

#define BITS_PER_KEYMAP		(32)
#define BYTES_PER_KEYMAP	((NUMBER_OF_KEYS/BITS_PER_KEYMAP)+1)

#define NO_KB_STATE_CHANGE (255)

typedef enum
{
	KB_WAIT_FOR_BR = 0,
	KB_INITIAL_CONTACT,
	KB_WAIT_FOR_MK_OFF,
	KB_FINAL_CONTACT,
	KB_ACTION_STATES,
} KEYBOARD_STATES;


typedef enum
{
	NO_CONTACT = 0,
	INVALID,
	TOP_CONTACT,
	BOTH_CONTACTS,
} KB_SWITCH_STATES;


typedef struct
{
	union {
		uint8_t SWITCHSTATE;
		struct {
			uint32_t	MK	:1;
			uint32_t	BR	:1;
		} SS;
	};
	uint8_t  keyState;
	uint16_t timer;
} Keyboard_KeyInformation_t;

typedef struct
{
	uint8_t currentState;
	uint8_t inputAction;
	void (*fnPtr)(uint8_t);
	uint8_t newState;
} kbSM_t;

extern volatile uint16_t Keyboard_Timer;

void Keyboard_IncrementTimer(void);
//gets the time difference between oldTime and current time.
uint16_t Keyboard_DeltaTime(uint16_t oldTime);
uint8_t Keyboard_DetermineNewState(uint8_t keyIndex, KB_SWITCH_STATES newState);

void Keyboard_GPIO_Init(void);
//Obtains the raw Keyboard State
uint16_t Keyboard_ReadRawState(void);

//Turns the keyboard raw state into a keyboard map.
void Keyboard_ProcessRawState(uint16_t keyboardState);

//This function can be used to detect any changes in keyboard state
uint8_t Keyboard_SaveRawState(uint16_t keyboardState, uint8_t column);

uint32_t Keyboard_GetStateMap(uint8_t index);

uint32_t Keyboard_GetRawBRMKStateMap(uint8_t index);
KB_SWITCH_STATES Keyboard_GetRawKeyState(uint32_t* brmkBitmap, uint8_t logicalIndex);
uint8_t Keyboard_ProcessKeyMap(void);

void Keyboard_StartTimer(uint8_t keyIndex);
void Keyboard_StopTimer(uint8_t keyIndex);
void Keyboard_MaxVelocity(uint8_t keyIndex);
void Keyboard_SendOnVelocity(uint8_t keyIndex);
void Keyboard_SendQuickOn(uint8_t keyIndex);
void Keyboard_SendOffVelocity(uint8_t keyIndex);
void Keyboard_SendQuickOff(uint8_t keyIndex);

void Keyboard_ExecuteState(uint8_t keyIndex, KB_SWITCH_STATES action);

#ifdef __cplusplus
}
#endif



#endif





