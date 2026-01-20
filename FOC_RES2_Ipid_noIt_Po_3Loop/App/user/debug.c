#include "debug.h"

/* 串口+DMA重定向printf */
int myprintf(const char *format,...)
{
  va_list arg;
  static char SendBuff[200] = {0};
  int rv;
  while(!usart_dma_tx_over);//等待前一次DMA发送完成
 
  va_start(arg,format);
  rv = vsnprintf((char*)SendBuff,sizeof(SendBuff)+1,(char*)format,arg);
  va_end(arg);
 
  HAL_UART_Transmit_DMA(&huart1,(uint8_t *)SendBuff,rv);
  usart_dma_tx_over = 0;//清0全局标志，发送完成后重新置1
 
  return rv;
}
 
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
  		 usart_dma_tx_over = 1;
	}
 
}

/* 通过蓝牙串口对下位机状态进行控制部分 */
/* 串口接收完成回调函数 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
   if (huart->Instance == USART1)
   {
        if(UART_RX_Flag == 0)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, BUFF_SIZE); // 接收完毕后重启串口DMA模式接收数据
            __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);		   	// 手动关闭DMA_IT_HT中断
            UART_RX_Flag = 1;
        }

   }
}	

/* ORE_error test */
void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    if(huart->Instance == USART1)
    {
      if (huart->ErrorCode == HAL_UART_ERROR_ORE)
      {
		      __HAL_UART_CLEAR_OREFLAG(huart);       /* 清除ORE位 */ 
          HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, BUFF_SIZE); // 接收发生错误后重启
          __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);		   // 手动关闭DMA_IT_HT中断
      }        
    }
}

/* 串口空闲缓冲区数据处理函数 */
void UART_RX_Handle(void)
{
    if(UART_RX_Flag)
    {
        switch(SlaveDevice_State)       
        {
            case DEVICE_IDLE:
                if(!strcmp(rx_buffer,"DEVICE_C_B_V")) SlaveDevice_State 
                    = DEVICE_CTRL_BY_VOFA;    /* 进入上位机初始选择状态 */
                break;
            case DEVICE_CTRL_BY_VOFA:
                if(!strcmp(rx_buffer,"DEVICE_IDLE")) SlaveDevice_State 
                    = DEVICE_IDLE;            /* 回到进入上位机控制初始状态前的默认状态 */
                else if(!strcmp(rx_buffer,"Po_C_Mode")) SlaveDevice_State 
                    = CTRL_MODE;              /* 进入上位机控制状态 */
                break;                    
            case CTRL_MODE:
                CtrlModeSet_Handle(rx_buffer);      /* 进入上位机速度控制传参 */
                if(!strcmp(rx_buffer,"Mode_Return")) SlaveDevice_State 
                    = DEVICE_CTRL_BY_VOFA;       /* 退出该控制模式,返回到上位机选择初始状态 */
                break;
            default:break;
        }
        UART_RX_Flag = 0;
        memset(rx_buffer, 0, BUFF_SIZE);							// 清除接收缓存
    }

}


/* 整数/小数字符串-->对应整型变量和浮点数变量 */

/* x除以n次10 */
float Pow_invert(uint8_t X,uint8_t n) 
{
  float result=X;
	while(n--)
	{
		result/=10;
	}
	return result;
}

//uint8_t Get_Vofa_RxData[5]={0x31,0x32,0x2E,0x31,0x33};//可以给数据包直接赋值直接调用一下换算程序，看是否输出为12.13
//Data_BitNum = 5//别忘记数据的长度也要设置
//然后直接在主程序就放  Printf("%f\n",RxPacket_Data_Handle());  Delay_ms(1000);就ok了

/* 数据包换算处理 */
float RxPacket_Data_Handle(uint8_t* Vofa_data_bag)
{
  float Data=0.0;
  uint8_t dot_Flag=0;	/* 小数点标志位，能区分小数点后或小数点前 0为小数点前，1为小数点后 */
  uint8_t dot_after_num=1;	/* 小数点后的第几位 */
  int8_t minus_Flag=1;				/* 负号标志位 -1为是负号 1为正号 */
  for(uint8_t i=0;i<10;i++)
  {
    if(Vofa_data_bag[i+6]==0x2D)		/* 如果第一位为负号 */
    {
      minus_Flag=-1;
      continue;		/* 跳过本次循环 */ 
    }
    if(dot_Flag==0)
    {
      if(Vofa_data_bag[i+6]==0x2E) /* 如果识别到小数点，则将dot_Flag置1 */
      {
        dot_Flag=1;
      }
      else  /* 还没遇到小数点前的运算 */
      {
        Data = Data*10 + Vofa_data_bag[i+6]-48;
      }
    }
    else  /* 遇到小数点后的运算 */
    {
      Data = Data + Pow_invert(Vofa_data_bag[i+6]-48,dot_after_num);
      dot_after_num++;
    }
  }
  return Data*minus_Flag; /* 将换算后的数据返回出来 这里乘上负号标志位 */

}

/* 上位机传参条件判断 */
void CtrlModeSet_Handle(uint8_t* Buffer)
{
    if(!strncmp(Buffer,"Posit=",6)) Motor_TargetAngle = RxPacket_Data_Handle(Buffer);

}











