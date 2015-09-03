/*
 * =====================================================================================
 *
 *       Filename:  Client.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月05日 08时36分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (wangweihao), 578867817@qq.com
 *        Company:  xiyoulinuxgroup
 *
 * =====================================================================================
 */

#include "Client.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>

/* 初始化监控本地负载的client */

static int loadServer_fd;
std::ifstream in("loadInfo.txt");

/* 定时器处理定时事件，发送给负载server自己的负载信息 */
void 
signalHandler(int sig)
{
    char buffer[1024] = "hello world\n";
    switch(sig)
    {
        case SIGALRM:
            printf("caught the SIGALRM signal\n");
            printf("get local load info\n");
           
            /* 获得带宽信息 */
            /* 后期可以加上cpu和磁盘io的信息 */
            std::cout << "execute command" << std::endl;
            std::string info;
            getline(in, info);
            info += "\n\0";
            const char *p = info.c_str();
            printf("p = %s\n", p);
            std::cout << "load:" << info << std::endl;

            /* send loadserver */
            send(loadServer_fd, p, strlen(p), 0);
            break;
    }
}

ServerClient::
ServerClient(std::string i, int p):
    ip(i), port(p)
{
    /* 初始化socket */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    loadServer_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(loadServer_fd < 0)
    {
        perror("create loadServer socket fd error\n");
        exit(1);
    }
    int con_ret = connect(loadServer_fd, (struct sockaddr*)&server, sizeof(server));
    if(con_ret == -1)
    {
        perror("load Serverconnect error\n");
        exit(1);
    }

    /*  
    std::thread tid([&](){
            // 设置定时器，并初始化 
            signal(SIGALRM, signalHandler);
            new_value.it_value.tv_sec = 0;
            new_value.it_value.tv_usec = 1;
            new_value.it_interval.tv_sec = 5;
            new_value.it_interval.tv_usec = 0;
            setitimer(ITIMER_REAL, &new_value, &old_value);
            // 获得带宽信息 
            while(1);
        }
    );
    */
    char buffer[16];
    std::thread tid([&](){
                while(1)
                {
                    std::string getinfo;
                    recv(loadServer_fd, buffer, 16, 0);
                    getinfo += info.getInfo(cpuInfo);
                    getinfo += "\0\n";
                    getinfo += info.getInfo(memInfo);
                    getinfo += "\0\n";
                    getinfo += info.getInfo(netInfo);
                    getinfo += "\0\n";
                    getinfo += info.getInfo(diskInfo);
                    getinfo += "\0\n";
                    std::cout << getinfo << std::endl;
                    send(loadServer_fd, getinfo.c_str(), getinfo.size(), 0);
                    //sleep(4);
                }
            });
    tid.detach();
}


