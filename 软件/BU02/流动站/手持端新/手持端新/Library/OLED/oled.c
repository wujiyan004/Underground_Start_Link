#include "oled.h"
#include "ws.h"

#define	Brightness	0xCF 	//亮度调节
#define X_WIDTH 	  128   //屏幕宽度
#define Y_WIDTH 	  64    //屏幕长度

/********************************************************************
* OLED写命令
********************************************************************/
void OLED_WrCmd(u8 cmd)
{
	u8 i=8;
	DC=0;
	SCL=0;;	
	while(i--)
	{
		if(cmd&0x80){SDA=1;}
		else{SDA=0;}
		SCL=1;	
		SCL=0;
		cmd<<=1;
	} 
}

/********************************************************************
* OLED写数据
********************************************************************/ 
void OLED_WrDat(u8 data)
{
	u8 i=8;	
	DC=1;
	SCL=0;
	while(i--)
	{
		if(data&0x80){SDA=1;}
		else{SDA=0;}
		SCL=1;	
		SCL=0;
		data<<=1;
	}
}
/********************************************************************
* OLED全屏操作 0x00 清屏
********************************************************************/ 
void OLED_Fill(u8 bmp_dat) 
{
	u8 y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}

/********************************************************************
* OLED初始化
********************************************************************/
void OLED_Init(void)
{
	RCC->APB2ENR|=1<<3;    	 		 
	GPIOB->CRH&=0X0F0000FF; 
	GPIOB->CRH|=0X30333300; 	
  GPIOB->ODR|=0XF<<10;
	GPIOB->ODR|=1<<15;
	
	CS=0; 		//使能信号段，拉低时正常使用
	RST=0;
	delay_ms(50);
	RST=1;
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness 亮度调节
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //清屏
}

