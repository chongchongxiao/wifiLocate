/*
  功能：将程序中用到的一些通用功能整合
  创建日期：2016.01.29
  创建人：mushao
  修改日志：
	2016.05.29	mushao	添加remove函数，最终作为解决应用信息中带有tab（'\t'）而编写
*/

#ifndef _UTIL_H_
#define _UTIL_H_


#include <iostream>
#include <vector>
using std::vector;
using std::string;

//定义调试宏
#ifdef DEBUG
using std::cout;
using std::endl;
#define PRINT_DEBUG(x) cout<<x<<endl
#else
#define PRINT_DEBUG(x) 0
#endif

string getMainDir();//获得程序主目录,以"\"结束,若获取失败则为""(空)
void print_lab();//打印实验室标识
vector<string> split(const string &orgin, const string &slipter);//string分割函数
bool startWith(const string &str,const string &head);//string的starWith函数
bool endWith(const string &str, const string &tail);//string的endWith函数
string remove(const string &orgin,const string &str);//string的remove函数
#endif
