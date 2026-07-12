#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
void uart_init(u32 pclk2,u32 bound);
void URAT1_Putbuff(u8 *buff,u16 len);
void UART1_Putchar(u8 ch);
#endif	   
















