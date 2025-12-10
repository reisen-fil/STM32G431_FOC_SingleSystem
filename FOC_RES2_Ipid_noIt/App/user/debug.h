#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "headfile.h"

int fputc(int ch, FILE *f);
void CtrlModeSet_Handle(uint8_t* Buffer);
void UART_RX_Handle(void);                      /* 串口空闲缓冲区数据处理函数 */     

#endif 