/********************************************************************
* OLED 设置坐标
********************************************************************/
void OLED_Set_Pos(u8 x,u8 y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
}
/********************************************************************************
* 显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7
* OLED_P6x8_Str(0,0,"Z");或传入字符串首地址
*********************************************************************************/
void OLED_P6x8_Str(u8 x,u8 y,u8 *ch)
{
	u8 c=0,i=0;
	while (*ch!='\0')
	{
		c =*ch-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
		x+=6;
		ch++;
	}
}
/*********************************************************************************
* 显示8*16一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7
*********************************************************************************/
void OLED_P8x16_Str(u8 x,u8 y,u8 *ch)
{
	u8 c=0,i=0;
	while (*ch!='\0')
	{
		c =*ch-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		ch++;
	}
}
/*********************************************************************************
* 显示16*16点阵 显示的坐标(x,y),y为页范围0~7
*********************************************************************************/
void OLED_P16x16_Ch(u8 x,u8 y,u8 N)
{
	u8 wm=0;
	u16 adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  	
}
/*********************************************************************************
* 显示BMP图片128*64起始点坐标(x,y),x的范围是0~127,y为页范围0~7 y1-1~8
*********************************************************************************/
void Draw_BMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP_Num)
{
	u8 *BMP_Addr[]={BMP1,BMP2,BMP3,BMP4,BMP5};  //存储图片地址
	u8 x,y;
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    { 
	    	OLED_WrDat(*BMP_Addr[BMP_Num-1]++);
	    }
	}
}
/*********************************************************************************
* u16->ACSII
*********************************************************************************/
void OLED_HEXACSII(u16 hex,u8* Print)
{
	u8 hexcheck ;
	u8 TEMP ;
	TEMP = 5 ;
	Print[TEMP ]='\0';
	while(TEMP)
	{
		TEMP -- ;
		hexcheck  =  hex%10 ;
		hex   /=10 ;
		Print[TEMP ]  = hexcheck + 0x30 ;
	}  
}
/*********************************************************************************
* 显示整数(6*8),显示的坐标(x,y),y为页范围0~7 num:整数 type:整数类型 INT or UINT
*********************************************************************************/
void OLED_P6x8_Num(u8 x,u8 y,int num,u8 type)
{
	u8 i,*ch1,ch[6];
	if(type)
	{	
		if(num<0) num=-num,OLED_P6x8_Str(x,y,"-");
		else      OLED_P6x8_Str(x,y," ");
		x += 6;
	}
	OLED_HEXACSII(num,ch);
	ch1 =&ch[4];       					//当数据为零时显示零
	for(i=0;i<4;i++)
	{
		if(ch[i]!='0')
		{
			ch1 =&ch[i];
			break;
		}
	}
	for(i=0;i<5;i++)
	{
		if(*ch1 == '\0') OLED_P6x8_Str(x, y, " ");//清空无数据区域
			else
			{	
				OLED_P6x8_Str(x, y, ch1);
				ch1++;
			}
		x += 6;
	}
}
/*********************************************************************************
* 显示整数(8*16),显示的坐标(x,y),y为页范围0~7 num:整数 type:整数类型 INT or UINT
*********************************************************************************/
void OLED_P8x16_Num(u8 x,u8 y,int num,u8 type)
{
	u8 i,*ch1,ch[6];
	if(type)
	{
		if(num<0) num=-num,OLED_P8x16_Str(x,y,"-");
		else      OLED_P8x16_Str(x,y," ");
		x += 8;
	}
	OLED_HEXACSII(num,ch);
	ch1 =&ch[4];       					//当数据为零时显示零
	for(i=0;i<4;i++)
	{
		if(ch[i]!='0')
		{
			ch1 =&ch[i];
			break;
		}
	}
	for(i=0;i<5;i++)
	{
		if(*ch1 == '\0') OLED_P8x16_Str(x, y, " ");//清空无数据区域
			else
			{	
				OLED_P8x16_Str(x, y, ch1);
				ch1++;
			}
		x += 8;
	}
}
/*********************************************************************************
* 显示浮点型(6*8),显示的坐标(x,y),y为页范围0~7 num:浮点型数据
*********************************************************************************/
void OLED_P6x8_Float(u8 x,u8 y,float num)
{
	u8 i,*ch1,ch[6],fl[6];
	u8 cx;
	u16 num1,num2;
	if(num<0) num=-num,OLED_P6x8_Str(x,y,"-");
	else      OLED_P6x8_Str(x,y," ");
	x+=6;
	num1=num;
	num*=100;
	cx=x;												//记录x值
	num2=(u32)num%100;
	OLED_HEXACSII(num1,ch);
  OLED_HEXACSII(num2,fl);
	ch1 =&ch[4];       					//当数据为零时显示零
	for(i=0;i<4;i++)
	{
		if(ch[i]!='0')
		{
			ch1 =&ch[i];
			break;
		}
	}
	while(*ch1 != '\0')
	{
		OLED_P6x8_Str(x, y, ch1);	
		x += 6;
		ch1++;
	}
	OLED_P6x8_Str(x, y, ".");	
	x += 6;
	ch1 =&fl[3];
	while(*ch1 != '\0')
	{
		OLED_P6x8_Str(x, y, ch1);	
		x += 6;
		ch1++;
	}
	for(i=x;i<cx+48;i+=6) OLED_P6x8_Str(i, y, " ");//清空无数据区域
}
/*********************************************************************************
* 显示浮点型(8*16),显示的坐标(x,y),y为页范围0~7 num:浮点型数据
*********************************************************************************/
void OLED_P8x16_Float(u8 x,u8 y,float num)
{
	u8 i,*ch1,ch[6],fl[6];
	u16 num1,num2;
	u8 cx;
	if(num<0) num=-num,OLED_P8x16_Str(x,y,"-");
	else      OLED_P8x16_Str(x,y," ");
	x+=8;
	num1=num;
	num*=100;
	num2=(u32)num%100;
	cx=x;												//记录x值
	OLED_HEXACSII(num1,ch);
  OLED_HEXACSII(num2,fl);
	ch1 =&ch[4];       					//当数据为零时显示零
	for(i=0;i<4;i++)
	{
		if(ch[i]!='0')
		{
			ch1 =&ch[i];
			break;
		}
	}
	while(*ch1 != '\0')
	{
		OLED_P8x16_Str(x, y, ch1);	
		x += 8;
		ch1++;
	}
	OLED_P8x16_Str(x, y, ".");	
	x += 8;
	ch1 =&fl[3];
	while(*ch1 != '\0')
	{
		OLED_P8x16_Str(x, y, ch1);	
		x += 8;
		ch1++;
	}
	for(i=x;i<cx+64;i+=8) OLED_P8x16_Str(i, y, " ");//清空无数据区域
}





