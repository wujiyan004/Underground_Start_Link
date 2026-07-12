#include "usart.h"	  
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数      
u8 Usart_ON_Flag = 0, Usart_ON_Flag1 = 0, Usart_Flag1, buf[25]; //双缓冲




struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
}
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
void UART1_Putchar(u8 ch)
{
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = ch;      	    //写DR,串口1将发送数据
}
void URAT1_Putbuff(u8 *buff,u16 len)
{
	while(len--)
	{
    UART1_Putchar(*buff);
	  buff++;
	}
}

#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
void USART1_IRQHandler(void)
{
	if(USART1->SR&(1<<5))	//接收到数据
	{	 
					u8 temp1;
					static u8 count1,last_data1,last_last_data1,Usart_ON_Count1;
					if(Usart_ON_Flag1==0)
					{	
						if(++Usart_ON_Count1>14)Usart_ON_Flag1=1;
					}
					temp1=USART1->DR;
					
				   if(Usart_Flag1==0)
						{	
						if(last_data1==0x0c&&last_last_data1==0xbb) 
						Usart_Flag1=1,count1=0;	
						}
					 if(Usart_Flag1==1)
						{	
							U1_RX_BUF[count1]=temp1;     
							count1++;    		
							if(count1==12)Usart_Flag1=0,flag_cs1 = 1;
						}
						last_last_data1=last_data1;
						last_data1=temp1;	

	}
} 
#endif										 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(1,1,USART1_IRQn,2);//组1
#endif
}
