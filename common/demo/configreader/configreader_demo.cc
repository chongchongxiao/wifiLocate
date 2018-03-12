#include <iostream>
#include "../../configreader.h"
using namespace std;
int main(){
	//时间服务器
	cout<<"时间服务器为:"<<getTimeServer()<<endl;
	//信道停留时间
	int staytime[13];
	getChannelStayTime(staytime);
	cout<<"信道停留时间为:";
	for(int i=0;i<13;i++){
		cout<<" "<<staytime[i];
	}
	cout<<endl;
	//wpalivetime
	cout<<"wpa解密信息存活时间:"<<getWpaLiveTime()<<endl;
	//bssidinfo
	Bssid_info* bssidlist=new Bssid_info();
	getBssidInfo(&bssidlist);
	cout<<"bssid信息"<<endl;
	while(bssidlist!=NULL){
		cout<<"phrase:"<< (bssidlist->phrase) <<",pwd:" << (bssidlist->pwd)<<",ap_mac:"<< (bssidlist->ap_mac)<<endl;
		bssidlist=bssidlist->next;
	}
	//proversionstatus
	cout<<"高级版本开关:"<<getProVersionStatus()<<endl;
}
