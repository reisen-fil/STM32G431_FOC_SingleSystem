#ifndef _PID_H_
#define _PID_H_

#include "Headfile.h"

void Curr_PI_Cal(Str_pid *PI_Control, float target, float outMax);
void Ipid_Init(Str_pid* Ipid,float Kp,float Ki,float Kd,float SumMAX);

#endif
