#ifndef _APP_DATA_H
#define _APP_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#define POT_COUNT 		(8)
#define FADER_COUNT 	(9)
#define MUTE_SOLO_COUNT (9)
#define DRUM_COUNT	    (16)

#define CONTROL_COUNT	(DRUM_COUNT + POT_COUNT + FADER_COUNT + MUTE_SOLO_COUNT)

#include "ControlSurfaceMap.h"
#include "ADC_CentreDetent.h"
#include "Rescale.h"



extern ADC_CentreDetent_t PitchBendDetent;
extern const ControlSurfaceMap_t DefaultADCMap[];
extern const Rescale_t Rescale_ModWheel;
extern ControlSurfaceMap_t** CurrentADCMap;
extern ControlSurfaceMap_t LoadedADCMap[];

#ifdef __cplusplus
}
#endif

#endif
