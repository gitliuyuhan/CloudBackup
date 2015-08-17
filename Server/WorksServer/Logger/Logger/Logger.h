/*
 * (C) 2007-2015 Alibaba Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Logger.h -- 
 *
 * Version: 1.0  2015年08月17日 21时19分09秒
 *
 * Authors:
 *     wangweihao (), wangweihao@xiyoulinux.org
 *
 */


/* 
 * 多缓冲实现一个能处理
 * 并发日志消息的日志库
 * */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <memory>
#include <list>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* 缓冲区类 */
#include "Buffer.h"

#define INFO 1  //输出日志消息
#define ERROR 2 //输出错误消息
#define FAIL 3  //输出操作失败消息

bool isHave();

/* 已满缓冲的数量 */
size_t readableNum;


class Logger
{
    public:
        /* 初始化一些信息 */
        Logger();
        /* 按格式将日志信息写入到指定文件 */
        void logStream(const char* msg, int len, char* rank);

    private:
        /* 是否有可读缓冲Buffer */
        bool isHave();
        /* 得到可用的缓冲 */
        std::shared_ptr<Buffer> useful();
        /* 线程读取后台缓冲区函数 */
        void threadFunc();

    private:
        /* 当前缓冲 */
        std::shared_ptr<Buffer> curBuf;
        /* 存储备用缓冲 */
        std::list<std::shared_ptr<Buffer>> bufList;
        /* 条件变量 */
        std::condition_variable readableBuf;
        /* 互斥锁 */
        std::mutex mutex;
};

bool isHave()
{
    return readableNum != 0;
}

std::shared_ptr<Buffer>
Logger::useful()
{
    auto iter = bufList.begin();
    for(; iter != bufList.end(); ++iter)
    {
        if((*iter)->useSize() == 0)
        {
            break;
        }
    }
    if(iter == bufList.end())
    {
        std::shared_ptr<Buffer> p = std::make_shared<Buffer>(4096);
        bufList.push_back(std::move(p));
        return p;
    }
    return *iter;
}

void
Logger::logStream(const char* msg, int len, char* rank)
{
    std::unique_lock<std::mutex> locker(mutex);
    if(curBuf->unuseSize() > len)
    {
        curBuf->append(msg, len);
    }else
    {
        std::cout << "1" << std::endl;
        auto useBuf = useful();
        std::cout << "2" << std::endl;
        curBuf.swap(useBuf);
        std::cout << "3" << std::endl;
        ++readableNum;
        std::cout << "4" << std::endl;
        readableBuf.notify_one();
        std::cout << "5" << std::endl;
    }
}

void 
Logger::threadFunc()
{
    while(1)
    {
        std::unique_lock<std::mutex> locker(mutex);
        auto iter = bufList.begin();
        if(readableNum == 0)
        {
            std::cout << "not date" << std::endl;
            readableBuf.wait(locker, ::isHave);
            std::cout << "have date" << std::endl;
        }
        for(; iter != bufList.end(); ++iter)
        {
            if((*iter)->useSize() != 0)
                break;
        }
        /* 如果到末尾没有找到，没有数据可读 */
        if(iter == bufList.end())
        {
            break;
        }
        else
        {
            int fd = open("1.txt", O_RDWR | O_APPEND, 00700);
            if(fd < 0)
            {
                perror("open error");
                exit(1);
            }
            write(fd, iter->get(), (*iter)->useSize());
            (*iter)->setSize();
            --readableNum;
        }
    }
}

Logger::Logger()
{
    /* 初始化Logger类和Buffer类 */
    curBuf = std::make_shared<Buffer>(4096);
    /* 开两块备用缓冲 */
    bufList.resize(2);
    *(bufList.begin()) = std::make_shared<Buffer>(4096);
    std::cout << (*(bufList.begin()))->useSize() << std::endl;
    std::cout << (*(bufList.begin()))->unuseSize() << std::endl;
    std::cout << (*(bufList.begin()))->Capacity() << std::endl;
    *(++bufList.begin()) = std::make_shared<Buffer>(4096);
    std::thread tid(std::mem_fn(&Logger::threadFunc), this);
    tid.detach();
}


#endif
