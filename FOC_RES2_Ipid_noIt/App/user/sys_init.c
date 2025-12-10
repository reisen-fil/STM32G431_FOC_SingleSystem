#include "sys_init.h"

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
}

void pid_init(void)
{
		Ipid_Init(&Iq_pid,0.02f,	0.0085f,0.0f,3.5f);		/* Iq  */
		Ipid_Init(&Id_pid,0.02f,0.0085f,0.0f,3.5f);		/* Id  */
	
}

/* 下位机空闲中断接收初始化 */
void vofa_test_init(void)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rx_buffer,BUFF_SIZE);	//手动开启串口DMA模式接收数据
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);		   	//手动关闭DMA_IT_HT中断
}

void motor_state_init(void)
{
		Motor_State = MOTOR_IDLE;									//Set IDLE
		HAL_ADCEx_InjectedStart_IT(&hadc1);				//Set Start_IT
}

