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
            return buffer.capacity();
        }

        /* 返回缓冲区的大小 */
        size_t Size()
        {
            return buffer.size();
        }

        /* 向buffer中添加数据 */
        void append(const char* mesg, int len)
        {
            strncpy(WritePoint(), mesg, len);
        }

        /* 返回buffer可用大小 */
        size_t avail()
        {
            return buffer.capacity()-writable;
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

size_t initializeSize = 4096;


#endif
