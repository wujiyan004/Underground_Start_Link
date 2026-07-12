#include "dwm1000.h"
#include "port.h"
#include "deca_device_api.h"
#include "deca_regs.h"
#include <math.h>
//DWM1000初始化参数设置
/* Default communication configuration. We use here EVK1000's default mode (mode 3). */
static dwt_config_t config =
{
    5,               /* Channel number. *///信道2频率范围为3744.0-4243.2Mhz,可以设置为信道5为6240.0-6739.2Mhz
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_64,   /* Preamble length. */
    DWT_PAC8,       /* Preamble acquisition chunk size. Used in RX only. */
    0,               /* TX preamble code. Used in TX only. */
    0,               /* RX preamble code. Used in RX only. */
    1,               /* Use non-standard SFD (Boolean) */
    DWT_BR_110K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (65 + 64 - 8) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};
u16 timee;
static uint32 status_reg = 0;//保存状态寄存器的值用于断点检查
static uint8 rx_buffer[RX_BUF_LEN];//接收数据缓冲区
static uint8 rx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'N', 'G', 0x21, 0, 0};//测距使用帧段
static uint8 tx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'N', 'G', 'W', 'A', 0x10, 0x02, 0, 0, 0, 0};
static uint8 rx_final_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'N', 'G', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint64 poll_rx_ts;//接收时间缓存变量
static uint64 resp_tx_ts;//发送时间
static uint64 final_rx_ts;//后接收时间
static uint8 frame_seq_nb = 0;//帧序号 每次传递后递增
static double tof;//飞行时间
	double  Test_data;
	
	
	

	
float dwGetReceivePower(void) {
	
	float C,N;
	 float twoPower17 = 131072.0;
    dwt_rxdiag_t *diagnostics;
    dwt_readdiagnostics(diagnostics);
   C = (&diagnostics->stdNoise)[3];
   N = diagnostics->rxPreamCount;

 
  return N;
}

















// 存储信号强度相关数据的结构体
typedef struct {
    int16_t rssi;           // 接收信号强度指示
    uint8_t fppl_count;     // 前导码检测计数
    uint8_t std_noise;      // 噪声标准差
} dw1000_signal_info_t;

// 读取DW1000的信号强度信息
uint8_t dw1000_get_signal_strength(dw1000_signal_info_t *info) {
    uint8_t reg_data[4];
    
    if (info == NULL) {
        return 0; // 参数错误
    }
    
    // 读取RSSI值 (范围: -140dBm 到 -40dBm)
dwt_readfromdevice(RX_FINFO_ID, 0, 2, reg_data);  // 确保读取2字节
RSSI_N = (int16_t)(reg_data[0] | (reg_data[1] << 8));  // 注意字节序
    
    // 读取前导码检测计数
    dwt_readfromdevice(RX_TIME_ID, 5, 1, &reg_data[0]);
    RSSI = reg_data[0] & 0x7F;
    
    // 读取噪声标准差
    dwt_readfromdevice(RX_FQUAL_ID, 2, 1, &reg_data[0]);
    info->std_noise = reg_data[0];
    RSSI_C = reg_data[0];
    return 1; // 成功读取
}
















	
	
	
	
	
static uint64 get_rx_timestamp_u64(void)//获取接收时间
{
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readrxtimestamp(ts_tab);

    for (i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= ts_tab[i];
    }

    return ts;
}
static uint64 get_tx_timestamp_u64(void)//获取发送时间
{
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readtxtimestamp(ts_tab);

    for (i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= ts_tab[i];
    }

    return ts;
}
static void final_msg_get_ts(const uint8 *ts_field, uint32 *ts)
{
    int i;
    *ts = 0;

    for (i = 0; i < FINAL_MSG_TS_LEN; i++)
    {
        *ts += ts_field[i] << (i * 8);
    }
}

void DWM1000_Init(u8 chan)
{

    config.txCode = config.rxCode = chan+8;


    peripherals_init();//spi初始化
    reset_DW1000();//复位
    spi_set_rate_low();//将DWM1000 SPI时钟设置为3M

    while(dwt_initialise(DWT_LOADUCODE) == -1);//校正DW1000

    spi_set_rate_high();//将DWM1000 SPI时钟设置为20M
	
	
	
	

	
	
    dwt_configure(&config);//配置DWM1000
    dwt_setrxantennadelay(RX_ANT_DLY);	//设置天线延迟 需要自行校准
    dwt_settxantennadelay(TX_ANT_DLY);
	//dwt_setdblrxbuffmode(1);
	//dwt_setsmarttxpower(1);//智能电源控制
	
	
		
	
	
	
	
	
	
	
}










u8 test;
	   dw1000_signal_info_t signal_info;

	


















	
