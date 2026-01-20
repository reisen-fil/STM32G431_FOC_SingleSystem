#include "debug.h"

/* 串口重定向 */
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
 
}; 
 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
    //二选一,功能一样
    HAL_UART_Transmit (&huart1 ,(uint8_t *)&ch,1,HAL_MAX_DELAY );
		return ch;
}

/* 通过蓝牙串口对下位机状态进行控制部分 */
/* 串口接收完成回调函数 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
   if (huart->Instance == USART1)
   {
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, BUFF_SIZE); // 接收完毕后重启串口DMA模式接收数据
        switch(SlaveDevice_State)       
        {
            case DEVICE_IDLE:
                break;
            case DEVICE_CTRL_BY_VOFA:
                break;
            default:break;
        }
        __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);		   	// 手动关闭DMA_IT_HT中断
        memset(rx_buffer, 0, BUFF_SIZE);							// 清除接收缓存
   }
}	





