#include <iostream>
#include <fstream>
#include <unistd.h>//for access & close()
#include <stdio.h>//for sprintf
#include <stdlib.h>//for exit()
#include <sys/stat.h>//for mkdir()
#include <sys/types.h>//for mkdir()
#include <time.h>
#include <string.h>//for memset()
#include <sys/file.h>//for flock()
#include <fcntl.h>//for open()
#include "util.h"//for PRINT_DEBUG() && getMainDir()
using namespace std;

//获得当前要写入的log文件名称
static string getLogFileName()
{
	//获得系统当前时间
	time_t rawtime=time(NULL);
	struct tm * timeinfo;
	timeinfo = localtime ( &rawtime );
	char fileName[256];
	memset(fileName,'\0',256);
	//根据当前时间得到文件名(一个月一个log文件)
	sprintf(fileName,"%04d%02d.log", timeinfo->tm_year+1900 , timeinfo->tm_mon+1);
	return string(fileName);
}

//将内容写入日志
static void writeLog(const string content)
{
	//获得主目录
	string logDir=getMainDir()+"log/";
	if(access(logDir.c_str(),0)!=0)
	{
		if(mkdir(logDir.c_str(),S_IRWXU | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH| S_IXOTH)!=0)
		{
			PRINT_DEBUG("创建日志文件夹失败，应用程序停止运行");
			exit(1);
		}
	}
	string fullName=logDir+getLogFileName();
	//写入日志
	ofstream oconf(fullName.c_str(),ios::app);
	//添加互斥锁
	int fd = open(fullName.c_str(), O_WRONLY | O_CREAT,0777);
	flock(fd, LOCK_EX);
	
	//写入内容
	PRINT_DEBUG(content);
	oconf << content <<endl;
	oconf.close();

	//释放锁
	flock(fd, LOCK_UN);
	close(fd);	
}

//将信息写入日志
void writeInfo(const string info)
{
	//获取系统当前时间
	time_t rawtime=time(NULL);
	struct tm * timeinfo;
	timeinfo = localtime ( &rawtime );
	char content[1024];
	memset(content,'\0',1024);
	sprintf(content,"%04d/%02d/%02d %02d:%02d:%02d\tinfo\t%s", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,info.c_str());
	writeLog(string(content));	 
}

//将错误写入日志
void writeError(const string Error)
{
	//获取系统当前时间
	time_t rawtime=time(NULL);
	struct tm * timeinfo;
	timeinfo = localtime ( &rawtime );
	char content[1024];
	memset(content,'\0',1024);
	sprintf(content,"%04d/%02d/%02d %02d:%02d:%02d\terror\t%s", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,Error.c_str());
	writeLog(string(content)); 
}
