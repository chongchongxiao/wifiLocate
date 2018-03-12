#include <iostream>
#include <unistd.h>//for readlink()
#include <string.h>//for memset()
#include <stdio.h>//for printf()
#include <vector>
using std::vector;
using std::string;

//获得程序主目录,以"\"结束,若获取失败则为""(空)
string getMainDir()
{
	char mainDir[4096];
	memset(mainDir,'\0',4096);
	//获取当前程序绝对路径，以‘/’结尾
	int cnt = readlink("/proc/self/exe",mainDir, 4096);
	if (cnt < 0 || cnt >= 4096)
	{
		return "";
	}
	//获取当前目录绝对路径，即去掉程序名
	for (int i = cnt; i >=0; --i)
	{
   	 	if (mainDir[i] == '/')
    		{
			mainDir[i+1] = '\0';
       		 	break;
   		}
	}
	return string(mainDir);
}

//打印实验室标识
void print_lab(){
	printf("\n");
	printf("*******************************************************************\n\n");
	printf("\033[32m");
	printf("    ####  ### ####  ####  ##### #########  ######    ##     ##\n");	
	printf("     ##   ##   ##    ##  ##     #########      ##  ####   ####\n");
	printf("     #######   ##    ##  ####      ###       ##      ##     ##\n");
	printf("     #######   ##    ##    ####    ###       ##      ##     ##\n");
	printf("     ##   ##   ##    ## ##   ##    ###   ##   ##     ##     ##\n");
	printf("    ####  ###   #####   ######     ###     ###      ####   ####\n\n");
	printf("\033[m");
	printf("*******************************************************************\n\n");
	printf("\n");
}

//string分割函数
vector<string> split(const string &orgin, const string &slipter)  
{  
    vector<std::string> result;//声明存放结果的容器  
    size_t pos = 0;//初始化起点为0
    size_t len = orgin.length();  
    size_t slipter_len = slipter.length();
    //若字符串或分割字符串为空则直接返回  
    if (slipter_len<=0||len<=0)
	 return result;
    //遍历字符串  
    while (pos < len)  
    {  
	//从当前游标位置向后找第一个分隔字符串
        int find_pos = orgin.find(slipter, pos);  
	//若未找到则后面为一个整体，添加并结束循环  
        if (find_pos < 0)
        {  
	    if(len-pos>0){//防止两个紧邻的分割字符串
            	result.push_back(orgin.substr(pos, len - pos));
	    }  
            break;  
        } 
	//若找到则将之前的部分添加到结果集中 
	if(find_pos-pos>0){//防止两个紧邻的分割字符串
        	result.push_back(orgin.substr(pos, find_pos - pos));
	}
	//游标后移  
        pos = find_pos + slipter_len;  
    }  
    return result;  
}

//string的startWith函数
bool startWith(const string &str, const string &head){
	if(0!=str.find(head,0))
		return false;
	return true;
}

//string的endWith函数
bool endWith(const string &str, const string &tail){
	if(str.size()<tail.size())//长度不足则直接返回
		return false;
	return str.substr(str.size()-tail.size(), tail.size()) == tail;
}

//string的remove()
string remove(const string &orgin,const string &str){
	vector<string> parts=split(orgin,str);
	string result="";
	vector<string>::iterator it;
	for(it=parts.begin();it!=parts.end();it++){
		result+=*it;
	}
	return result;
}
