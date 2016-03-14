/*======================================================
    > File Name: send_file.h
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2016年01月09日 星期六 13时12分38秒
 =======================================================*/
/*定义发送文件类的头文件*/

#pragma once

#include "package.h"
#include <string>
#include <netinet/in.h>
#include <atomic>

struct sockaddr_in;

class SendFile
{
    public:
        SendFile();
        ~SendFile();

        void startSendFile(std::string &filename);                                      //开始发送文件

    private:
        void sendBroadcast(void);                                                       //持续发送广播
        std::string generateVerificationCode(void);                                     //随即生成验证码
        void waitRecipientReply(void);                                                  //线程函数
        void checkConnect(const std::string &randomCode);                               //检查连接状态
        void configureUdpFd(void);                                                      //配置udpFd_
        static void printRate(int n);                                                   //打印发送速率
        void setTimer(void);

        std::string addressBroadcast_;                          //广播ip地址
        int         udpPort_;                                   //udp端口号
        int         localPort_;                                 //本地端口
        int         udpFd_;                                     //udp套接字描述符
        struct sockaddr_in udpAddress_;                         //udp地址
        struct sockaddr_in localAddress_;                       //本地地址
        struct sockaddr_in recvAddress_;                        //接收端地址
        std::string identifyCode_;                              //验证码
        int         eventFd_;                                   //用于事件通知
};
