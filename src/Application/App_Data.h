#ifndef _APP_DATA_H
#define _APP_DATA_H

#ifdef __cplusplus
extern "C" {
#endif



#include "ControlSurfaceMap.h"
#include "ADC_CentreDetent.h"
#include "Rescale.h"



extern ADC_CentreDetent_t PitchBendDetent;
extern const ControlSurfaceMap_t ADCMap[];
extern const Rescale_t Rescale_ModWheel;



#ifdef __cplusplus
}
#endif

#endif
