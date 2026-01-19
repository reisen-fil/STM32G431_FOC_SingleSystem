#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "Headfile.h"

int fputc(int ch, FILE *f);
void CtrlModeSet_Handle(uint8_t* Buffer);
void UART_RX_Handle(void);                    /* 串口空闲缓冲区数据处理函数 */   

#define printf myprintf                       /* 重定向后的printf打印函数(USART+DMA) */
int myprintf(const char *format,...);       

#endif




