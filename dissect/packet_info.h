
#ifndef __PACKET_INFO_H__
#define __PACKET_INFO_H__
#include <sys/types.h>
#include <iostream>
#include <string.h>//for memset()
#include <stdint.h>//for uint16_t....
#include <stdio.h>//for sprintf()
using std::string;
/* ************************************************************************* */
/*             		     帧类型定义        			             */
/* ************************************************************************* */
#define MGT_FRAME            0x00	/* Frame type is management */
#define CONTROL_FRAME        0x01	/* Frame type is control */
#define DATA_FRAME           0x02	/* Frame type is Data */

//加密方式定义
#define ENCRYPT_TYPE_NONE	0
#define ENCRYPT_TYPE_WPA	1
#define	ENCRYPT_TYPE_PSK	2
#define	ENCRYPT_TYPE_IEEE8021X	3
#define	ENCRYPT_TYPE_FT_PSK	4
#define	ENCRYPT_TYPE_WPA_SHA256	5
#define	ENCRYPT_TYPE_PSK_SHA256	6
#define	ENCRYPT_TYPE_TDLS_TPK	7
#define	ENCRYPT_TYPE_WEP	8

//应用信息类型定义
enum APPTYPE
{
    NONE,QQ,HTTP,SSID,WECHAT
};


class Packet_info
{
	public:
	//报文捕获信息
	time_t 	captime; //捕获时间
	int 	len, //报文长度
		caplen; //捕获长度

//radiotapheader信息		
	int 	rthdr_len, //radiotapHeader的长度		
		rssi_signal, //信号强弱（使用dbm表示）
		channel_id; //频道号码
	float 	data_rate; //传输速率（Mb/s）
	bool 	datapad; //是否需要对齐（对ieee80211长度有影响）
	bool	wep_encrypted;//ap是否采用wep加密方式

//ieee80211信息
	int 	ieee80211hdr_len, //ieee80211帧头长度
		frame_type;//帧类型
	string 	sta_mac, //sta的mac字符串表示
		ap_mac;//ap的mac的字符串表示
	u_char stmac[6],//sta的mac数值表示
		bssid[6];//ap的mac数值表示
	int encrypt_type;//加密方式
	bool 	encrypted;//本报文是否加密
//应用层数据
    enum APPTYPE app_type;//应用类型
    string	app_info;//应用信息

	Packet_info()
	{
		captime=0;
		len=0;
		caplen=0;
		rthdr_len=0;
		rssi_signal=0;
		channel_id=0;
		data_rate=0.0;
		datapad=false;
		wep_encrypted=false;
		ieee80211hdr_len=0;
		frame_type=0;
		sta_mac="ff:ff:ff:ff:ff:ff";
		ap_mac="ff:ff:ff:ff:ff:ff";
		memset(stmac,0xff,6);
		memset(bssid,0xff,6);
		encrypt_type=ENCRYPT_TYPE_NONE;
		encrypted=false;
        app_type=NONE;
        app_info="NULL";
	}		
};

struct pkt_compare //there is no ().
{
    int flag;
    bool stamacFlag;//是否按stamac去重
    bool apmacFlag;//是否按apmac去重
    pkt_compare(){
        flag=0;
		stamacFlag=(bool)(flag&1);//第一位表示stamac标志
		apmacFlag=(bool)(flag&2);//第二位表示apmac标志
    }
	bool operator()(const Packet_info &a, const Packet_info &b) const
	{
		if(0==flag){//如果不去重，那么每个都不一样
			return true;
		}
		if(stamacFlag && (a.sta_mac!=b.sta_mac) ){//按stamac去重
			return a.sta_mac<b.sta_mac;	
		}
		if(apmacFlag && (a.ap_mac!=b.ap_mac) ){//按apmac去重
			return a.ap_mac<b.ap_mac;
		}
		return false;//去重条件一样，那么按重复对待
	}
};

#endif /* __PACKET_INFO_H__ */

