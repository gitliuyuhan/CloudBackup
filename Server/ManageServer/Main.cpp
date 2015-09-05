/*************************************************************************
  > File Name: Main.cpp
  > Author: lewin
  > Mail: lilinhan1303@gmail.com
  > Company:  Xiyou Linux Group
  > Created Time: 2015年08月05日 星期三 10时48分15秒
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
#include"Epoll.hpp"
#include"Mission.cpp"
#include"ThreadPool.hpp"

//#define MaxClientConnection 2
#define MAXUSERS            1000

std::string       WorkIp = "127.0.0.1";
std::string       WorkPort = "10000";
MyDataBase        DataBase;        //数据库

template<typename T>
void RecvFromClient( Epoll & e , const int & socketfd , ThreadPool<T> & pool , Mission  mission[]) {
    char buf[TCP_BUFFER_SIZE];
    while(1) {
        memset( buf , '\0' , TCP_BUFFER_SIZE );
        int ret = recv( socketfd , buf , TCP_BUFFER_SIZE - 1 , 0);
        if( 0 > ret )  {
            if((errno == EAGAIN ) || (errno == EWOULDBLOCK )) {
                break;
            }
            close(socketfd);
            break;
        }
        else if( 0 == ret ) {
            close( socketfd );
        }
        else{
            //判断用户的行为
            //UserRequest(buf,socketfd);
            strcpy(mission[socketfd].buf,buf);
//            mission[socketfd].socketfd = socketfd;
            mission[socketfd].MissionInit(socketfd);
            pool.AddTask(mission[socketfd]);
            std::cout<<"------\n";
        }
    }
}

template<typename T>
void EpollMission( Epoll & e , ThreadPool<T> & pool ,char * ip , char * port , Mission  mission[])  {
    int num;
//  int socketfds[MaxClientConnection];
    e.CreateTcpSocket();
    e.RegisterSocket();
    while(1) {
        num = e.SetEpollWait();
        for(int i = 0 ; i < num ; ++i )  {
            int socketfd = e.events[i].data.fd;
            if( socketfd == e.socketfd) {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( e.socketfd , (struct sockaddr *)&client_address , &client_addrlength);
                if( connfd >= 0 )  {
                    std::cout << "Already connect!" << std::endl; //system log
                }
                e.addfd(e.epollfd , connfd);
            }
            else if( e.events[i].events & EPOLLIN )  {
                RecvFromClient( e , socketfd , pool , mission);
            }

        }

    }
    close(e.socketfd);
}

//保存登录用户的ID
int      Mission::Users[MAXUSERS+3] = {0};

int main( int argc , char * argv[] )  {

    if(argc < 3) {
        std::cout << "参数错误!" << std::endl;
        exit(0);
    }

    pthread_t EpollThreadID;
    Mission * mission = new Mission[MAXUSERS + 3];
    ThreadPool<Mission> pool(2);

    Epoll e(argv[1] , argv[2]);
    EpollMission(e , pool ,argv[1] , argv[2] , mission);

    return EXIT_SUCCESS;
}
