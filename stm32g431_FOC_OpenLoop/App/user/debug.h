#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "Headfile.h"

int fputc(int ch, FILE *f);
void CtrlModeSet_Handle(uint8_t* Buffer);
void UART_RX_Handle(void);

#endif




