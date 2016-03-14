/*======================================================
    > File Name: logFile.h
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2015年12月26日 星期六 14时57分54秒
 =======================================================*/
#ifndef LOG_FILE_H_
#define LOG_FILE_H_
namespace netlib
{
    class LogFile
    {
        public:
            LogFile(int rollSize);
            ~LogFile();
            void append(char *log,int len);

        private:
         int rollSize_;
         int fd_;
            
    };
}

#endif

