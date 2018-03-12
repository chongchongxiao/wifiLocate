/*
  功能：测试ieee80211_dissect的功能
  创建日期：2015.09.17
  创建人：mushao
*/

#include <iostream>
#include "../ieee80211_dissect.h"
using namespace std;
int main() {
	u_char data[26] = {0x88,0x0a,0x02,0x01,0xd0,0x22,0xbe,0xe0,0x69,0x9c,0x7a,0x69,0x6c,0x05,0xc2,0x08,
			   0x14,0x14,0x4b,0x80,0xe5,0xd6,0xf0,0x14,0x00,0x00};
	Packet_info* pkt_info = new Packet_info();
	pkt_info->caplen = 26;
	pkt_info->rthdr_len = 0;
	pkt_info->datapad = false;
	if (!dissect_ieee80211(data, pkt_info)) {
		cout << "Dissection failed！" << endl;
		return -1;
	}
	cout << "ieee80211hdr_len: " << pkt_info->ieee80211hdr_len << endl;
	cout << "frame_type: " << pkt_info->frame_type << endl;
	cout << "sta_mac: " << pkt_info->sta_mac << endl;
	cout << "ap_mac: " << pkt_info->ap_mac << endl;
	cout << "encrypted:" << pkt_info->encrypted<<endl;
	delete(pkt_info);
	return 0;
}

/*结果
frame_type2
sta_mac: d0:22:be:e0:69:9c
ap_mac: 7a:69:6c:05:c2:08
encrypted:0
*/

