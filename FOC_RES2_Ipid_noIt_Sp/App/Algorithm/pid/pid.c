#include "pid.h"

/* 电流环pid */
void Curr_PI_Cal(Str_pid *PI_Control, float target, float outMax)
{
    PI_Control->target = target;
    
    PI_Control->error = PI_Control->target - PI_Control->value;							//误差等于目标值减去测量值
    
    PI_Control->erro_sum += PI_Control->error * PI_Control->ki;							//积分项累加
    
    if(PI_Control->erro_sum > PI_Control->errSumMax)										//积分限幅
        PI_Control->erro_sum = PI_Control->errSumMax;
    else if(PI_Control->erro_sum < -PI_Control->errSumMax)
        PI_Control->erro_sum = -PI_Control->errSumMax;   
    
    PI_Control->out = PI_Control->kp * PI_Control->error + PI_Control->erro_sum;		//得到PI运算结果
    
    if(PI_Control->out > outMax)													//最大输出限幅
        PI_Control->out = outMax;
    else if(PI_Control->out < -outMax)
        PI_Control->out = -outMax;
}

/* 限幅部分 */
float LimitAmp(float Amp,float Amp_max,float Amp_min)
{
	if(Amp >= Amp_max) Amp=Amp_max;
	else if(Amp <= Amp_min) Amp=Amp_min;
	return Amp;
}

/* 转速环pid */
void Sp_PI_Cal(Str_pid *PI_Control, float target, float outMax)
{
    PI_Control->target = target;
    PI_Control->error = PI_Control->target - PI_Control->value;							//误差等于目标值减去测量值    
    PI_Control->erro_sum += PI_Control->error;							//积分项累加

		PI_Control->erro_sum = LimitAmp(PI_Control->erro_sum,PI_Control->errSumMax,-PI_Control->errSumMax);		//积分项累加	
	
    PI_Control->out = LimitAmp(PI_Control->kp * PI_Control->error + PI_Control->ki * PI_Control->erro_sum,
														outMax,-outMax);		//得到PI运算结果
}

/* PIDPara_Init */
void single_pid_Init(Str_pid *PI_Control,float Kp,float Ki,float Kd,float SumMAX)
{
		PI_Control->kp = Kp;
		PI_Control->ki = Ki;
		PI_Control->kd = Kd;
	
		PI_Control->errSumMax = SumMAX;
}




