#include "capture.h"

Capture::Capture()
{
    macAdr="ff:ff:ff:ff:ff:ff";
    memset(errbuf,'\0',PCAP_ERRBUF_SIZE);
    num=0;
    snaplen=-1;
}

Capture::Capture(string macAdr)
{
    this->macAdr=macAdr;
    memset(errbuf,'\0',PCAP_ERRBUF_SIZE);
    num=0;
    snaplen=-1;
}

Capture::~Capture()
{

}


void Capture::analysis(u_char *args, const pcap_pkthdr *header, const u_char *data)
{
    //创建数据存储对象
    Packet_info* pkt_info = new Packet_info();
    //记录捕获时间
    pkt_info->captime = header->ts.tv_sec;
    //表示抓到的数据长度
    pkt_info->len = header->len;
    //表示数据包的实际长度
    pkt_info->caplen = header->caplen;
    //解析捕获的数据包并存入pkt_info
    Dissecter dissecter;//解析对象
    dissecter.dissect(data,pkt_info);

    cout<<endl<<pkt_info->sta_mac<<endl;

    countSignal(pkt_info);
    delete(pkt_info);
}

void Capture::startCapture()
{
    pcap_loop(handle,snaplen,analysis,NULL);
}

void Capture::stopCapture()
{
    pcap_close(handle);
}


void Capture::setMacAddress(string mac)
{
    this->macAdr=mac;
}

string Capture::getMacAddress()
{
    return macAdr;
}

bool Capture::setConfige()
{
    string str;//过滤规则表达式
    if(handle)
    {
        writeError("capture:未释放已打开网卡句柄");
        return false;
    }
    handle = pcap_open_live("wlx3c46d81f5624", BUFSIZ, 1, 0, errbuf);
    if(!handle)
    {
        writeError("capture:打开网卡失败");
        return false;
    }
    writeInfo("capture:打开网卡成功");
    str="ether src "+macAdr;
    pcap_compile(handle, &filter,str.c_str(), 1, 0);
    pcap_setfilter(handle, &filter);
    writeInfo("capture:设置过滤规则成功");
    return true;
}

bool Capture::checkHandle()
{
    if(!handle)
    {
        return false;
    }
    return true;
}