void Dwm1000_Respond(double *dis)
{
    dwt_setrxtimeout(0);//清除接收超时 进行下次测距
    dwt_rxenable(0);    //使能接收
	

		
			//flag_cs1 = 	dwt_read16bitoffsetreg(0x10,2);
		
		
		

	
	
//RSSI = dwt_read32bitoffsetreg(0x1e,0);
	
	
//	dwt_read32bitreg(0x1e);
//	
//	 dwt_readrxdata(RSSI, 4, 0);
//	
//	
//	
	
	
	
   // 
    while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR))){   };//记录状态寄存器的值并轮询错误信息

    if (status_reg & SYS_STATUS_RXFCG)//如果接受机正常
    {
			
        static uint32 frame_len;//接收数据长度

        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);//清除DW1000状态寄存器中RX帧事件
        frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;//计算接收帧的数据长度

        if (frame_len <= RX_BUFFER_LEN)//接收到帧后放入接收缓冲区
        {
            dwt_readrxdata(rx_buffer, frame_len, 0);
        }

        rx_buffer[ALL_MSG_SN_IDX] = 0;//清除不相关字段简化验证

        if (memcmp(rx_buffer, rx_poll_msg, ALL_MSG_COMMON_LEN) == 0)//轮询 DS TWR发起者 验证公共字节是否相同/        /先看看是不是第一次第一次接受的数据
        {
            uint32 resp_tx_time;
            poll_rx_ts = get_rx_timestamp_u64();//获取接收时间
            time = poll_rx_ts;
            resp_tx_time = (poll_rx_ts + (POLL_RX_TO_RESP_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;//////////////////////////////
            dwt_setdelayedtrxtime(resp_tx_time);//设置发送响应时间
            dwt_setrxaftertxdelay(RESP_TX_TO_FINAL_RX_DLY_UUS);///////////////////////////////////////////////////////
            dwt_setrxtimeout(FINAL_RX_TIMEOUT_UUS);//设置最终消息接收的预期延迟与超时
            tx_resp_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
            /************************************************************/
            if(*dis < 0) *dis = -*dis;
			//Test_data = *dis *100;
			
			tx_resp_msg[SEND_DATA_HIGH] = testbuffer[0]; //将上次测距数据发过去
            tx_resp_msg[SEND_DATA_LOW] = testbuffer[1];
			
			
////			
////            tx_resp_msg[SEND_DATA_HIGH] = (u16)  *dis >> 8; //将上次测距数据发过去
////            tx_resp_msg[SEND_DATA_LOW] = (u16) *dis;
            /*************************************************************/
            dwt_writetxdata(sizeof(tx_resp_msg), tx_resp_msg, 0);                                     //发送一次测距数据给出去
            dwt_writetxfctrl(sizeof(tx_resp_msg), 0);
            dwt_starttx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED);//编写并发送响应消息

            while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)));//记录状态寄存器的值并轮询错误信息 等待接收数据

			
			
			
			
			
			
			
				//	test=	dwt_readwakeuptemp();
			
			
			
			
				
			
			
			
			
			
			
            frame_seq_nb++;//帧序号递增

            if (status_reg & SYS_STATUS_RXFCG)//如果接受机正常 
            {
				
					u8 test_H;
                dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);//清除RX TX帧事件
				
				
				
			
					

				
				
				
                frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;//接收帧

                if (frame_len <= RX_BUF_LEN)
                {
                    dwt_readrxdata(rx_buffer, frame_len, 0);
					
					
					
					
					
					
				
					
					
					
					
                }

                rx_buffer[ALL_MSG_SN_IDX] = 0;//清除不相关字段简化验证

                if (memcmp(rx_buffer, rx_final_msg, ALL_MSG_COMMON_LEN) == 0)//轮询 DS TWR发起者                 //然后再等待手持端回来的数据是不是自己想要的
                {
                    uint32 poll_tx_ts, resp_rx_ts, final_tx_ts;
                    uint32 poll_rx_ts_32, resp_tx_ts_32, final_rx_ts_32;
                    double Ra, Rb, Da, Db;
                    int64 tof_dtu;
                    resp_tx_ts = get_tx_timestamp_u64();//获取发送响应时间
                    final_rx_ts = get_rx_timestamp_u64();//获取最后接收时间
                    final_msg_get_ts(&rx_buffer[FINAL_MSG_POLL_TX_TS_IDX], &poll_tx_ts);//获取最后时间戳
                    final_msg_get_ts(&rx_buffer[FINAL_MSG_RESP_RX_TS_IDX], &resp_rx_ts);
                    final_msg_get_ts(&rx_buffer[FINAL_MSG_FINAL_TX_TS_IDX], &final_tx_ts);
                    poll_rx_ts_32 = (uint32)poll_rx_ts;//计算飞行时间
                    resp_tx_ts_32 = (uint32)resp_tx_ts;
                    final_rx_ts_32 = (uint32)final_rx_ts;
                    Ra = (double)(resp_rx_ts - poll_tx_ts);
                    Rb = (double)(final_rx_ts_32 - resp_tx_ts_32);
                    Da = (double)(final_tx_ts - resp_rx_ts);
                    Db = (double)(resp_tx_ts_32 - poll_rx_ts_32);
                    tof_dtu = (int64)((Ra * Rb - Da * Db) / (Ra + Rb + Da + Db));
                    tof = tof_dtu * DWT_TIME_UNITS;
                    *dis = tof * SPEED_OF_LIGHT;
					
							//	dw1000_get_signal_strength(&signal_info);
					//TEST_D = tof * SPEED_OF_LIGHT;
		
												//RSSI = 	dwt_read32bitoffsetreg(0x12,1);
				
			
				
//				test_H = RSSI/256;
			//	UWB_ttyusb_1 = 10*log10((RSSI_C*127*1024)/(RSSI*RSSI));
				//*dis = *dis - 0.07*log10((RSSI_C))  ; // ln(10) 的近似值;

					
				//	deca_sleep(100);
                }
            }
            else 
			{
				//deca_sleep(100+5);//休眠固定时间
			dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);//否则清除RX错误事件
			}
        }
    }
    else 
	{
		//deca_sleep(100+5);//休眠固定时间
	dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);//否则清除RX错误事件
	}
}


