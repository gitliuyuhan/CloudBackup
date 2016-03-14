/*======================================================
    > File Name: tools.h
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2016年01月20日 星期三 14时35分20秒
 =======================================================*/
#pragma once

#include "package.h"

#include <string>

struct sockaddr_in; //前向声明

//获取广播地址
std::string getBroadcastAddress(void);

//获取本地IP地址
std::string getLocalIpAddress(void);

//将文件名从文件的绝对路径中提取出来
std::string extractFilename(std::string filename);

//生成UDP数据包
UdpDataPacket *makeUdpDataPacket(int number,int type,const char *data);

//生成套接字地址
void makeSockAddress(sockaddr_in &sockaddr,const char *address,int port);

