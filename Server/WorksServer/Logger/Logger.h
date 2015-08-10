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
        static std::list<std::shared_ptr<Buffer>> bufVector;
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

int readableNum = 0;

bool
Logger::isHave()
{
    return readableNum != 0;
}

void 
Logger::Threadfunc()
{
    while(1)
    {
        func();
    }
}

void
Logger::func()
{
    std::unique_lock<std::mutex> locker(mutex);
    auto iter = bufVector.begin();
    if(readableNum == 0)
    {
        readableBuf.wait(locker, Logger::isHave);
    }
    for(; iter != bufVector.end(); ++iter)
    {
        if((*iter)->Size() == 0)
            break;
    }
    if(iter == bufVector.end())
        return;
    else
    {
        /* write file */
        /*  */
        bzero(iter->get(), (*iter)->Capacity());
        --readableNum;
    }
}

std::thread
Logger::readThread(Threadfunc);

/* return useful buffer, if not have, return a new buffer */
std::shared_ptr<Buffer> 
Logger::
useFul()
{
    auto iter = bufVector.begin();
    for(; iter != bufVector.end(); ++iter)
    {
        if((*iter)->Size() == 0)
        {
            break;
        }
    }
    if(iter == bufVector.end())
    {
        std::shared_ptr<Buffer> p = std::make_shared<Buffer>();
        bufVector.push_back(std::move(p));
        return p;
    }
    return *iter;
}

void
Logger::
logStream(const char* mesg, int len)
{
    if(curBuf->avail() > len)
    {
        curBuf->append(mesg, len);
    }
    else
    {
        /* add date not rush mutex, only buffer full rush mutex */
        std::unique_lock<std::mutex> locker(mutex);
        auto useBuf = useFul();
        curBuf.swap(useBuf);
        ++readableNum;
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
        (*bufVector.begin()) = std::make_shared<Buffer>();
        (*(++bufVector.begin())) = std::make_shared<Buffer>();
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
        (*bufVector.begin()) = std::make_shared<Buffer>(bufSize);
        (*(++bufVector.begin())) = std::make_shared<Buffer>(bufSize);
    }
    return myLogger;
}


#endif
