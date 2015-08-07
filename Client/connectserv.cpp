/*======================================================
    > File Name: connectServ.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月05日 星期三 16时13分52秒
 =======================================================*/

#include<QString>

#include<iostream>

#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

using namespace std;

int ConnectServer(QString ip,QString port)
{
    string  strip,strport;
    strip = ip.toStdString();
    strport = port.toStdString();

    struct sockaddr_in    servAddress;
    bzero(&servAddress,sizeof(servAddress));
    servAddress.sin_family = AF_INET;
    inet_pton(AF_INET,strip.c_str(),&servAddress.sin_addr);
    servAddress.sin_port = htons(atoi(strport.c_str()));
    int  sockfd = socket(PF_INET,SOCK_STREAM,0);
    assert(sockfd >= 0);

    if(connect(sockfd,(struct sockaddr*)&servAddress,sizeof(servAddress)) != -1)
    {
        return sockfd;
    }
    close(sockfd);
    return -1;

}

void closefd(int sock)
{
    close(sock);
}



