#ifndef __MYMATH_H_
#define __MYMATH_H_

#include "stm32g4xx_hal.h"

/* FOC */
 
#define PI		(3.141526535f)        //Pi的数学的定义
#define DIV_1_2	(0.5f)                //1除2
#define DIV_2_3	(0.66666f)            //2除3
#define Sqrt_3_div_2	(0.86602f)    //根号3除2
#define Sqrt_3_div_3    (0.57733f)    //根号3除3
#define Vdc	(12.0f)                    //假设的VBus，这个根据自己的代码修改(12/2)
#define Ts	(5000*2)	              //周期，PWM计数器        
#define SVPWM_K (1.732f * Ts / Vdc)   //这个是SVPWM的算法公式
#define POLE_PAIR_NUM 7         /* 电机极对数 */

#define Ref  0.01f			/* 采样电阻阻值 */
#define Gain  50.0f            /* 电流采样运放增益 */

#define MICROSECONDS_PER_SECOND  1000000.0f         /* 1s对应的us数量 */
#define BUFF_SIZE                256                 /* 空闲中断缓冲区大小 */

/*
注意:AS5600的地址0x36是指的是原始7位设备地址,而ST I2C库中的设备地址是指原始设备地址左移一位得到的设备地址
*/

#define Slave_Addr                0x36<<1		//设备从地址
#define Write_Bit                 0	   			//写标记
#define Read_Bit                  1    			//读标记
#define Angle_Hight_Register_Addr 0x0C 			//寄存器高位地址
#define Angle_Low_Register_Addr   0x0D 			//寄存器低位地址


#endif 
