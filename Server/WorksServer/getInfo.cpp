/*
 * (C) 2007-2015 Alibaba Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * getInfo.cpp -- 
 *
 * Version: 1.0  2015年08月17日 16时53分25秒
 *
 * Authors:
 *     wangweihao (), wangweihao@xiyoulinux.org
 *
 */

#include "getInfo.h"

std::string
getDate::getInfo(int types)
{
    std::string retInfo;
    switch(types)
    {
        case cpuInfo:
            retInfo = getCpuInfo();
            break;

        case memInfo:
            retInfo = getMemInfo();
            break;

        case netInfo:
            retInfo = getNetInfo();
            break;
        
        case diskInfo:
            retInfo = getDiskInfo();
            break;
    }
    return retInfo;
}

std::string
getDate::getCpuInfo()
{
    FILE *fp;
    char buffer[512];
    /* 通过top命令获得cpu使用率, -n表示只执行一次 */
    std::string command("top -n 1 | grep Cpu | awk \'{print}\'");
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        perror("popen error");
        exit(1);
    }
    fgets(buffer, 512, fp);
   // printf("cpu info:%s", buffer);
    pclose(fp);
    return buffer;
}

std::string 
getDate::getMemInfo()
{
    FILE *fp;
    char buffer[512];
    /* 通过top命令获得memory使用率, -n表示只执行一次 */
    std::string command("top -n 1 | grep Mem | awk \'{print}\'");
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        perror("popen error");
        exit(1);
    }
    fgets(buffer, 512, fp);
    //printf("memory info:%s", buffer);
    pclose(fp);
    return buffer;
}

std::string 
getDate::getNetInfo()
{
    FILE *fp;
    char buffer[512];
    /* net  sar */
    std::string command("sar -n DEV 1 1 | grep wlan0 | grep 平均时间 | awk \'{print}\'");
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        perror("popen error");
        exit(1);
    }
    fgets(buffer, 512, fp);
    //printf("net info:%s", buffer);
    pclose(fp);
    return buffer;
}

std::string 
getDate::getDiskInfo()
{
    FILE *fp;
    char buffer[512];
    /* iostat */
    std::string command("iostat | grep sda | awk \'{print}\'");
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        perror("popen error");
        exit(1);
    }
    fgets(buffer, 512, fp);
    //printf("disk info:%s", buffer);
    pclose(fp);
    return buffer;
}
