#ifndef __AS5600_H
#define __AS5600_H

#include "headfile.h"

void AS5600_Read_Reg(unsigned char reg, unsigned char* buf, unsigned short len);
void AS5600_Write_Reg(unsigned char reg, unsigned char value);
float GetAngle_Without_Track(void);
float GetAngle(void);

float Get_Current_Angle(void);
int16_t Get_int_Angle(void);
float Get_CalibAngle(void);

float Get_MotorVelocity(Speed_Calculator* CalcStruct);


#endif /* __BSP_AS5600_H */
