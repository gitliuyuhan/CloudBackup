/*======================================================
    > File Name: fixBuffer.h
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2015年12月24日 星期四 17时02分09秒
 =======================================================*/
#ifndef FIX_BUFFER_H_
#define FIX_BUFFER_H_

#include <vector>
#include <string>
#include <assert.h>
#include <fcntl.h>


namespace netlib
{
    class FixBuffer
    {
        public:
            FixBuffer()
                :buffer_(1024*1024*4)   //初始化大小为4M
            {
                readableIndex_ = 0;
                writeableIndex_ = 0;
            }
            ~FixBuffer()
            {
            
            }
            int readableSize(void)  //可读字节数
            {
                return writeableIndex_ - readableIndex_;    
            }

            int writeableSize(void) //可写字节数
            {
                return buffer_.size() - writeableIndex_;
            }

            void append(const char *data,int len) //添加数据到buffer中
            {
                std::copy(data,data + len,getWritePeek());
                moveWriteIndex(len);
            }
            
            char *getReadPeek(void) //获得读位置的指针
            {
                return begin() + readableIndex_;
            }

            char *getWritePeek(void)    //获得写位置的指针
            {
                return begin() + writeableIndex_;
            }


            void moveWriteIndex(int len)    //移动可写下标
            {
                writeableIndex_ = writeableIndex_ + len;
            }

            void resetBuffer(void)  //重置buffer
            {
                readableIndex_ = 0;
                writeableIndex_ = 0;
            }

        private:
            char *begin()
            {
                return &*buffer_.begin();
            }


            std::vector<char> buffer_;
            int readableIndex_;
            int writeableIndex_;
                        
   };
}

#endif
