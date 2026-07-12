#include "key.h"

//按键初始化
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟  
	GPIOA->CRL&=0XFFFFFF0F;	//PA1 设置成输入	  
	GPIOA->CRL|=0X00000080;
	GPIOA->ODR|=1<<1;	   	//PA1 上拉
	
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟  
	GPIOC->CRH&=0X000FFFFF;	//PC13 PC14 PC15 设置成输入	  
	GPIOC->CRH|=0X88800000;
	GPIOC->ODR|=7<<13;	   	//PC13 PC14 PC15 上拉
}

//按键处理函数
//返回按键值
u8 KEY_Scan(void)
{	     
 	return (S1<<3|S2<<2|S3<<1|S4<<0);// 无按键按下
}
