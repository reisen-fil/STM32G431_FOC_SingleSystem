#ifndef _SVPWM_H_
#define _SVPWM_H_

#include "Headfile.h"

void Park_transit(struct DQ_Aix* DQ,struct AlphaBeta_Aix* AB);
void Rev_Park(struct DQ_Aix* DQ,struct AlphaBeta_Aix* AB);
void clark_transit(struct Three_Phase* abc,struct AlphaBeta_Aix* AB);
void Rev_clark(struct Three_Phase* abc,struct AlphaBeta_Aix* AB);

void CalculateSector(struct AlphaBeta_Aix AB, struct SVPWM_Control* ptr);
void CalulateDutyCycle(struct SVPWM_Control* ctrl,struct SVPWM_Duty* ptr);


#endif
