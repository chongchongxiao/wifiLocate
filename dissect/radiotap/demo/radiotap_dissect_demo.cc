/*
  功能：测试radiotap_dissect
  创建日期：2015.09.16
  创建人：mushao
*/

#include <iostream>
#include <stdlib.h>//for malloc
#include "../radiotap_dissect.h"
using namespace std;
int main() {
	Packet_info* pkt_info = new Packet_info();
	u_char h80211[36] =			
		{0x00,0x00,0x24,0x00,0x2f,0x40,0x00,0xa0,0x20,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
		 0x8d,0xeb,0x33,0x02,0x00,0x00,0x00,0x00,0x10,0x24,0x6c,0x09,0xc0,0x00,0xb1,0x00,
 		 0x00,0x00,0xb1,0x00};
	dissect_radiotap(h80211, pkt_info);
	cout << "length:" << pkt_info->rthdr_len << endl;
	cout << "rate:" << pkt_info->data_rate << endl;
	cout << "signal:" << pkt_info->rssi_signal << endl;
	cout << "channelID:" << pkt_info->channel_id << endl;
	delete(pkt_info);
	return 0;
}

/*结果
length:36
rate:18
signal:-79
channelID:1
*/


