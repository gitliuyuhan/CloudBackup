/*======================================================
    > File Name: package.h
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2016年01月09日 星期六 12时46分00秒
 =======================================================*/

/*自己封装的UDP包的定义*/

#pragma once


#define UDP_PORT   "4446"                       //广播端口号
#define LOCAL_PORT "2222"                       //本地端口号
#define BUFFER_SIZE 1024                        //缓冲区大小
#define AMOUT       5000000                     //广播量

enum CONNECT_STAT{CONNECT = 0,UNCONNECT};       //广播之后客户端的俩中连接状态

enum PACKET_TYPE
{
    SEND_START = 0,                             //此状态表明此包为数据的起始包
    SEND_DATA,                                  //此状态表明此包为数据包
    SEND_END,                                   //此状态表明此包为数据的终止包
    SEND_UDP,                                   //此状态表明此包为广播包
    SEND_CORRECT,                               //此状态表明为对端连接成功的包
    SEND_UNCORRECT                              //此状态表明为对端连接失败的包
};

struct UdpDataPacket
{
    int  packetType;                            //包的类型
    int  packetNumber;                          //包的编号
    char packetData[BUFFER_SIZE];               //包的数据
    int  dataLength;                            //数据的长度
};

