#ifndef _GLOBAL_VARIABLE_H__
#define _GLOBAL_VARIABLE_H__

#include "stm32g4xx_hal.h"
#include "Mymath.h"

/* AS5600 */
extern float angle_prev;
extern int full_rotations; 	// full rotation tracking;
extern float Zero_Angle;			//AS5600_Origin_Angle

/* 设备机(下位机)状态 */
typedef enum {
    DEVICE_IDLE = 0,        
    DEVICE_CTRL_BY_VOFA,     	/* 上位机控制模式 */
	CTRL_MODE,
    DEVICE_CTRL_BY_PERI			/* 外部硬件控制模式(按键等) */              
}Device_State;

/* SlaveDevice_State */
extern Device_State SlaveDevice_State;

//原始三相电流/变换后的给定三相电压
struct Three_Phase 
{
	float Ia,Ib,Ic;
	float Ua,Ub,Uc;
};

//DQ轴坐标系结构体
struct DQ_Aix
{
    //转矩分量与励磁分量
		float I_d;	    //d-q坐标系，d轴电压分量
		float I_q;	    //d-q坐标系，q轴电压分量
	
		float U_d;			//给定电压力矩
		float U_q;		
	
		float theta;	//转子电气角度
};

//α-β轴坐标系结构体
struct AlphaBeta_Aix
{
		float I_Alpha;	    //α-β坐标系，α轴电压分量
		float I_Beta;		//α-β坐标系，β轴电压分量
	
		float U_Alpha;			//给定电压力矩
		float U_Beta;
};

//SVPWM部分
struct SVPWM_Control
{
		uint8_t sector;	        //扇区
		float X;				//SVPWM算法中间变量
		float Y;				//SVPWM算法中间变量
		float Z;				//SVPWM算法中间变量
		float T_First;	        //SVPWM算法中间变量
		float T_Second;	        //SVPWM算法中间变量
		float T0;		 		//SVPWM算法中间变量
		float Ta;				//SVPWM算法中间变量
		float Tb;				//SVPWM算法中间变量
		float Tc;				//SVPWM算法中间变量
};

//三相占空比控制结构体
struct SVPWM_Duty
{	
		float Ta;			//A相占空比PWM寄存器的值
		float Tb;			//B相占空比PWM寄存器的值
		float Tc;			//C相占空比PWM寄存器的值
};

extern struct Three_Phase Current_abc;		/* Origin_abc */
extern struct AlphaBeta_Aix Clark_AB;		/* Clark_AB */
extern struct DQ_Aix Park_DQ;					/* Park_DQ */
extern struct SVPWM_Control FOC_ctrl;
extern struct SVPWM_Duty FOC_Duty;

extern float Target_Uq;            /* 目标电压力矩 */

/* ADC(I) */
extern uint16_t MyADC_Value[2];
extern float Amp_Offset[2];		/* 采样电压偏置 */

/* debug */
extern uint8_t rx_buffer[BUFF_SIZE];           /* 接收缓冲区 */
extern uint8_t UART_RX_Flag;



#endif

