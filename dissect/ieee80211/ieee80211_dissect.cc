#include <stdint.h>//for uint16_t....
#include <string.h>//for memcpy
#include <stdio.h>//for sprintf()
#include "../packet_info.h"//for packet_info
#include "../../common/util.h"//for PRINT_DEBUG

#ifndef roundup2
/*当y为2的n次幂时，y-1就是一个前面全是0，后面n位全是1的二进制数。取反后则前面全是1，后面n位全是0。
 于是x加上y-1后，如果原来x的后n位不全为0，则会导致第n+1位被进位，否则x和y-1相加后不影响x前面的位。
 这样，最后再用y的取反来保留前面的位，清空后面的位，就得到了一个刚好不小于x，而且低n位全部为0的数，也就是刚好不小于x的2的n次方的整数倍*/
#define	roundup2(x, y)	(((x)+((y)-1))&(~((y)-1)))//用于内存对齐，x按y的整数倍对齐，解释在上面
#endif

//从帧控制域中提取帧类型
#define FCF_FRAME_TYPE(x)    (((x) & 0x0C) >> 2)   //修改   lichong  2017.8.25
//#define FCF_FRAME_TYPE(x)    (((x) & 0xC) >> 2)

//从帧控制域中提取帧子类型
#define FCF_FRAME_SUBTYPE(x) (((x) & 0xF0) >> 4)

//将帧类型和子类型从帧控制域转换为MGT_, CTRL_, or DATA_ values之一
#define COMPOSE_FRAME_TYPE(x) (((x & 0x0C)<< 2)+FCF_FRAME_SUBTYPE(x))//类型在高四位，子类型在第四位

//数据帧子类型
#define DATA_FRAME_IS_CF_ACK(x)  ((x) & 0x01)
#define DATA_FRAME_IS_CF_POLL(x) ((x) & 0x02)
#define DATA_FRAME_IS_NULL(x)    ((x) & 0x04)
#define DATA_FRAME_IS_QOS(x)     ((x) & 0x08)

//从帧控制域中提取flags
#define FCF_FLAGS(x)           (((x) & 0xFF00) >> 8)

//flags域的各个位
#define FLAG_TO_DS		0x01
#define FLAG_FROM_DS		0x02
#define FLAG_MORE_FRAGMENTS	0x04
#define FLAG_RETRY		0x08
#define FLAG_POWER_MGT		0x10
#define FLAG_MORE_DATA		0x20
#define FLAG_PROTECTED		0x40
#define FLAG_ORDER		0x80

//从数据帧的帧控制域中提取出地址类型标识
#define FCF_ADDR_SELECTOR(x) ((x) & ((FLAG_TO_DS|FLAG_FROM_DS) << 8))
//数据帧地址类型定义
#define DATA_ADDR_T1         0
#define DATA_ADDR_T2         (FLAG_FROM_DS << 8)
#define DATA_ADDR_T3         (FLAG_TO_DS << 8)
#define DATA_ADDR_T4         ((FLAG_TO_DS|FLAG_FROM_DS) << 8)

//帧长度定义
#define DATA_SHORT_HDR_LEN     24
#define DATA_LONG_HDR_LEN      30
#define MGT_FRAME_HDR_LEN      24

//COMPOSE_FRAME_TYPE()计算出的管理帧子类型定义
#define MGT_ASSOC_REQ        0x00	/* association request        */
#define MGT_ASSOC_RESP       0x01	/* association response       */
#define MGT_REASSOC_REQ      0x02	/* reassociation request      */
#define MGT_REASSOC_RESP     0x03	/* reassociation response     */
#define MGT_PROBE_REQ        0x04	/* Probe request              */
#define MGT_PROBE_RESP       0x05	/* Probe response             */
#define MGT_BEACON           0x08	/* Beacon frame               */
#define MGT_ATIM             0x09	/* ATIM                       */
#define MGT_DISASS           0x0A	/* Disassociation             */
#define MGT_AUTHENTICATION   0x0B	/* Authentication             */
#define MGT_DEAUTHENTICATION 0x0C	/* Deauthentication           */
#define MGT_ACTION           0x0D	/* Action */

