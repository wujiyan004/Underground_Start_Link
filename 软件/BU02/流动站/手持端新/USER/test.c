#include "headfile.h"
#include "adc.h"
//JTAG模式设置定义
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	
#define N 12
double Distance_Station_One,Distance_Station_Two,Distance_Station_Three,Distance_Station_Four,Distance_Station_Five,Distance_Station_Six,d_7,d_8,d_9,d_10,d_11,d_12,d_13,d_14;//距离
u16 D_1,D_2,D_3,D_4,D_5,D_6,D_7,D_8,D_9,D_10,D_11,D_12,D_13,D_14,D_15,D_16,D_17,D_18,D_19,D_20,D_21,D_22,D_23,D_24;
uint8_t TAG_ID,ANCHOR_ID, jumptime=0;
int x1,y1,x2,y2,x3,y3;
int flag_bz=0;
float r1,r2,r3,r4,r5,r6;
float L12=918,L23=910,L34,L45,L13,L14,L15,L24,L25,L35,H=0;//基站两两之间的距离
float jd[2];
float jd_v[2];
double flag_cs1=0,flag_cs2=0,flag_cs=0;
u8 jd_R[2];
u8 rx[4];
u8 rx_num1=0;
u8 NUM=0;
u8 flag=0;
u8 flag_y=0;
u8 flag_x=0;
u8 iii=0;
u8 flag_over=0;
u8 flag_over1=0;
u8 jd_x[10];
u8 jd_y[10];
u8 Usart_Flag4,Usart_ON_Flag4;
u8 U4_RX_BUF[10];
float b,theta1,theta2,X,Y,X1,Y1,Y2,X2;
float xyd1[2],xyd2[2];
float max1,max2,max3,min1,min2,min3;
int time;
	int wjyqq;
double POSX,POSY,POS_X_max=8.3,POS_X_min=8.3,POS_Y_min=1.0,POS_Y_max=1.0;
#define GPIOC_CLK (*(volatile unsigned int *)(0x40021000 + 0x18)) 
#define GPIOC_CRH (*(volatile unsigned int *)(0x40011000 + 0x04))	
#define GPIOC_ODR (*(volatile unsigned int *)(0x40011000 + 0x0C))	
double D1,D2;
int main(void)
{
	u16 adcx=4050;
	Stm32_Clock_Init(9);	  //系统时钟设置
	delay_init(72);	  		  //延时初始化
	uart_init(72,115200); 	//串口初始化为115200
	uart2_init(72,115200);
	LED_Init();             //LED初始化
	Adc_Init();		  		//ADC初始化	    
	OLED_Init();            //OLED初始化
	KEY_Init();             //拨码开关初始化
	DWM1000_Init();         //DWM1000初始化
		delay_ms(100);

	
		GPIOC_CLK |= (1<<4);             
		GPIOC_CRH &= ~(0xf<<(4*5));  		 
		GPIOC_CRH |= (1<<(4*5));	
	//FMQ=1;
		GPIOC_ODR &= (0x1<<(1*13)); //叫
		GPIOC_ODR &= ~(0x1<<(1*13)); //不叫
	delay_ms(100);
	LED=0;
	FMQ=0;
	delay_ms(300);


  while(1)
	{		
//FMQ=0;
//delay_ms(1000);		
//	FMQ=1;

		
	Dwm1000_Start(&Distance_Station_Two,1);//基站二		//13.535
		Dwm1000_Start(&Distance_Station_One,2);//与基站一通信//6.541
		Dwm1000_Start(&Distance_Station_Five,6);//与基站一通信
	 Dwm1000_Start(&Distance_Station_Six,7);//与基站一通信
		
////		if(Distance_Station_Five >=0 && Distance_Station_Five<=1000)
////		{
////		Distance_Station_Five = Distance_Station_Five -50;
////			Distance_Station_Six = Distance_Station_Six -50;
////		}
////		else if(Distance_Station_Five >=1000 && Distance_Station_Five<=2000)
////		{
////			Distance_Station_Five = Distance_Station_Five -100;
////			Distance_Station_Six = Distance_Station_Six -100;
////		}
////		
////		
		
		
	 	D1 = Distance_Station_Five/1000.0;
	    D2 = Distance_Station_Six/1000.0;
	

////		POSX = (D1*D1-D2*D2+7.7*7.7)/15.4;
////		POSY = sqrt(fabs(D1*D1-POSX*POSX));
////		

////	
////		
////	
////			if(POSX > POS_X_max)
////			{
////			POS_X_max = POSX;
////			}

////			if(POSX < POS_X_min )
////			{
////			POS_X_min = POSX;
////			}
////			
////			
////			
////		
////			
////			if(POSY > POS_Y_max)
////			{
////			POS_Y_max = POSY;
////			}

////			if(POSY < POS_Y_min )
////			{
////			POS_Y_min = POSY;
////			}
////			
////			
////		
		
		
		
		
		
		
		
		
		
		
		
		
	
		
		
////		Dwm1000_Start(&d_7,5);//与基站一通信
		Dwm1000_Start(&d_8,6);//与基站一通信
////		
		Dwm1000_Start(&d_9,7);//与基站一通信
////		Dwm1000_Start(&d_10,8);//与基站一通信
//		Dwm1000_Start(&d_11,9);//与基站一通信
//		Dwm1000_Start(&d_12,10);//与基站一通信
//		Dwm1000_Start(&d_13,11);//与基站一通信
//		Dwm1000_Start(&d_14,12);//与基站一通信
	}	 
} 