#ifndef SYSTEMFUNCTION_H
#define SYSTEMFUNCTION_H

#include <iostream>
#include <stdio.h>
#include <pcap/pcap.h>
#include <string.h>
#include <pthread.h> //pthread_t
#include "capture.h" //Capture
#include "dissect/packet_info.h"//for packet_info
#include "dissect/dissecter.h"//for Dissecter
#include "dissect/dissecter.h"
#include "common/logwriter.h"
using namespace std;

bool start(Capture *ct);//创建抓包线程
bool stop(Capture *ct);//通过关闭网卡的方式结束线程



#endif // SYSTEMFUNCTION_H