//COMPOSE_FRAME_TYPE()计算出的控制帧的子类型定义
#define CTRL_BLOCK_ACK_REQ   0x18	/* Block ack Request		    */
#define CTRL_BLOCK_ACK	     0x19	/* Block ack			    */
#define CTRL_PS_POLL         0x1A	/* power-save poll               */
#define CTRL_RTS             0x1B	/* request to send               */
#define CTRL_CTS             0x1C	/* clear to send                 */
#define CTRL_ACKNOWLEDGEMENT 0x1D	/* acknowledgement               */
#define CTRL_CFP_END         0x1E	/* contention-free period end    */
#define CTRL_CFP_ENDACK      0x1F	/* contention-free period end/ack */

//COMPOSE_FRAME_TYPE()计算出的数据帧的子类型的定义
#define DATA                        0x20	/* Data                       */
#define DATA_CF_ACK                 0x21	/* Data + CF-Ack              */
#define DATA_CF_POLL                0x22	/* Data + CF-Poll             */
#define DATA_CF_ACK_POLL            0x23	/* Data + CF-Ack + CF-Poll    */
#define DATA_NULL_FUNCTION          0x24	/* Null function (no data)    */
#define DATA_CF_ACK_NOD             0x25	/* CF-Ack (no data)           */
#define DATA_CF_POLL_NOD            0x26	/* CF-Poll (No data)          */
#define DATA_CF_ACK_POLL_NOD        0x27	/* CF-Ack + CF-Poll (no data) */

#define DATA_QOS_DATA               0x28	/* QoS Data                   */
#define DATA_QOS_DATA_CF_ACK        0x29	/* QoS Data + CF-Ack	      */
#define DATA_QOS_DATA_CF_POLL       0x2A	/* QoS Data + CF-Poll		  */
#define DATA_QOS_DATA_CF_ACK_POLL   0x2B	/* QoS Data + CF-Ack + CF-Poll	  */
#define DATA_QOS_NULL		    0x2C	/* QoS Null			  */
#define DATA_QOS_CF_POLL_NOD	    0x2E	/* QoS CF-Poll (No Data)		  */
#define DATA_QOS_CF_ACK_POLL_NOD    0x2F	/* QoS CF-Ack + CF-Poll (No Data) */

//报文是否加密
#define IS_PROTECTED(x)			((x) & FLAG_PROTECTED)

//管理帧域代码
#define	TAG_SSID	0
#define TAG_RSN_IE                    48
//wifi名称最大值
#define	MAX_SSID_LEN	32
//RSN标签长度
#define	TAG_RSN_IE_LEN	20

//计算帧头长度
static int find_header_length(uint16_t fcf)
{
	switch (FCF_FRAME_TYPE(fcf))
	{
		case MGT_FRAME: //管理帧
			return MGT_FRAME_HDR_LEN;//固定为24
		case CONTROL_FRAME: //控制帧
			switch (COMPOSE_FRAME_TYPE(fcf))
			{
				case CTRL_CTS:
				case CTRL_ACKNOWLEDGEMENT:
					return 10;
				case CTRL_RTS:
				case CTRL_PS_POLL:
				case CTRL_CFP_END:
				case CTRL_CFP_ENDACK:
				case CTRL_BLOCK_ACK_REQ:
				case CTRL_BLOCK_ACK:
					return 16;
			}
			return 4;
		case DATA_FRAME: //数据帧
			{
				int len = (FCF_ADDR_SELECTOR(fcf) == DATA_ADDR_T4) ? DATA_LONG_HDR_LEN : DATA_SHORT_HDR_LEN;//addr4是否存在
				if (DATA_FRAME_IS_QOS(COMPOSE_FRAME_TYPE(fcf)))//Qos是否存在
					return len + 2;
				else
					return len;
			}
		default:
			return 4;
	}
}

