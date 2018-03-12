/*
  功能：对外提供解析报文的接口，对内通过调用不同层的解析方法对报文进行分层解析（包括加密报文解密）
  创建日期：2015.11.26
  创建人：mushao
*/
#ifndef _DISSECTER_H_
#define _DISSECTER_H_

#include "packet_info.h"
class Dissecter
{
	public:
		Dissecter();
		~Dissecter();
		void dissect(const u_char *data,Packet_info *pkt_info);
	private:
};
#endif
