/*************************************************************************
    > File Name: epoll.hpp
    > Author: lewin
    > Mail: lilinhan1303@gmail.com
    > Company:  Xiyou Linux Group
    > Created Time: 2015年07月27日 星期一 08时40分51秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<cstdlib>
#include<cerrno>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<thread>
#include<list>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<atomic>

void receiveMessage(int ClientSocket) {
    char receiveBuf[1024];
    while(1) {
    recv(ClientSocket , (void *)&receiveBuf , sizeof(receiveBuf) , 0);
    std::cout << receiveBuf << std::endl;
    bzero(&receiveBuf,sizeof(receiveBuf));
    }
}

void sendMessage(int ClientSocket)  {
    char sendBuf[1024];
    while(1) {
        std::cin >> sendBuf;
        send(ClientSocket , sendBuf , strlen(sendBuf) , 0);
    }
}

int main(int argc , char * argv[]) {
    int ClientSocket = socket(AF_INET , SOCK_STREAM , 0);
    struct sockaddr_in servaddr;

    bzero(&servaddr , sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6000);
    inet_pton(AF_INET , "192.168.20.196" , &servaddr.sin_addr);


    if(connect(ClientSocket , (struct sockaddr *)&servaddr , sizeof(servaddr)) != 0 ) {
        std::cout << "Client Connect Error" << std::endl;
        return -1;
    }

    std::thread sendTd(sendMessage , ClientSocket);
    std::thread recvTd(receiveMessage , ClientSocket);

    sendTd.join();
    recvTd.join();
    return 0;
}

