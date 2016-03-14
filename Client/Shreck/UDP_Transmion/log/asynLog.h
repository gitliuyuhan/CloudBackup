/*======================================================
    > File Name: asynLog.h
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2015年12月24日 星期四 17时33分26秒
 =======================================================*/
#ifndef ASYNLOG_H_
#define ASYNLOG_H_

#include <memory>
#include "fixBuffer.h"
#include "timestamp.h"
#include <vector>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>

namespace netlib
{
    enum LogLevel
    {
        OFF,    //关闭所有日志记录
        FATAL,  //导致程序退出的错误
        ERROE,  //发生了错误但不影响系统运行
        WARN,   //会出现潜在的错误情形
        INFO,   //系统发生了那些事情
        DEBUG,  //调试程序有关的信息
        ALL,    //输出所有日志记录
    };
    class AsynLog
    {
        public:
            AsynLog(int roolSize);
            ~AsynLog();
            void append(char *logline,int len,LogLevel level); //添加日志行
            void setLevel(LogLevel level); //设置日志的水平
            void stop(void);   //关闭日志
        private:
            void threadFunc(void);  //线程函数
            bool aboveLevel(LogLevel level); //判断某条日志等级是否超过我们所设的level 
            void writeBuffer(const char* logline,int len);  //将日志内容写入buffer中
            std::string toStringForLevel(LogLevel level);   //将对应的level转化为字符串           

            int rollSize_;  //文件达到多大时滚动 
            std::unique_ptr<FixBuffer> currentBuffer_;   //当前buffer
            std::unique_ptr<FixBuffer> nextBuffer_;      //备用buffer
            std::vector<std::unique_ptr<FixBuffer>> buffers_;    //保存buffer的vector
            std::thread acceptThread_;   //后台接受数据的线程
            std::mutex mutex_;      //互斥变量
            std::condition_variable_any condition_; //条件变量
            Timestamp timestamp_;   //时间戳
            LogLevel currentLevel_; //当前日志等级
            bool running_;                //日志时候否正在运行
    };
}

#endif
