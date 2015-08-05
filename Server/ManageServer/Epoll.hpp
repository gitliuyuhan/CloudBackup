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

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define LISTEN_NUMBER 1024


class Epoll{
    public:
        Epoll(char * ip , char * port):epollfd(-1) , ServerIP(ip) , Port(port)  {}
        ~Epoll() {}

        int SetNonBlocking ( int fd )  {
            int old_option = fcntl( fd , F_GETFL );
            int new_option = old_option | O_NONBLOCK;
            fcntl( fd , F_SETFL , new_option);
            return old_option;
        }

        void addfd( int epollfd , int fd )  {
            epoll_event event;
            event.data.fd = fd ;
            event.events = EPOLLIN | EPOLLOUT | EPOLLERR;
            epoll_ctl( epollfd , EPOLL_CTL_ADD , fd , &event );
            SetNonBlocking( fd );
        }

        void CreateTcpSocket(  )  {
            int port = atoi(Port);
            struct sockaddr_in address;
            bzero( &address , sizeof(address));
            address.sin_family = AF_INET;
            inet_pton( AF_INET , ServerIP , &address.sin_addr);
            address.sin_port = htons( port );

            if((socketfd = socket( PF_INET , SOCK_STREAM , 0)) < 0 )  {
                std::cout << "Create Tcp Socket Error! "  << std::endl;
                return;
            }
            if((bind(socketfd , (struct sockaddr*)&address , sizeof( address ))) == -1)  {
                std::cout << "Tcp Socket Bind Error!" << std::endl;
                return;
            }
            if((listen( socketfd , LISTEN_NUMBER)) == -1)  {
                std::cout << "Socket Listen Error!" << std::endl;
                return;
            }
        }

        void RegisterSocket()  {
            if((epollfd = epoll_create(LISTEN_NUMBER)) == -1 )  {
                std::cout << "Create Epollfd Error!" << std::endl;
                return;
            }
            addfd( epollfd , socketfd );
        }

        int SetEpollWait()  {
            int num;
            if((num = epoll_wait( epollfd , events , MAX_EVENT_NUMBER , -1)) < 0 )  {
                std::cout << "Create Epollfd Error!" << std::endl;
            }
            return num;
        }

    public:
        int epollfd;
        int socketfd; //监听用
        char * ServerIP;
        char * Port;
        epoll_event events[MAX_EVENT_NUMBER];

};


