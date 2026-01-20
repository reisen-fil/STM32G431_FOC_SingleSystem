#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "Headfile.h"

void CtrlModeSet_Handle(uint8_t* Buffer);
void UART_RX_Handle(void);

#define printf myprintf                       /* 重定向后的printf打印函数(USART+DMA) */
int myprintf(const char *format,...);      

#endif




