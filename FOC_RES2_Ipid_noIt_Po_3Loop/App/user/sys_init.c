#include "sys_init.h"

/* BSP外设初始化 */
void mcu_module_init(void)
{
	HAL_OPAMP_Start(&hopamp1);				/* OPA */
	HAL_OPAMP_Start(&hopamp3);
	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);

	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);

	HAL_TIM_Base_Start(&htim2);			/* TIM2 for Speed timing */
	
//		OLED_Init();			/* OLED */
//		OLED_Clear();		
}

/* pid参数初始化 */
void pid_init(void)
{
	single_pid_Init(&Iq_pid,0.01f,0.0085f,0.0f,3.5f);			/* Iq */
	single_pid_Init(&Id_pid,0.01f,0.0085f,0.0f,3.5f);			/* Id */
	single_pid_Init(&Speed_pid,0.08f,0.002f,0.0f,1.0f);			/* Sp */
	single_pid_Init(&Position_pid,0.4f,0.0f,0.35f,15.0f);		/* Po */	
	
}

/* 下位机空闲中断接收初始化 */
void vofa_test_init(void)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rx_buffer,BUFF_SIZE);	//手动开启串口DMA模式接收数据
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);		   	//手动关闭DMA_IT_HT中断
}

/* 电机工作状态初始化 */
void motor_state_init(void)
{
	Motor_State = MOTOR_IDLE;						//Set IDLE
	HAL_ADCEx_InjectedStart_IT(&hadc1);				//Set Start_IT
}

