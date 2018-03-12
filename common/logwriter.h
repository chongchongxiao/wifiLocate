/*
  功能：向其它源文件提供记录日志的方法
  创建日期：2015.09.25
  创建人：mushao
修改日志：
  2016.01.28	mushao	将相关函数的参数从char*转换为string
  2016.04.12	mushao	修复并发写日志文件的bug
*/

#ifndef _LOGWRITER_H_
#define _LOGWRITER_H_

#include <iostream>
using std::string;
void writeInfo(const string info);//向日志中写入信息（正常日志信息）
void writeError(const string error);//向日志中写入错误（错误日志信息）

#endif
