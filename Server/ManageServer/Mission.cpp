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
#include"jsoncpp-src-0.5.0/include/json/json.h"
#include"MyDataBase.cpp"

#define MaxClientConnection 1000


class Mission{
    public:
        Mission(char * ip , char * port):e(ip , port){}
        ~Mission() {}

        //验证账户密码任务
        int AccountPasswd(Json::Value & root , int & socketfd) {
            std::string UserName , Passwd;
            UserName = root["UserName"].asString();
            Passwd = root["Passwd"].asString();
            if( 1 == db[socketfd].AccountPasswd(UserName , Passwd))  {
                return 1;
            }
            return -1;
        }

        //注册用户
        int Regiester(Json::Value & root , int & socketfd)  {
            std::string UserName , Passwd , Email;
            UserName = root["UserName"].asString();
            Passwd = root["Passwd"].asString();
            Email = root["Email"].asString();

            if(1 == db[socketfd].Register(UserName , Passwd , Email))  {
                return 1;
            }
            return -1;
        }

        //进入目录
        std::string DirFiles(Json::Value root , int & socketfd)  {
            std::string AimDir , AllFiles;
            AimDir = root["UserFilePath"].asString();
            AllFiles = db[socketfd].DirFiles(AimDir);

            return AllFiles;
        }
        //创建新目录
        int CreateNewDir(Json::Value root , int &  socketfd) {
            std::string DirName;
            DirName = root["UserFilePath"].asString();
            if( 1 ==db[socketfd].CreateNewDir(DirName)) {
                return 1;
            }
            return -1;
        }
        //重命名文件或目录
        int RenameFileName(Json::Value root , int & socketfd) {
            std::sting OldName , NewName;
            OldName = root["OldName"].asString();
            NewName = root["NewName"].asString();
            if( 1 == db[socketfd].RenameFileName(OldName , NewName)) {
               return 1;
            }
            return -1;
        }
        //删除文件或目录
        int DeleteFileORDir(Json::Value root , int & socketfd)  {
            std::string File;
            File = root["UserFilePath"].asString();
            if( 1 == db[socketfd].DeleteFileORDir(File))  {
                return 1;
            }
            return -1;
        }

        //上传文件
        int UploadFile(Json::Value root , int & socketfd)  {
            std::string UserFilePath , UserFileSize , ServerFilePath , MD5;
            int flag;
            UserFilePath = root["UserFilePath"].asString();
            UserFileSize = root["UserFileSize"].asString();
            ServerFilePath = root["ServerFilePath"].asString();
            MD5 = root["MD5"].asString();
            flag = root["flag"].asInt();

            if(1 == db[socketfd].UploadFile(UserFilePath , UserFileSize , ServerFilePath , MD5 , flag))  {
                return 1;
            }
            return -1;
        }

        //下载文件
        int DownloadFile(Json::Value root , int & socketfd)  {
            Json::Value croot;
            std::string UserFilePath , ServerFilePath;
            UserFilePath = root["UserFilePath"].asString();
            ServerFilePath = db[socketfd].DownloadFile(UserFilePath);
            croot["PATH"] = ServerFilePath;
            //给客户端发过去
            char * buf = croot.toStyledString();
            send(socketfd , (void *)buf , sizeof(buf) , 0);
        }
        //监控文件
        int MonitorFile(Json::Value root , int & socketfd)  {
            std::string UserFilePath , UserFileSize , ServerFilePath , MD5;
            int flag;
            UserFilePath = root["UserFileSize"].asString();
            UserFileSize = root["UserFileSize"].asString();
            ServerFilePath = root["ServerFilePath"].asString();
            MD5 = root["MD5"].asString();
            flag = root["flag"].asInt();

            if( true == db[socketfd].MonitorFile(UserFileSize , UserFileSize , ServerFilePath , MD5 , flag))  {
                return 1;
            }
            return -1;
        }

        //恢复文件
        int RestoreFile(Json::Value root , int & socketfd)  {
            Json::Value croot;
            std::string UserFilePath , ServerFilePath;
            UserFilePath = root["UserFilePath"].asString();
            ServerFilePath = db[socketfd].DownloadFile(UserFilePath);
            croot["PATH"] = ServerFilePath;
            //给客户端发过去
            char * buf = croot.toStyledString();
            send(socketfd , (void *)buf , sizeof(buf) , 0);
        }

        //根据用户给的行为来判断需要调用什么函数
        void UserRequest(std::string & buf , int & socketfd) {
            Json::Reader reader;
            Json::Value root;
            int status;

            if(reader.parse(buf,root))  {
                status = root["status"].asInt();
            }

            switch (status):
            case 1:AccountPasswd(root , socketfd);break;
            case 2://注册  先判断数据库有没有该用户
                ;break;
            case 3://进入目录  返回该目录下的文件
                ;break;
            case 4://新建目录  返回空目录
                ;break;
            case 5://重命名文件或目录   修改数据库
                ;break;
            case 6://删除目录或者文件  修改数据库
                ;break;
            case 7://上传文件  找到空闲服务器  给客户端返回子服务器Ip及端口
                ;break;
            case 8://下载文件  找到文件的服务器地址  给客户端返回文件路径
                ;break;
            case 9://监控文件  将客户端指定的信息写入数据库
                ;break;
            case 10://恢复文件 返回文件所有数据
                ;break;
a        }


        void RecvFromClient(const int & socketfd) {
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
                    UserRequest(buf,socketfd);
                }
            }
        }

        void EpollMission( char * ip , char * port )  {
            int num;
            int socketfds[MaxClientConnection];
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
                        RecvFromClient(socketfd);
                    }

                }

            }
            close(e.socketfd);
        }

    private:
        Epoll e;
        MyDataBase db[MaxClientConnection];
};
