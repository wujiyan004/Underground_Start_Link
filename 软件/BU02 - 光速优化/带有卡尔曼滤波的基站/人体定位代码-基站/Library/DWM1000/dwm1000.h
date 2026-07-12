#ifndef __DWM1000_H
#define __DWM1000_H

#include "headfile.h"

typedef signed long long int64;
typedef unsigned long long uint64;
#define TX_ANT_DLY 16460 //默认64 MHz PRF天线延迟 
#define RX_ANT_DLY 16460 //可自行修改校准16418
#define RX_BUF_LEN 24    //接收缓冲区长度
#define ALL_MSG_COMMON_LEN 10//消息中的公共字节数据
#define ALL_MSG_SN_IDX 2 /* 帧中的特定字段*/
#define FINAL_MSG_POLL_TX_TS_IDX 10
#define FINAL_MSG_RESP_RX_TS_IDX 14
#define FINAL_MSG_FINAL_TX_TS_IDX 18
#define FINAL_MSG_TS_LEN 4
#define SEND_DATA_HIGH 11
#define SEND_DATA_LOW  12
/* Delay between frames, in UWB microseconds. See NOTE 4 below. */
/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.46 ms with above configuration. */
#define POLL_RX_TO_RESP_TX_DLY_UUS 1650//回复时间延迟
/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define RESP_TX_TO_FINAL_RX_DLY_UUS 140
/* Receive final timeout. See NOTE 5 below. */
#define FINAL_RX_TIMEOUT_UUS 3000//最终超时
/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
 * 1 uus = 512 / 499.2 ? and 1 ? = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536
#define SPEED_OF_LIGHT 299792458//光速299702547//299552816//299792458
extern u8 time;
void DWM1000_Init(u8 chan);
void Dwm1000_Respond(double *dis);
void Dwm1000_Respond1(double *dis);
void Dwm1000_Respond2(double *dis);
#endif
