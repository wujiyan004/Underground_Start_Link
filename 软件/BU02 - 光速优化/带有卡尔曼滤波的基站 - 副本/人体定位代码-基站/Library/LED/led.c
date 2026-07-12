#include "led.h"



void LED_Init(void)
{
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 		 
	GPIOB->CRH&=0XF0FFFFFF; 
	GPIOB->CRH|=0X03000000;  	 
  GPIOB->ODR|=1<<14;      //PA12 输出高
	
}



