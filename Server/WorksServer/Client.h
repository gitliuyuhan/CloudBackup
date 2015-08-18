/*
 * =====================================================================================
 *
 *       Filename:  Client.h
 *
 *    Description:  
 *
 *
 *        Version:  1.0
 *        Created:  2015年08月05日 08时10分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (wangweihao), 578867817@qq.com
 *        Company:  xiyoulinuxgroup
 *
 * =====================================================================================
 */

/* 
 * 每太worker服务器都会有监控本地的负载的client，
 * 将自己的负载信息传送给负载均衡服务器，负载服
 * 务器进行分析，然后将结果传递给master服务器 
 * */

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <thread>
#include "getInfo.h"


/* 定时器执行函数 */
void signalHandler(int sig);


class ServerClient
{
    public:
        /* 构造函数，初始化ip和端口 */
        ServerClient(std::string i, int p);

        friend void signalHandler(int sig);

    private:
        /* 负载均衡服务器的端口和ip */
        std::string ip;
        int port;

        /* 定时器设置 */
        struct itimerval new_value, old_value;

        /* Client结构体 */
        struct sockaddr_in server;

        /* 获取负载 */
        getDate info;

};

#endif 
