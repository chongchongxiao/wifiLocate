#ifndef COUNT_SIGNAL_H
#define COUNT_SIGNAL_H

#include <iostream>
#include <fstream>
#include <stdio.h> //sprintf
#include <unistd.h>//for access & close()
#include <sys/stat.h>//for mkdir()
#include <sys/types.h>//for mkdir()
#include <stdlib.h> // exit()
#include "../dissect/packet_info.h"
#include "util.h"
#include "logwriter.h"
using namespace std;

void countSignal(Packet_info* pkt_info);//统计信号强度


#endif // COUNT_SIGNAL_H
