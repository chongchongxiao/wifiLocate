/*
  功能：解析报文的radiotap部分
  创建日期：2015.09.16
  参考代码：wireshark0.99.2/epan/packet-radiotap.c
  创建人：mushao
  修改记录：
	2016.01.08	mushao	参照wireshark1.11.2相关代码，
				修改频率转频道方法
				对多present情况进行折中（只分析第一个，但是要计算多个长度）
				考虑内存对其对代码进行改进（radiotap的数据域都是按8\16\32\64内存对齐的）
	2016.05.11	mushao	radiotap中包含有是否使用wep加密的标志位，获取并添加到报文信息中
*/
#ifndef _RADIOTAP_DISSECT_H_
#define _RADIOTAP_DISSECT_H_

#include "../packet_info.h"
//处理radiotap_header的调用函数，结果存在pkt_info结构体中
bool dissect_radiotap(const u_char* data, Packet_info* pkt_info); 

#endif
