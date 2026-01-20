#include "Global_Variable.h"

/* Motor_State */
System_State Motor_State;
uint16_t FOC_Stablity_Cnt;

/* SlaveDevice_State */
Device_State SlaveDevice_State;

/* AS5600 */
float angle_prev=0; 
int full_rotations=0; // full rotation tracking;
float Zero_Angle=0;			//AS5600_Origin_Angle
uint8_t ZeroAlign_Flag;

/* ADC(I) */
uint16_t MyADC_Value[2];
float Amp_Offset[2];		/* 采样电压偏置 */

/* FOC */
struct Three_Phase Current_abc;		/* Origin_abc */
struct AlphaBeta_Aix Clark_AB;		/* Clark_AB */
struct DQ_Aix Park_DQ;					/* Park_DQ */
struct SVPWM_Control FOC_ctrl;
struct SVPWM_Duty FOC_Duty;

uint8_t Ictrl_cnt;			/* Ipid_Ctrl_cnt */
uint8_t Po_ctrl_cnt;			/* Po_pid_Ctrl_cnt */

/* pid */
Str_pid Id_pid;   /* 电流环pid */
Str_pid Iq_pid;
Str_pid Speed_pid;    /* 速度环pid */
Str_pid Position_pid;   /* 位置式pid */

Speed_Calculator MotorSpeed_Calc;       /* 电机角速度计算 */
float Angle_Offset,Last_Angle_Offset;                     /* 从编码器中获取的带偏置的原始角度 */


/* debug */
uint8_t rx_buffer[BUFF_SIZE];           /* 接收缓冲区 */







