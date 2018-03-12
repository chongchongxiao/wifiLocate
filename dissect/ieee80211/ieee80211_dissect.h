/*
  功能：解析报文中ieee80211头部
  创建日期：2015.09.17
  创建人：mushao
  修改日志：
	2015.11.26	mushao	为新增加的地址数值表示添加赋值实现（stmac[6],bssid[6],以方便解密时比较用）
	2015.11.28	mushao	为新增加的帧类型域添加赋值实现（frame_type）
	2016.01.09	mushao	去除stmac和bssid，简化和整理代码
	2016.01.10	mushao	恢复stmac和bssid，解密需要
	2016.05.12	mushao	从beacon和probeResponse帧中提取ssid和加密类型，作为应用信息返回
*/
#ifndef _IEEE80211_DISSECT_H_
#define _IEEE80211_DISSECT_H_

#include "../packet_info.h"
//解析ieee80211报文的调用函数
bool dissect_ieee80211(const u_char* data, Packet_info* pkt_info);

#endif//for _IEEE80211_DISSECT_H_

