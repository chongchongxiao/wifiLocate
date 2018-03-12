#include <iostream>
#include <stdlib.h> // exit()
#include <unistd.h> // sleep()
#include "systemfunction.h"
#include "capture.h"
#include "common/util.h"
#include "common/logwriter.h"
using namespace std;

int main()
{
    Capture *ct = new Capture("ec:df:3a:6d:aa:d0");
    if(!start(ct))
    {
        writeInfo("wifiLocate:创建抓包线程失败");
        exit(1);
    }
    sleep(300);//主线程休眠5min，即抓包5分钟
    delete(ct);

    return 0;
}
