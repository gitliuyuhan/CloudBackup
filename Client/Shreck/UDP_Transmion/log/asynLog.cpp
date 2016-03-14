/*======================================================
    > File Name: asynLog.cpp
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2015年12月24日 星期四 18时48分21秒
 =======================================================*/
#include "asynLog.h"
#include "logFile.h"
#include "timestamp.h"
#include <stdio.h>
#include <string.h>
#include <memory>
#include "fixBuffer.h"
#include <mutex>
#include <condition_variable>
#include <functional>
#include <pthread.h>

using namespace netlib;

AsynLog::AsynLog(int rollSize)
    :rollSize_(rollSize),
    running_(true),
    currentBuffer_(new FixBuffer()),
    nextBuffer_(new FixBuffer()),
    acceptThread_(std::bind(&AsynLog::threadFunc,this))
{
    printf("日志系统已开启\n");   
}

AsynLog::~AsynLog()
{
    stop();
}

void AsynLog::setLevel(LogLevel level)
{
    currentLevel_ = level;   
}

bool AsynLog::aboveLevel(LogLevel level)
{
    return level <= currentLevel_;
}


void AsynLog::append(char *logline,int len,LogLevel level)
{
    if(aboveLevel(level))   //如果level超过所设等级
    {
        std::lock_guard<std::mutex> guard(mutex_);
        //与时间以及线程id等字符串连接
        std::string log1(logline);  //日志内容
        std::string log2(timestamp_.toStringTime());//获得时间戳的字符串
        char log3[30];
        snprintf(log3,sizeof(log3),"threadid[%lu]: ",pthread_self());//获得线程id的字符串
        std::string log4(toStringForLevel(level));  //获得等级对应的字符串
        log2 = log2 + log3 + log4 + log1;
        writeBuffer(log2.c_str(),log2.size());  //写入buffer
    }
}


void AsynLog::writeBuffer(const char *logline,int len)
{
    if(currentBuffer_->writeableSize() >= len)  //如果当前buffer空间足
    {
        currentBuffer_->append(logline,len);
    }
    else
    {
        buffers_.push_back(std::move(currentBuffer_)); //返回指针，自己变为空
        
        if(nextBuffer_)
        {
            currentBuffer_ = std::move(nextBuffer_);    //将nextBuffer_的控制权交给currentBuffer_
        }
        else
        {
            currentBuffer_.reset(new FixBuffer());  //申请一块新的buffer
        }

        currentBuffer_->append(logline,len);
        condition_.notify_one();    //唤醒后台线程
    }
}

std::string AsynLog::toStringForLevel(LogLevel level)
{
    switch(level)
    {
        case LogLevel::OFF:
            return std::string("OFF ");break;
        case LogLevel::FATAL:
            return std::string("FATAL ");break;
        case LogLevel::ERROE:
            return std::string("ERROE ");break;
        case LogLevel::WARN:
            return std::string("WARN ");break;
        case LogLevel::INFO:
            return std::string("INFO ");break;
        case LogLevel::DEBUG:
            return std::string("DEBUG ");break;
        case LogLevel::ALL:
            return std::string("ALL ");break;
        default:return std::string("UNKNOWN ");
    }
    return NULL;
}

void AsynLog::stop(void)
{
    if(running_)
    {
        running_ = false;
        acceptThread_.join();   //等待后台进程退出
        printf("日志系统已关闭\n");
    }
}

void AsynLog::threadFunc(void)
{
    std::unique_ptr<FixBuffer> newBuffer1(new FixBuffer);
    std::unique_ptr<FixBuffer> newBuffer2(new FixBuffer); 
    std::vector<std::unique_ptr<FixBuffer>> buffersToWrite;
    LogFile output(1000);
    buffersToWrite.reserve(16);
    while(running_)
    {
        {
            std::lock_guard<std::mutex> guard(mutex_);  //临界区加锁
            if(buffers_.empty())    //如果buffers_为空
            {
                condition_.wait_for(mutex_,std::chrono::seconds(3));
            }
            buffers_.push_back(std::move(currentBuffer_));
            currentBuffer_ = std::move(newBuffer1);
            buffersToWrite.swap(buffers_);  //交换俩个容器
            if(!nextBuffer_)
            {
                nextBuffer_ = std::move(newBuffer2);
            }
            
        }
        assert(!buffersToWrite.empty());

        if(buffersToWrite.size() > 25)
        {
            //日志异常
        }

        //将buffer中的内容写进文件中
        for(int i = 0; i < buffersToWrite.size(); i++)
        {
            output.append(buffersToWrite[i]->getReadPeek(),buffersToWrite[i]->readableSize());        
        }

        if(buffersToWrite.size() > 2)
        {
            buffersToWrite.resize(2);
        }

        if(!newBuffer1)
        {
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite[0]);
            newBuffer1->resetBuffer();  //重置buffer
        }

        if(!newBuffer2)
        {
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite[1]);
            newBuffer2->resetBuffer();
        }

        buffersToWrite.clear();
    }   
}
