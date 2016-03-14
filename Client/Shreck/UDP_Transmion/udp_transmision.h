/*************************************************************************
    > File Name: udp_transmision.h
    > Author: MiaoShuai
    > Mail: 945970809@qq.com 
    > Created Time: 2015年10月12日 星期一 20时41分48秒
 ************************************************************************/
#ifndef _UDP_TRANSMISION_
#define _UDP_TRANSMISION_

#include<iostream>
#include<set>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<ifaddrs.h>
#include<stdlib.h>
#include<random>
#include<pthread.h>
#include<fcntl.h>
#include<sys/types.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::set;

#define UDP_PORT "3333"		//广播端口号
#define LOCAL_PORT "2222"	//本地端口号
#define BUFFER_SIZE 1400	//缓冲区大小
#define AMOUT 5000000		//广播量

enum CONNECT_STAT{CONNECT=0,UNCONNECT};		//在广播之后客户端的俩种连接状态


enum PACKET_TYPE{
	SEND_START=0,							//此状态表明此包为开始发送的起始包
	SEND_DATA,								//此状态表明此包为正在传送数据的包
	SEND_END,								//此状态表明此包为终止发送的终止包
	SEND_UDP,								//此状态表明此包为广播包
	SEND_CORRECT,							//此状态为连接成功包
	SEND_UNCORRECT							//此状态为连接失败包
};

//获取本地IP地址
string GetLocalIpAddress(void);
//void WaitRecipientReply(void);			//等待接收方回应

class UdpDataPacket
{
	public:
		int packetType;						//包的类型
		int packetNumber;					//包的编号
		char packetData[BUFFER_SIZE];		//包的数据
};

bool ComparePacketNumber(const UdpDataPacket &pack1,const UdpDataPacket &pack2);


class SendFile
{
	public:
		SendFile();
		void StartSendFile(char *filename);			//开始传送文件
		static void *WaitRecipientReply(void *arg);	//等待接收方回应
	private:
		string GetBroadcastAddress(void);			//获取广播地址
		string extractFileName(string filename);	//将文件名从文件的绝对路径中提取出来
		void SendBroadcast(void);					//持续发送广播
		string GenerateVerificationCode();			//随机生成验证码
		
		string addressBroadcast;					//广播ip地址
		int udpPort;								//udp端口
		int localPort;								//本地端口
		int udpfd;									//udp套接字描述符
		sockaddr_in udpAddress;						//udp地址
		sockaddr_in localAddress;					//本地地址
		sockaddr_in recvAddress;					//接收端地址
		string identifyCode;						//验证码
		
};


class RecvFile
{
	private:
		int recvFd;							//接收端套接字描述符
		int udpPort;						//与服务器所发送地址的端口号一致
		
		std::set<UdpDataPacket,decltype(ComparePacketNumber) *> recvDataSet;
		sockaddr_in recvAddress;			//接收方地址
		sockaddr_in sendAddress;			//发送方地址

	public:
		RecvFile();
		void StartRecvFile(void);			//开始接收文件

};
#endif