//从beacon和proberesponse帧中获得wifi名和加密方式
static void getBssidInfo(const u_char* data, Packet_info* pkt_info){
	int data_len=(pkt_info->caplen) - (pkt_info->rthdr_len) - (pkt_info->ieee80211hdr_len);//剩余报文长度
	int offset=12;//beacon和proberesponse帧固定域长度都是12字节
	if(data_len<=offset){
		PRINT_DEBUG("ieee80211_dissect.cc:错误 报文长度不足");
		return;
	}
	uint8_t tag_no,tag_len;//标签编号和长度
	string ssid;
	while(offset<data_len-1){
		tag_no=(uint8_t)data[offset];
		tag_len=(uint8_t)data[offset+1];
		if(offset+2+tag_len>data_len){//长度超标则退出循环
			break;
		}
		switch (tag_no) {//根据不同的编号做不同的处理
			case TAG_SSID:{//获取ssid
				//处理ssid长度
				int ssid_len=tag_len;
				if(ssid_len > MAX_SSID_LEN){
					ssid_len=MAX_SSID_LEN;
				}
				char _ssid[64]={0};
				memcpy(_ssid,data+offset+2,ssid_len);
				ssid+=_ssid;
				break;
			}
			case TAG_RSN_IE:{//获取加密类型
				if(TAG_RSN_IE_LEN==tag_len){
					pkt_info->encrypt_type=data[offset+19];
				}
				break;
			}
			default:break;
			
		}
		offset+=2+tag_len;
	}
	//将ssid和加密方式写入报文应用信息
	if(ssid.size()>0){
		pkt_info->app_type=SSID;
		string enc_type="NONE";
		if(pkt_info->wep_encrypted){
			pkt_info->encrypt_type=ENCRYPT_TYPE_WEP;
			enc_type="WEP";
		}else{
			switch(pkt_info->encrypt_type){
				case ENCRYPT_TYPE_WPA : enc_type="WPA";break;
				case ENCRYPT_TYPE_PSK : enc_type="PSK";break;
				case ENCRYPT_TYPE_IEEE8021X : enc_type="FT over IEEE 802.1X";break;
				case ENCRYPT_TYPE_FT_PSK : enc_type="FT using PSK";break;
				case ENCRYPT_TYPE_WPA_SHA256 : enc_type="WPA (SHA256)";break;
				case ENCRYPT_TYPE_PSK_SHA256 : enc_type="PSK (SHA256)";break;
				case ENCRYPT_TYPE_TDLS_TPK : enc_type="TDLS / TPK Handshake";break;
				default:break;
			}
		}
		pkt_info->app_info=string("SSID:")+ssid+",EncryptType:"+enc_type;
	}
}

