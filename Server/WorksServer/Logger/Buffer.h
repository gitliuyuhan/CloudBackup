/*
 * =====================================================================================
 *
 *       Filename:  Buffer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月09日 20时47分28秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (wangweihao), 578867817@qq.com
 *        Company:  xiyoulinuxgroup
 *
 * =====================================================================================
 */


#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <vector>
#include <cstring>
#include <iostream>

/* 
 * Buffer缓冲区类
 * */

class Buffer
{
    /* 初始化默认大小 */
    static size_t initializeSize;
    
    public:
        /* 构造函数，初始化buffer,并且设置可读可写的位置 */
        explicit Buffer(size_t BufferSize = initializeSize):
            readable(0), writable(0)
        {
            buffer.resize(BufferSize);
        }

        /* 返回缓冲区的容量 */
        size_t Capacity()
        {
            std::cout << "capacity" << std::endl;
            return buffer.capacity();
        }

        /* 返回缓冲区的大小 */
        size_t Size()
        {
            std::cout << "writable" << writable << std::endl;
            return writable;
        }

        /* set Size */
        void setSize(void)
        {
            readable = 0;
            writable = 0;
        }

        /* 向buffer中添加数据 */
        void append(const char* mesg, int len)
        {
            std::cout << "append" << std::endl;
            strncpy(WritePoint(), mesg, len);
            writable += len;
        }

        /* 返回buffer可用大小 */
        size_t avail()
        {
            std::cout << "avail" << std::endl;
            return Capacity()-writable;
        }

    private:
        /* 返回可读位置的指针 */
        char* ReadPoint()
        {
            return &buffer[readable];
        }
        
        /* 返回可写位置的指针 */
        char* WritePoint()
        {
            return &buffer[writable];
        }
        
        /* 返回可读位置 */
        size_t ReadAddr()
        {
            return readable;
        }

        /* 返回可写位置 */
        size_t WriteAddr()
        {
            return writable;
        }

    private:
        std::vector<char> buffer;
        size_t readable;
        size_t writable;
};



#endif
