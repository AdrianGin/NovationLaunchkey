#ifndef MODE_REMAP_H
#define MODE_REMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ModeManager.h"


typedef enum
{

	MR_SETMIN = 0,
	MR_SETMAX,
	MR_SETTYPE,
	MR_SETVALUE,
	MR_SETCHAN,
	MR_SETLSB,
	MR_INVALID,

} MM_REMAP_STATES;

typedef enum {
	RT_CC = 0,
	RT_AFT_TCH,
	RT_PROGRAM_CHANGE,
	RT_CHANNEL_PRES,
	RT_NRPN,
	RT_RPN,
	RT_NOTEOFF,
	RT_NOTEON,

} RT_REMAP_TYPES;


extern const uint8_t StatusTypeLookup[];

uint8_t ModeRemap_HandleInput(MM_Input_t* input);

#ifdef __cplusplus
}
#endif

#endif
