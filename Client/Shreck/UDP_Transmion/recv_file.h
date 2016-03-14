/*======================================================
    > File Name: recv_file.h
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2016年01月09日 星期六 13时26分51秒
 =======================================================*/
/*接收文件类的定义*/

#pragma once

#include "package.h"
#include <string>
#include <set>
#include <netinet/in.h>

//用来给set集合传递比较函数
bool comparePacketNumber(const UdpDataPacket &pack1,const UdpDataPacket &pack2);

class RecvFile
{
    public:
        RecvFile();
        ~RecvFile();
        
        void startRecvFile(void);               //开始接受文件

    private:
        void getConnection(void);
        int getFileFd(void);
        void setRecvFd(int on);
        void checkSetData(int &n,int fd,UdpDataPacket &data);
        typedef std::set<UdpDataPacket,decltype(comparePacketNumber) *> DataPacketSet;

        DataPacketSet       recvDataSet_;       //接收数据的一个buffer用来将数据弄为有序
        struct sockaddr_in         recvAddress_;       //接收方地址
        struct sockaddr_in         sendAddress_;       //发送发地址
        int                 recvFd_;             //接受端套接字描述符

        int                 udpPort_;           //与服务器所发地址的端口一致
};


