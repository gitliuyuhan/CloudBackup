/*======================================================
    > File Name: logFile.cpp
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2015年12月26日 星期六 15时12分35秒
 =======================================================*/
#include "logFile.h"
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

using namespace netlib;

LogFile::LogFile(int rollSize)
    :rollSize_(rollSize)
{
    fd_ = open("/home/shreck/log/mylog.log",O_WRONLY);
    assert(fd_ > 0);
}

LogFile::~LogFile()
{
    close(fd_);
}

void LogFile::append(char *log,int len)
{
    int ret = write(fd_,log,len);
    printf("写入了%d\n",ret);
    assert(ret == len);
}


