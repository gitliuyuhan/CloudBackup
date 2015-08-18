/*
 * =====================================================================================
 *
 *       Filename:  1.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月10日 08时54分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (wangweihao), 578867817@qq.com
 *        Company:  xiyoulinuxgroup
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "Logger.h"

int Logger::readableNum = 0;
std::mutex Logger::mutex;
std::condition_variable Logger::readableBuf;
std::thread Logger::readThread(Logger::Threadfunc);
std::shared_ptr<Logger> Logger::myLogger;
std::shared_ptr<Buffer> Logger::curBuf;
std::list<std::shared_ptr<Buffer>> Logger::bufList;
size_t Buffer::initializeSize = 4096;

int main(int argc, char *argv[])
{
    Logger log;
    log.setLogger();
    for(int i = 0; i < 600; i++)
    {
        log.logStream("wangweihao", 10);
        std::cout << i << std::endl;
    }
    std::cout << "result..." << std::endl;
    sleep(5);

	return EXIT_SUCCESS;
}


