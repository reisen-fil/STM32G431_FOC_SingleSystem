#include "PMSM_FOC.h"

/* 闭环FOC执行函数 */
void ClosedLoop_PMSM_FOC_Ctrl(void)
{
		FOC_SVPWM(Iq_pid.out,Id_pid.out);						
}		

/* 零电角度校准 */
void Set_ZeroAngle(void)
{
	/* 给定适当电流实现零角度初始定位 */
	Park_DQ.theta=0*(PI/180.0);
	FOC_SVPWM(0.0f,2.5f);	/* d轴电流-->给定0电角度，以便后续对齐编码器获得的实时变化的电角度 */

}

/* 检测电流采样放大器偏置 */
// 如不考虑放大器的静态零偏，会导致电流环启动卡顿
void Check_DriftOffsets(void)
{
		MyADC_Value[0] = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);			
		MyADC_Value[1] = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);	
	
		Amp_Offset[0] = 3.3*((float)MyADC_Value[0]/4096);
		Amp_Offset[1] = 3.3*((float)MyADC_Value[1]/4096);	
}

/* FOC Algorithm Calc */
void FOC_SVPWM(float FOC_Uq,float FOC_Ud)
{
	Park_DQ.U_q=FOC_Uq;		/* q轴力矩电流 */
	Park_DQ.U_d=FOC_Ud;		/* d轴转轴部分 */

	Rev_Park(&Park_DQ,&Clark_AB);			/* 反park变换获取静止坐标系中的Ia、Ib */
	CalculateSector(Clark_AB,&FOC_ctrl);		/* 扇区计算及扇区判断 */
	CalulateDutyCycle(&FOC_ctrl,&FOC_Duty);		/* 对应扇区的占空比计算 */

	TIM1->CCR1 = FOC_Duty.Ta;		/* 占空比设置 */
	TIM1->CCR2 = FOC_Duty.Tb;
	TIM1->CCR3 = FOC_Duty.Tc;	
}

/* FOC System Running */
void FOC_Model(void)			//FOC控制环路(转速环->电流环(内环))
{
	MyADC_Value[0] = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);
	MyADC_Value[1] = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);						
	
	Current_abc.Ia = -(float)(3.3*((float)MyADC_Value[0]/4096)-Amp_Offset[0])/7.3f/0.05f;
	Current_abc.Ic = -(float)(3.3*((float)MyADC_Value[1]/4096)-Amp_Offset[1])/7.3f/0.05f;
	Current_abc.Ib = -Current_abc.Ia-Current_abc.Ic; 

//				Set_DAC(((float)(Current_abc.Ib*7.3f*0.05f)+1.65));			//Test 
	clark_transit(&Current_abc,&Clark_AB);
	Park_DQ.theta = Get_CalibAngle();					//Update ele_angle
	Park_transit(&Park_DQ,&Clark_AB);

	Ictrl_cnt++;
	if(Ictrl_cnt >= 10)		/* 位置环部分(以速度环作为内环)-->确保电流环的执行频率是转速环的10倍 */
	{
		Ictrl_cnt = 0;	

		Angle_Offset = Get_Current_Angle();		/* 获取带偏置的原始角度 */
		Position_pid.value = Angle_Offset - Zero_Angle;

		if(Position_pid.value < 0 && Po_ctrl_cnt>=10)	/* 过滤掉在对齐后一段时间对相对角度的检测 */
		{
			Position_pid.value += 360.0f;		/* 处理359->0边界 */
			if(Position_pid.value>250.0f) Position_pid.value -= 360.0f;	 	
		}
		else if(Po_ctrl_cnt<10)
		{
			Po_ctrl_cnt++;
		}
		Po_PD_Cal(&Position_pid,Po_Target_Angle,65.0f);
		
		Speed_pid.value = Get_MotorVelocity(&MotorSpeed_Calc);
		Sp_PI_Cal(&Speed_pid,Position_pid.out,15.0f);
	}	

	Id_pid.value = Park_DQ.I_d;
	Curr_PI_Cal(&Id_pid,0.0f,5.5f);
	
	Iq_pid.value = Park_DQ.I_q;
	Curr_PI_Cal(&Iq_pid,Speed_pid.out,3.0f);			//Set Iq
	
	ClosedLoop_PMSM_FOC_Ctrl();			
}


// 当 TIM1 的重复计数器为 0 时，触发 ADC1 注入组采样
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC1)
    {
		switch(Motor_State) 
		{
			case MOTOR_IDLE:
				Check_DriftOffsets();	/* 获取电流偏置 */
				Motor_State = MOTOR_INIT;
				break;
			case MOTOR_INIT:
				FOC_Stablity_Cnt++;
				switch(FOC_Stablity_Cnt)
				{
						case 1:
								Set_ZeroAngle();		//Ud = constant , theia = 0
								break;
						case 8500:		//wait for 1s
								Zero_Angle = Get_Current_Angle();		//GetAngle Zero_Bias
								Motor_State = MOTOR_START;
								FOC_Stablity_Cnt = 0;
								ZeroAlign_Flag = 1;
								break;
						default:break;
				}
				break;
			case MOTOR_START:
				FOC_Model();	
				break;							
			default:break;
		}		 
    }
}

