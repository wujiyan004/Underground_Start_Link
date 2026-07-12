#include "headfile.h"
#include "kalman.h"

double distance, distance1, distance2, Last_distance, Last_distance1, Last_last_distance1, Last_distance2; //距离
double Real_dsi = 0;
float WJY1993 = 0.0;
float WJY11 = 0.0;
u8 time;//接收时间缓存变量
u16 Dis;
int rx;
float UWB_ttyusb_1 = 0, UWB_tty_USB_2 = 0;
int _distance_G = 0, _distance_max_ = 10000, _distance_min_ = 0, _distance_L = 0, _distance_G1 = 0, _distance_L1 = 0, _distance_G2 = 0, _distance_L2 = 0;
float Y = 0;
int UWB_ID = 0;
u8 LED1 = 0, LED2 = 0, LED3 = 0;
u8 U1_RX_BUF[50];
double MAX_dis = 0, Min_dis = 2000;
int flag_cs1 = 0;
short	int Yaw = 1, Pitch = 1, Roll = 1;
double Ya, Pi, Ro;



double RSSI, RSSI_N, RSSI_C, TEST_D;


u8 testbuffer[2];

double test_POS;



double real_DIS;



int main(void)
{
    u8 State;
    Stm32_Clock_Init(9);	  //系统时钟设置
    delay_init(72);	  		  //延时初始化
    uart_init(72, 115200); 	//串口初始化为115200
    uart2_init(72, 115200);
//    LED_Init();             //LED初始化
//    OLED_Init();            //OLED初始化
//    KEY_Init();             //拨码开关初始化
//    OLED_P16x16_Ch(0, 0, 4);
//    OLED_P16x16_Ch(16, 0, 5);
//    OLED_P16x16_Ch(0, 3, 16);
//    OLED_P16x16_Ch(16, 3, 17);
//    OLED_P8x16_Str(32, 3, ":");
//    OLED_P16x16_Ch(112, 3, 18);
//    OLED_P8x16_Str(0, 6, "T");
//    OLED_P8x16_Str(8, 6, "D");
//    OLED_P8x16_Str(16, 6, "O");
//    OLED_P8x16_Str(24, 6, "A");
//    OLED_P16x16_Ch(32, 6, 19);
//    OLED_P16x16_Ch(47, 6, 20);
//    OLED_P8x16_Str(60, 6, ":");
//    OLED_P8x16_Str(66, 6, "b");
//    OLED_P8x16_Str(74, 6, "y");
//    OLED_P16x16_Ch(83, 6, 21);
//    OLED_P16x16_Ch(98, 6, 22);
//    OLED_P16x16_Ch(113, 6, 23);
    UWB_ID = 5;
    DWM1000_Init(5);//根据拨码开关选择基站号
    delay_ms(1000);




    UART1_Putchar(0xaa);
    UART1_Putchar(0x00);
    UART1_Putchar(0x3d);
    UART1_Putchar(0x08);
    UART1_Putchar(0x0a);
    delay_ms(10);



    UART1_Putchar(0xaa);
    UART1_Putchar(0x00);
    UART1_Putchar(0x3d);
    UART1_Putchar(0x08);
    UART1_Putchar(0x0a);


//            UART2_Putchar(0xaa);
//            UART2_Putchar(0x00);
//            UART2_Putchar(0x3d);
//            UART2_Putchar(0x01);
//            UART2_Putchar(0x0a);







    while(1)
    {

//		delay_ms(10);
//
//		    UART1_Putchar(0xaa);
//            UART1_Putchar(0x01);
//            UART1_Putchar(0x1a);
//            UART1_Putchar(0x0c);

//
//

//
//
//
        delay_ms(10);

        UART1_Putchar(0xaa);
        UART1_Putchar(0x01);
        UART1_Putchar(0x28);
        UART1_Putchar(0x0c);

//

//
//
//
        if(flag_cs1)//IMU检测
        {
            Roll = ((U1_RX_BUF[1] << 8) |  U1_RX_BUF[0]);
            Pitch = ((U1_RX_BUF[3] << 8) |  U1_RX_BUF[2]);
            Yaw = ((U1_RX_BUF[5] << 8) |  U1_RX_BUF[4]);
            Ya = Yaw / 16.0;
            Pi = Pitch / 16.0;
            Ro = Roll / 16.0;

            flag_cs1 = 0;
        }


//
//
//















        Dwm1000_Respond(&distance);//得到与标签之间的距离

        if(distance <= 50.0 )
        {
            //WJY1993 = distance;
            WJY1993 =	distance;
            KalMan_Update(&WJY1993);
            //  LED = ~LED; //状态灯闪烁
            Y = WJY11 * 100;
            _distance_G = Y / 256;
            _distance_L = Y - _distance_G * 256;

            testbuffer[0] = _distance_G;
            testbuffer[1] = _distance_L;





            if(Y < 100)
            {
                test_POS = Y + 15;


            }
            else if( Y >= 100 && Y < 150)
            {
                test_POS = Y * 0.87719 + 27.63;
            }
            else if( Y >= 150 && Y < 200)
            {
                test_POS = Y * 0.917 + 22.02;
            }

            else if( Y >= 200 && Y < 250)
            {
                test_POS = Y * 0.98 + 9.8;
            }

            else if( Y >= 250 && Y < 300)
            {
                test_POS = Y * 0.98 + 9.8;
            }

            else if( Y >= 300 && Y < 350)
            {
                test_POS = Y * 0.926 + 25.926 ;
            }


            else if( Y >= 350 && Y < 400)
            {
                test_POS = Y ;
            }


            else if( Y >= 400 && Y < 450)
            {
                test_POS = Y * 1.136 - 54.54 ;
            }

            else if( Y >= 450 && Y < 500)
            {
                test_POS = Y * 0.952 + 27.14 ;
            }

            else if( Y >= 500 && Y < 550)
            {
                test_POS = Y * 0.9345 + 35.98 ;
            }




            else if( Y >= 550 && Y < 600)
            {
                test_POS = Y * 1.031 - 17.01 ;
            }



            else if( Y >= 600 && Y < 650)
            {
                test_POS = Y * 1.053 - 30 ;
            }



            else if( Y >= 650 && Y < 700)
            {
                test_POS = Y * 1.04 - 22.91 ;
            }




            else if( Y >= 700 && Y < 750)
            {
                test_POS = Y * 0.893 + 80.35 ;
            }

            else if( Y >= 750 && Y < 800)
            {
                test_POS = Y * 1.0417 - 31.25 ;
            }


            else if( Y >= 800 && Y < 850)
            {
                test_POS = Y + 2 ;
            }


            else if( Y >= 850 && Y < 900)
            {
                test_POS = Y * 0.98 + 18.63 ;
            }


            else if( Y >= 900 && Y < 950)
            {
                test_POS = Y * 1.087 - 77.03 ;
            }








            real_DIS = test_POS / 100.0;








////////			Real_dsi = distance*100 ;
////////
////////
////////			if(Real_dsi > MAX_dis)
////////			{
////////			MAX_dis = Real_dsi;
////////			}
////////
////////
////////
////////						if(Real_dsi < Min_dis && Real_dsi!=0)
////////			{
////////			Min_dis = Real_dsi;
////////			}






////
////            UART1_Putchar(0xff);
////            UART1_Putchar(0xfe);
////            UART1_Putchar(_distance_G);
////            UART1_Putchar(_distance_L);
////            UART2_Putchar(0xff);
        }

    }

}