#ifndef __OLED_H
#define __OLED_H

#include "headfile.h"


#define  SCL	PBout(13)
#define  SDA	PBout(15)
#define  RST  PBout(11)
#define  DC	  PBout(10)
#define  CS	  PBout(12)

#define BMP_1 1  //图片宏定义
#define BMP_2 2
#define BMP_3 3
#define BMP_4 4
#define BMP_5 5


#define INT 	1  //数据类型宏定义
#define UINT  0

void OLED_Init(void);
void OLED_Fill(unsigned char bmp_dat);
void OLED_P6x8_Str(u8 x,u8 y,u8 *ch);
void OLED_P8x16_Str(u8 x,u8 y,u8 *ch);
void OLED_P16x16_Ch(u8 x,u8 y,u8 N);
void Draw_BMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP_Num);
void OLED_P6x8_Num(u8 x,u8 y,int num,u8 type);
void OLED_P8x16_Num(u8 x,u8 y,int num,u8 type);
void OLED_P6x8_Float(u8 x,u8 y,float num);
void OLED_P8x16_Float(u8 x,u8 y,float num);

#endif
