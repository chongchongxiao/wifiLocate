#include "systemfunction.h"

void* startCapture(void* args); //启动抓包程序


bool start(Capture* ct)
{
    pthread_t tId;
    int ret;
    if(!ct->setConfige())
    {
        writeError("systemfunction:打开网卡或设置过滤规则失败");
        return false;
    }
    ret=pthread_create(&tId,NULL,startCapture,ct);
    if(ret!=0)
    {
        writeError("SystemFunction:创建抓包线程失败");
        return false;
    }
    writeInfo("SystemFunction:创建抓包线程成功");
    return true;
}

bool stop(Capture* ct)
{
    //关闭网卡的时候先检查是否已经关闭，若已经关闭，则不需要关闭
    if(ct->checkHandle())
    {
        ct->stopCapture();
    }
    writeInfo("systemfunction:结束抓包程序成功");
    return true;
}


//
void* startCapture(void* args)
{
    Capture *ct=(Capture *)args;
    ct->startCapture();
    writeInfo("systemfunction:开始抓包");
}








