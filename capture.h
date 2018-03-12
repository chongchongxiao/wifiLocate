#ifndef CAPTURE_H
#define CAPTURE_H

#include <iostream>
#include <stdio.h>
#include <pcap/pcap.h>
#include <string.h>
#include <pthread.h> //pthread_t
#include "dissect/packet_info.h"//for packet_info
#include "dissect/dissecter.h"//for Dissecter
#include "dissect/dissecter.h"
#include "common/logwriter.h"
#include "common/count_signal.h"
using namespace std;

class Capture
{
private:
    string macAdr;  //mac地址
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle;   //网卡句柄
    int snaplen; //表示抓到包snaplen个包后pcap_loop返回，负数则一直抓包
    struct bpf_program filter; //过滤规则
    int num;  //抓包数量
    pthread_t tId; //创建线程启动抓包程序

    //内部作为参数的函数必须声明为静态才可以使用
    static void analysis(u_char *args, const pcap_pkthdr *header, const u_char *data); //解析回调函数

public:
    Capture();
    Capture(string macAdr);
    ~Capture();
    bool setConfige();//打开网卡 和设置过滤规则
    void startCapture(); //开始
    void stopCapture();  //停止
    void setMacAddress(string macAdr); //重新设置mac地址
    string getMacAddress();  //获得当前mac地址
    bool checkHandle();//检查handle是否打开，没有打开则没有运行抓包程序
};

#endif // CAPTURE_H
