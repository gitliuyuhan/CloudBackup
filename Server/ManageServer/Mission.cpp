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

#define MaxClientConnection 2


class Mission{
    public:
        Mission(){}
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
            std::string OldName , NewName;
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
        void DownloadFile(Json::Value root , int & socketfd)  {
            Json::Value croot;
            std::string UserFilePath , ServerFilePath;
            UserFilePath = root["UserFilePath"].asString();
            ServerFilePath = db[socketfd].DownloadFile(UserFilePath);
            croot["PATH"] = ServerFilePath;
            //给客户端发过去
            std::string s = croot.toStyledString();
            const char * buf = s.c_str();
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
        void RestoreFile(Json::Value root , int & socketfd)  {
            Json::Value croot;
            std::string UserFilePath , ServerFilePath;
            UserFilePath = root["UserFilePath"].asString();
            ServerFilePath = db[socketfd].DownloadFile(UserFilePath);
            croot["PATH"] = ServerFilePath;
            //给客户端发过去
            std::string s = croot.toStyledString();
            const char * buf = s.c_str();
            send(socketfd , (void *)buf , sizeof(buf) , 0);
        }

        //根据用户给的行为来判断需要调用什么函数
        void UserRequest() {
            Json::Reader reader;
            Json::Value root;
            int status;

            if(reader.parse(buf,root))  {
                status = root["status"].asInt();
            }
            switch (status)
            {
                case 1:AccountPasswd(root , socketfd);break;
                case 2:Regiester(root , socketfd);break;
                case 3:DirFiles(root , socketfd);break;
                case 4:CreateNewDir(root , socketfd);break;
                case 5:RenameFileName(root , socketfd);break;
                case 6:DeleteFileORDir(root , socketfd);break;
                case 7:UploadFile(root , socketfd);break;
                case 8:DownloadFile(root, socketfd);break;
                case 9:MonitorFile(root , socketfd);break;
                case 10:RestoreFile(root , socketfd);break;
            }
        }
    public:
        MyDataBase db[MaxClientConnection];
        char buf[512];
        int socketfd;
};
