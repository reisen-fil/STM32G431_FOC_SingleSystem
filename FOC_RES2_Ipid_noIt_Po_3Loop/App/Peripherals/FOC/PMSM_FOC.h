#ifndef __PMSM_FOC_H__
#define __PMSM_FOC_H__

#include "Headfile.h"

void Open_PMSM_FOC(void);
void Set_ZeroAngle(void);
void FOC_SVPWM(float FOC_Iq,float FOC_Id);	



#endif

