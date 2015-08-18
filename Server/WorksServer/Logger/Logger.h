/*
 * =====================================================================================
 *
 *       Filename:  Logger.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月09日 16时24分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (wangweihao), 578867817@qq.com
 *        Company:  xiyoulinuxgroup
 *
 * =====================================================================================
 */


#ifndef _LOGGER_H_
#define _LOGGER_H_

/* 
 * 单例模式实现
 * 一个能处理并发日志消息的日志库 
 * */


/* 定义日志级别 */
#define INFO 1        //输出记录消息
#define ERROR 2       //输出错误消息
#define FAIL 3        //输出操作失败消息

#include <iostream>
#include <memory>
#include <list>
#include "Buffer.h"  //realize a easy Buffer class
#include <mutex>
#include <condition_variable>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



class Logger
{
    public:
        /* 创建日志类实例 */
        static std::shared_ptr<Logger> setLogger();
        static std::shared_ptr<Logger> setLogger(size_t bufSize);

        /* 得到日志类实例 */
        static std::shared_ptr<Logger> getLogger();

        /* 按格式输出日志信息到指定文件 */
        static void logStream(const char* mesg, int len);


    private:
        /* shared_ptr智能指针管理log类 */
        static std::shared_ptr<Logger> myLogger;

        /*  */
        static std::shared_ptr<Buffer> curBuf;
        /*  */
        static std::list<std::shared_ptr<Buffer>> bufList;
        /*  */
        static std::shared_ptr<Buffer> useFul();

        static std::condition_variable readableBuf;
        static int readableNum;
        static std::mutex mutex;

        static std::thread readThread;
        
        static void Threadfunc();
        static void func();
        static bool isHave();

    private:
        /* 不能被拷贝或赋值 */
        //Logger(const Logger&) = delete;
        //Logger& operator=(const Logger&) = delete;
};


bool
Logger::isHave()
{
    return readableNum != 0;
}

void 
Logger::Threadfunc()
{
    std::cout << "线程开始执行啦" << std::endl;
    readThread.detach();
    while(1)
    {
        func();
    }
}

void
Logger::func()
{
    std::cout << __LINE__ << std::endl;
    std::unique_lock<std::mutex> locker(mutex);
    auto iter = bufList.begin();
    if(readableNum == 0)
    {
        std::cout << "数据为空，线程被阻塞" << std::endl;
        readableBuf.wait(locker, Logger::isHave);
        std::cout << "线程被唤醒" << std::endl;
    }
    std::cout << "heihei" << std::endl;
    std::cout << "capacity" << bufList.size() << std::endl;
    /* 找数据不为空的Buffer */
    for(; iter != bufList.end(); ++iter)
    {
        if((*iter)->Size() != 0)
            break;
    }
    /* 如果到末尾没找到，没有数据可读 */
    if(iter == bufList.end())
    {
        std::cout << "没有数据读" << std::endl;
        return;
    }
    else
    {
        /* write file */
        int fd = open("1.txt", O_RDWR | O_APPEND, 00700);
        if(fd < 0)
        {
            perror("open error\n");
            exit(1);
        }
        write(fd, iter->get(), (*iter)->Capacity());
        bzero(iter->get(), (*iter)->Capacity());
        (*iter)->setSize();
        --readableNum;
    }
}

/* return useful buffer, if not have, return a new buffer */
std::shared_ptr<Buffer> 
Logger::
useFul()
{
    auto iter = bufList.begin();
    for(; iter != bufList.end(); ++iter)
    {
        if((*iter)->Size() == 0)
        {
            break;
        }
    }
    if(iter == bufList.end())
    {
        std::shared_ptr<Buffer> p = std::make_shared<Buffer>();
        bufList.push_back(std::move(p));
        return p;
    }
    return *iter;
}

void
Logger::
logStream(const char* mesg, int len)
{
    std::cout << "logStream" << std::endl;
    if(curBuf->avail() > len)
    {
        curBuf->append(mesg, len);
    }
    else
    {
        /* add date not rush mutex, only buffer full rush mutex */
        std::cout << "mutex" << std::endl;
        std::unique_lock<std::mutex> locker(mutex);
        std::cout << "useful" << std::endl;
        auto useBuf = useFul();
        std::cout << "swap" << std::endl;
        curBuf.swap(useBuf);
        std::cout << "readablenum" << std::endl;
        ++readableNum;
        std::cout << readableNum << std::endl;
        std::cout << "notify_one" << std::endl;
        readableBuf.notify_one();
    }
}

std::shared_ptr<Logger>
Logger::
getLogger()
{
    return myLogger;
}

std::shared_ptr<Logger>
Logger::
setLogger()
{
    if(myLogger == nullptr)
    {
        myLogger = std::move(std::make_shared<Logger>());
        curBuf = std::make_shared<Buffer>();
        bufList.resize(2);
        (*bufList.begin()) = std::make_shared<Buffer>();
        (*(++bufList.begin())) = std::make_shared<Buffer>();
    }
    return myLogger;
}

std::shared_ptr<Logger>
Logger::
setLogger(size_t bufSize)
{
    if(myLogger == nullptr)
    {
        myLogger = std::move(std::make_shared<Logger>());
        curBuf = std::make_shared<Buffer>(bufSize);
        bufList.resize(2);
        (*bufList.begin()) = std::make_shared<Buffer>(bufSize);
        (*(++bufList.begin())) = std::make_shared<Buffer>(bufSize);
    }
    return myLogger;
}


#endif
