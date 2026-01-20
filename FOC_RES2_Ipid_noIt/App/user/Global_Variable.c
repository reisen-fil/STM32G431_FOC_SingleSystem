#include "Global_Variable.h"

/* Motor_State */
System_State Motor_State;
uint16_t FOC_Stablity_Cnt;

/* SlaveDevice_State */
Device_State SlaveDevice_State;

/* AS5600 */
float Zero_Angle=0;			//AS5600_Origin_Angle

/* ADC(I) */
uint16_t MyADC_Value[2];
float Amp_Offset[2];		/* 采样电压偏置 */

/* FOC */
struct Three_Phase Current_abc;		/* Origin_abc */
struct AlphaBeta_Aix Clark_AB;		/* Clark_AB */
struct DQ_Aix Park_DQ;					/* Park_DQ */
struct SVPWM_Control FOC_ctrl;
struct SVPWM_Duty FOC_Duty;

/* pid */
Str_pid Id_pid;   /* 电流环pid */
Str_pid Iq_pid;

/* debug */
uint8_t rx_buffer[BUFF_SIZE];           /* 接收缓冲区 */
uint8_t UART_RX_Flag;
volatile uint8_t usart_dma_tx_over = 1;        /* USART+DMA发送完成置位 */
float Target_Iq;                        /* 目标力矩电流Iq */


