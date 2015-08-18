/*
 * (C) 2007-2015 Alibaba Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * 1.cpp -- 
 *
 * Version: 1.0  2015年08月17日 21时14分36秒
 *
 * Authors:
 *     wangweihao (), wangweihao@xiyoulinux.org
 *
 */

#include "Buffer.h"
#include "Logger.h"

int main()
{
    Logger l;
    for(int i = 0; i < 1500; i++)
    {
        l.logStream("wangweihao", 10, "INFO");
    }

    sleep(2);
}
