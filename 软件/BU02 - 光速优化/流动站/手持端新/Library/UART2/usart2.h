#ifndef __USART2_H
#define __USART2_H


#include "headfile.h"


void uart2_init(u32 pclk2,u32 bound);
void UART2_Putchar(u8 ch);
void URAT2_Putbuff(u8 *buff,u16 len);

#endif
