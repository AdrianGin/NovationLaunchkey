
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "HardwareSpecific.h"


#define KEYS_PER_COLUMN	(8)

#define NUMBER_OF_KEYS	(61)

#define BITS_PER_KEYMAP		(32)
#define BYTES_PER_KEYMAP	(3)

void Keyboard_GPIO_Init(void);
//Obtains the raw Keyboard State
uint16_t Keyboard_ReadRawState(void);

//Turns the keyboard raw state into a keyboard map.
void Keyboard_ProcessRawState(uint16_t keyboardState);

void Keyboard_SetMapBit(uint32_t* bitmap, uint8_t bit);
void Keyboard_ClrMapBit(uint32_t* bitmap, uint8_t bit);
uint32_t Keyboard_GetStateMap(uint8_t index);
uint8_t Keyboard_GetKeyIndex(uint8_t byteIndex, uint8_t bitIndex);


#ifdef __cplusplus
}
#endif



#endif





