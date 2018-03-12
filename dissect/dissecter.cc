#include "dissecter.h"
#include "radiotap/radiotap_dissect.h"
#include "ieee80211/ieee80211_dissect.h"
#include "../common/util.h"//for PRINT_DEBUG
Dissecter::Dissecter()
{
}
Dissecter::~Dissecter()
{
}
void Dissecter::dissect(const u_char *rawData, Packet_info *pkt_info) {
	u_char *data = (u_char*) rawData; //获得源数据

	//*1 处理radiotap_header,结果保存到pkt_info结构体中
	if (!dissect_radiotap(data, pkt_info)) {
        PRINT_DEBUG("dissect.cc:信息 在radiotap解析处结束解析");
		return;
	}
    PRINT_DEBUG("dissect.cc::信息 解析radiotap_header完成");
	//*2 remove FCS if present (madwifi) (校验并去掉最后的校验位,4位)
        /*if( decrypter.check_crc_buf( data + pkt_info->rthdr_len, pkt_info->caplen - pkt_info->rthdr_len - 4 ) == 1 ){
        	pkt_info->len    -= 4;
        	pkt_info->caplen -= 4;
        PRINT_DEBUG("dissect.cc:信息 校验并去除尾部校验位,移除后报文长度为：" << pkt_info->len <<",捕获报文长度为："<< pkt_info->caplen);
        }*/
    //*2去掉最后的校验位，4位
        if(true){
            pkt_info->len    -= 4;
            pkt_info->caplen -= 4;
        PRINT_DEBUG("dissect.cc:信息 校验并去除尾部校验位,移除后报文长度为：" << pkt_info->len <<",捕获报文长度为："<< pkt_info->caplen);
        }
	//*3 处理ieee80211帧头
	if(pkt_info->caplen <= pkt_info->rthdr_len || !dissect_ieee80211(data+pkt_info->rthdr_len,pkt_info) ){
        PRINT_DEBUG("dissect.cc:信息 在ieee80211解析处结束解析");
		return;
	}
    PRINT_DEBUG("dissect.cc::信息 解析ieee80211完成");
}

