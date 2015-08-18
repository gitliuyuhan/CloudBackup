/*
 * (C) 2007-2015 Alibaba Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Buffer.h -- 
 *
 * Version: 1.0  2015年08月17日 21时00分16秒
 *
 * Authors:
 *     wangweihao (), wangweihao@xiyoulinux.org
 *
 */

/* 
 * Buffer缓冲区类
 * */

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <vector>
#include <cstring>
#include <iostream>


class Buffer
{
    public:
        explicit Buffer(size_t BufferSize):
            readable(0), writable(0)
    {
        buffer.resize(BufferSize);
    }

    /* 返回缓冲区容量 */
    size_t Capacity()
    {
        return buffer.capacity();
    }

    /* 返回缓冲区使用大小 */
    size_t useSize()
    {
        return writable;
    }

    /* 返回缓冲区未使用大小 */
    size_t unuseSize()
    {
        return Capacity() - writable;
    }

    /* 还原readable和writable的位置 */
    void setSize(void)
    {
        readable = 0;
        writable = 0;
    }

    void append(const char* msg, int len)
    {
        strncpy(writePoint(), msg, len);
        writable += len;
    }

    private:
    /* 返回可读位置的指针 */
    char* readPoint()
    {
        return &buffer[readable];
    }

    /* 返回可写位置的指针 */
    char* writePoint()
    {
        return &buffer[writable];
    }

    /* 返回可读位置 */
    size_t readAddr()
    {
        return readable;
    }

    /* 返回可写位置 */
    size_t writeAddr()
    {
        return writable;
    }

    private:
        /* buffer用vector<char>更方便管理 */
        std::vector<char> buffer;
        /* 读位置和写位置 */
        size_t readable;
        size_t writable;
};

#endif