/****************************************************************
****************	解析ieee80211函数入口	*****************
*****************************************************************/
bool dissect_ieee80211(const u_char* data, Packet_info* pkt_info) {
	if ((pkt_info->caplen) - (pkt_info->rthdr_len) < 2) {
        PRINT_DEBUG("ieee80211_dissect.cc:错误 报文长度不足");
		return false;
	}	
	uint16_t fcf=*((uint16_t*)data); //获取帧控制域fcf(小端存储，直接获取就可以)
	int hdr_len = find_header_length(fcf); //获取长度
	if (pkt_info->datapad) //如果radiotap中pad为true，按4字节对齐
		hdr_len = roundup2(hdr_len, 4); 
	if ((pkt_info->caplen) - (pkt_info->rthdr_len) < hdr_len) {
        PRINT_DEBUG("ieee80211_dissect.cc:错误 报文长度不足"<< (pkt_info->caplen - pkt_info->rthdr_len) <<":"<<hdr_len);
		return false;
	}
	uint16_t flags = FCF_FLAGS(fcf); //提取标识
	int isprotected = IS_PROTECTED(flags);//获取是否加密
	int frame_type=FCF_FRAME_TYPE(fcf);//帧类型

	char sta_mac[18];//手机mac
	memset(sta_mac,0,18);
	u_char stmac[6];//手机mac数值表示
	memset(stmac,0,6);
	
	char ap_mac[18];//ap mac
	memset(ap_mac,0,18);
	u_char bssid[6];//ap的mac数值表示
	memset(bssid,0,6);
	switch (frame_type) {	
		case MGT_FRAME: //管理帧
			//如果是beacon或proberesponse,提取ssid和加密类型作为应用信息
			if(MGT_BEACON ==COMPOSE_FRAME_TYPE(fcf) || MGT_PROBE_RESP==COMPOSE_FRAME_TYPE(fcf)){
				getBssidInfo(data+hdr_len,pkt_info);
			}
			char src[18];
			memset(src,0,18);
			char dst[18];
			memset(dst,0,18);
			sprintf(ap_mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[16], data[17],
					data[18], data[19], data[20], data[21]);
			sprintf(dst, "%02x:%02x:%02x:%02x:%02x:%02x", data[4], data[5], data[6],
					data[7], data[8], data[9]);
			sprintf(src, "%02x:%02x:%02x:%02x:%02x:%02x", data[10], data[11],
					data[12], data[13], data[14], data[15]);
			//与ap不同的就是客户端
			if(strcmp(dst,ap_mac)!=0)
			{
				sprintf(sta_mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[4], data[5], data[6],
					data[7], data[8], data[9]);
				memcpy(stmac,data+4,6);
			}
			else if(strcmp(src,ap_mac)!=0)
			{
				sprintf(sta_mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[10], data[11],
					data[12], data[13], data[14], data[15]);
				memcpy(stmac,data+10,6);
			}
			else
			{
				sprintf(sta_mac,"ff:ff:ff:ff:ff:ff");
				memset(stmac,0xff,6);
			}
		break;

		//控制帧对本项目没有什么用，所以不做处理
		case CONTROL_FRAME: //控制帧
			return false;

		case DATA_FRAME: //数据帧的处理
			{
				uint32_t addr_type = FCF_ADDR_SELECTOR(fcf);
				switch (addr_type) 
				{
					case DATA_ADDR_T1://在该类型下只有ap
						sprintf(ap_mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[16], data[17],
								data[18], data[19], data[20], data[21]);
						memcpy(bssid,data+16,6);
						sprintf(sta_mac, "ff:ff:ff:ff:ff:ff");
						memset(stmac,0xff,6);	
						break;

					case DATA_ADDR_T2://设备mac从4开始，ap mac从10开始
						sprintf(sta_mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[4], data[5],
								data[6], data[7], data[8], data[9]);
						memcpy(stmac,data+4,6);
						sprintf(ap_mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[10], data[11],
								data[12], data[13], data[14], data[15]);
						memcpy(bssid,data+10,6);
						break;

					case DATA_ADDR_T3://设备mac从10开始，ap mac从4开始
						sprintf(ap_mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[4], data[5],
								data[6], data[7], data[8], data[9]);
						memcpy(bssid,data+4,6);
						sprintf(sta_mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[10], data[11],
								data[12], data[13], data[14], data[15]);
						memcpy(stmac,data+10,6);
						break;

					case DATA_ADDR_T4://对项目无用
						return false;
				}
			}
			break;
		default:
            PRINT_DEBUG("ieee80211_dissect.cc:错误 未知帧类型错误");
			return false;//防止出现意外情况
	}

	//保存结果
	pkt_info->ieee80211hdr_len = hdr_len;
    PRINT_DEBUG("ieee80211_dissect.cc:信息 ieee80211hdr_len:" << pkt_info->ieee80211hdr_len);
	pkt_info->frame_type=frame_type;//获取帧类型
    //pkt_info->frame_type=fcf;//获取帧类型
	PRINT_DEBUG("ieee80211_dissect.cc:信息 frame_type:" << pkt_info->frame_type);
	pkt_info->ap_mac=string(ap_mac);
	PRINT_DEBUG("ieee80211_dissect.cc:信息 ap_mac:" << pkt_info->ap_mac);
	pkt_info->sta_mac=string(sta_mac);
	PRINT_DEBUG("ieee80211_dissect.cc:信息 sta_mac:" << pkt_info->sta_mac);
	memcpy(pkt_info->stmac,stmac,6);
	memcpy(pkt_info->bssid,bssid,6);
	pkt_info->encrypted=isprotected?true:false;
	PRINT_DEBUG("ieee80211_dissect.cc:信息 encrypted:" << pkt_info->encrypted);
	return true;
}


