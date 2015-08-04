/*
 * =====================================================================================
 *
 *       Filename:  Server.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月02日 19时40分28秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (wangweihao), 578867817@qq.com
 *        Company:  xiyoulinuxgroup
 *
 * =====================================================================================
 */

#ifndef _SERVER_H_
#define _SERVER_H_

/* c++ */
#include <iostream>
#include <string>
#include <list>   //任务队列大小和线程池大小必须相同
#include <thread>

/* c socket api */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/* 线程池 */
#include "ThreadPool.h"

enum { maxCon = 10000};

class workServer
{
    public:
        /* 构造函数，初始化ip和端口 */
        workServer(std::string m_ip, int m_port);
        /* 析构函数，关闭打开的socket */
        ~workServer();
    
        /* server运行函数 */
        int Run();

    private:
        /* register登记连接的socket */
        int Register(int fd, bool oneshot);
        /*  set nonblocking  */
        int setnonblocking(int fd);


    private:
        /* ip和端口 */
        std::string ip;
        int port;

        /* 服务端结构体 */
        struct sockaddr_in server;
        struct epoll_event events[maxCon];
        struct epoll_event ev;

        /* socket */
        int listen_fd;
        int epoll_fd;

        
        /* 线程池 */
        ThreadPool pool;

        /* 服务端对于负载均衡服务器充当Clinet */
        //Client
};

#endif
