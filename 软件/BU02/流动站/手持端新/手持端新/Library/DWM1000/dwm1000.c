#include "dwm1000.h"
#include "port.h"
#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_sleep.h"
//DWM1000初始化参数设置
/* Default communication configuration. We use here EVK1000's default mode (mode 3). */
static dwt_config_t config = {
	2,               /* Channel number. */
	DWT_PRF_64M,     /* Pulse repetition frequency. */
	DWT_PLEN_1024,   /* Preamble length. */
	DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
	0,               /* TX preamble code. Used in TX only. */
	0,               /* RX preamble code. Used in RX only. */
	1,               /* Use non-standard SFD (Boolean) */
	DWT_BR_110K,     /* Data rate. */
	DWT_PHRMODE_STD, /* PHY header mode. */
	(1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};
static uint8 tx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'N', 'G', 0x21, 0, 0};//测距使用特定帧段
static uint8 rx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'N', 'G', 'W', 'A', 0x10, 0x02, 0, 0, 0, 0};
static uint8 tx_final_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'N', 'G', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8 frame_seq_nb = 0;//帧序号 每次传递后递增
static uint32 status_reg = 0;//保存状态寄存器的值用于断点检查
static uint8 rx_buffer[RX_BUF_LEN];//接收数据缓冲区
static uint64 poll_tx_ts;//时间点
static uint64 resp_rx_ts;
static uint64 final_tx_ts;
static uint64 get_tx_timestamp_u64(void)//获取TX时间戳
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
static uint64 get_rx_timestamp_u64(void)//获取RX时间戳
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
static void final_msg_set_ts(uint8 *ts_field, uint64 ts)//最终消息发送时间戳
{
	int i;
	for (i = 0; i < FINAL_MSG_TS_LEN; i++)
	{
		ts_field[i] = (uint8) ts;
		ts >>= 8;
	}
}
void DWM1000_Init(void)
{
	peripherals_init();
	reset_DW1000();//复位
  spi_set_rate_low();//将DWM1000 SPI时钟设置为3M
	while(dwt_initialise(DWT_LOADUCODE) == -1);//校正DW1000
	spi_set_rate_high();//将DWM1000 SPI时钟设置为20M
	dwt_configure(&config);//配置DWM1000
	dwt_setrxantennadelay(RX_ANT_DLY);	//设置天线延迟 需要自行校准
	dwt_settxantennadelay(TX_ANT_DLY);
	dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);//设置相应的延时与超时
	dwt_setrxtimeout(RESP_RX_TIMEOUT_UUS);
}
void Dwm1000_Start(u16 *dis,u32 station)
{
	config.txCode=config.rxCode=station+8;
	dwt_configure(&config);//配置DWM1000
	tx_poll_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
	dwt_writetxdata(sizeof(tx_poll_msg), tx_poll_msg, 0);
	dwt_writetxfctrl(sizeof(tx_poll_msg), 0);//将数据传入DW1000准备传输
	dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);//启动传输
	while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)));//记录状态寄存器的值并轮询错误信息
  frame_seq_nb++;//帧序号递增
	if (status_reg & SYS_STATUS_RXFCG)//如果接收成功
  {
		static uint32 frame_len;//接收数据长度
		dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);//清除 RX TX事件
		frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;//接收数据  计算接收帧的数据长度
		if (frame_len <= RX_BUF_LEN)//接收到帧后放入接收缓冲区
		{
		  dwt_readrxdata(rx_buffer, frame_len, 0);
		}
		
		
		
		rx_buffer[ALL_MSG_SN_IDX] = 0;//清除不相关字段简化验证
		
		
		
		

		if (memcmp(rx_buffer, rx_resp_msg, ALL_MSG_COMMON_LEN) == 0)//判断是不是返回数据
    {
			uint32 final_tx_time;
			*dis=rx_buffer[SEND_DATA_HIGH]*256+rx_buffer[SEND_DATA_LOW];//接收测距数据
			poll_tx_ts = get_tx_timestamp_u64();//获取时间戳
			resp_rx_ts = get_rx_timestamp_u64();
			final_tx_time = (resp_rx_ts + (RESP_RX_TO_FINAL_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;//计算最终消息发送时间
			dwt_setdelayedtrxtime(final_tx_time);
			
			if(station == 0x05)
			{
						final_tx_ts = (((uint64)(final_tx_time & 0xFFFFFFFE)) << 8) + 16520;//最终TX时间为编程传输时间加天线延迟
			}
			else
			{
			final_tx_ts = (((uint64)(final_tx_time & 0xFFFFFFFE)) << 8) + 16340;//最终TX时间为编程传输时间加天线延迟
			}
			
/////////final_tx_ts = (((uint64)(final_tx_time & 0xFFFFFFFE)) << 8) + TX_ANT_DLY;//最终TX时间为编程传输时间加天线延迟
			final_msg_set_ts(&tx_final_msg[FINAL_MSG_POLL_TX_TS_IDX], poll_tx_ts);//在最后消息上写上所有时间戳
			final_msg_set_ts(&tx_final_msg[FINAL_MSG_RESP_RX_TS_IDX], resp_rx_ts);
			final_msg_set_ts(&tx_final_msg[FINAL_MSG_FINAL_TX_TS_IDX], final_tx_ts);
			tx_final_msg[ALL_MSG_SN_IDX] = frame_seq_nb;//写并发送最后的消息
			dwt_writetxdata(sizeof(tx_final_msg), tx_final_msg, 0);
			dwt_writetxfctrl(sizeof(tx_final_msg), 0);
			dwt_starttx(DWT_START_TX_DELAYED);//启动传输	
			while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS));//等待发送完成	
      dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);//清除TX事件标志
			frame_seq_nb++;
			//delay_ms(1);
		//	 deca_sleep(100+0);//休眠固定时间
		}
		else
		{
		//	delay_ms(5);
		//	 deca_sleep(100+5);//休眠固定时间
		}
	}
	else 
	{
	dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);//否则清除RX错误事件	
		 //deca_sleep(100+5);//休眠固定时间
		//delay_ms(5);
	}
	
	
}