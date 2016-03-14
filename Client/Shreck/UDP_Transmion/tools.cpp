/*======================================================
    > File Name: tools.cpp
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2016年01月20日 星期三 14时41分19秒
 =======================================================*/
#include "tools.h"
#include "package.h"

#include <string>
#include <sys/types.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


std::string getBroadcastAddress(void)
{
   ifaddrs *ifAddrStruct = NULL;
    
    char addressBuffer[100];
    std::string interfaceName1 = "wlan1";
    std::string interfaceName2 = "eth0";
    void *tmpAddressPtr = NULL;

    
    //获取ifconfig相关信息的结构体
    getifaddrs(&ifAddrStruct);

    //从链表中获取广播地址
    while(ifAddrStruct != NULL)
    {
        
        //std::cout<<ifAddrStruct->ifa_name<<std::endl;
        if((strcmp(ifAddrStruct->ifa_name,interfaceName1.c_str()) == 0) || (strcmp(ifAddrStruct->ifa_name,interfaceName2.c_str()) == 0)) 
        {
            
            tmpAddressPtr = &((struct sockaddr_in *)(ifAddrStruct->ifa_ifu.ifu_broadaddr))->sin_addr;
            inet_ntop(AF_INET,tmpAddressPtr,addressBuffer,INET_ADDRSTRLEN);
            
            if(addressBuffer[0] == '1' && addressBuffer[1] == '9' && addressBuffer[2] == '2')
            {
                return addressBuffer;
            }
        }
        ifAddrStruct = ifAddrStruct->ifa_next;
    }
        
    return NULL; 
}


std::string getLocalIpAddress(void)
{
    ifaddrs *ifAddrStruct = NULL;
    char addressBuffer[100];
    std::string interfaceName1 = "eth0";
    std::string interfaceName2 = "wlan0";
    void *tmpAddressPtr = NULL;

    //获取ifconfig相关信息的结构体
    getifaddrs(&ifAddrStruct);

    //从链表中获取广播地址
    while(ifAddrStruct != NULL)
    {   
        
        if((strcmp(ifAddrStruct->ifa_name,interfaceName1.c_str()) == 0) || (strcmp(ifAddrStruct->ifa_name,interfaceName2.c_str()) == 0)) 
        {
            
            tmpAddressPtr = &((struct sockaddr_in *)(ifAddrStruct->ifa_addr))->sin_addr;
            inet_ntop(AF_INET,tmpAddressPtr,addressBuffer,INET_ADDRSTRLEN);
            if(addressBuffer[0] == '1' && addressBuffer[1] == '9' && addressBuffer[2] == '2')
            {
                return addressBuffer;
            }
        }
        ifAddrStruct = ifAddrStruct->ifa_next;
        
    }
    
    return "\0";    
}


std::string extractFilename(std::string filename)
{
    int pos = filename.find_last_of('/');

    return std::string(filename.substr(pos + 1));
}


UdpDataPacket *makeUdpDataPacket(int number,int type,const char *data)
{
    UdpDataPacket *p = new UdpDataPacket;
    p->packetNumber = number;
    p->packetType = type;
    strcpy(p->packetData,data);
    p->dataLength = strlen(data);
    return p;
}


void makeSockAddress(sockaddr_in &sockaddr,const char *address,int port)
{
    bzero(&sockaddr,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    inet_pton(AF_INET,address,&sockaddr.sin_addr);
    sockaddr.sin_port = htons(port);
}
