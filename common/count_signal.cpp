#include "count_signal.h"

string getFileName();

void countSignal(Packet_info* pkt_info)
{
    string dataDir=getMainDir()+"count_signal/";
    if(access(dataDir.c_str(),0)!=0)
    {
        if(mkdir(dataDir.c_str(),S_IRWXU | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH| S_IXOTH)!=0)
        {
            writeError("创建统计信号强度文件夹失败，应用程序停止运行");
            exit(1);
        }
    }
    //获取捕获报文时间
    //time_t rawtime=time(NULL);
    time_t rawtime=pkt_info->captime;
    struct tm * timeinfo;
    timeinfo = localtime ( &rawtime );
    char content[1024];
    memset(content,'\0',1024);
    sprintf(content,"%04d/%02d/%02d %02d:%02d:%02d\t%d",
            timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,pkt_info->rssi_signal);

    string tmpname=dataDir+getFileName();//获取当前要处理的文件名称
    //cout<<tmpname<<endl;
    ofstream writer(tmpname.c_str(),ios::app);
    cout<<content<<endl;
    writer<<content<<endl;
    writer.close();
}

//获取文件名的函数
string getFileName()
{
    char name[256];
    memset(name,'\0',256);
    //获得系统当前时间
    time_t rawtime=time(NULL);
    struct tm * timeinfo;
    timeinfo = localtime ( &rawtime );
    //根据当前时间得到文件名
    sprintf(name,"%04d%02d%02d.dat",timeinfo->tm_year+1900,
            timeinfo->tm_mon+1 , timeinfo->tm_mday /*timeinfo->tm_hour , timeinfo->tm_min */);
    return string(name);
}
