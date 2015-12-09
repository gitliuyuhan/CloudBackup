/*************************************************************************
  > File Name: epoll.hpp
  > Author: lewin
  > Mail: lilinhan1303@gmail.com
  > Company:  Xiyou Linux Group
  > Created Time: 2015年07月27日 星期一 08时40分51秒
 ************************************************************************/

#include<iostream>
#include<array>
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

//#define SIZE                1000
#define MaxClientConnection 2
#define WORKPORT            "10001"

extern MyDataBase  DataBase;
extern std::string WorkIp;
extern std::string WorkPort;

class Mission{
    public:
        Mission()
        {}
        ~Mission() 
        {
            std::cout<<"该任务销毁\n";
        }

        void MissionInit(int sock)
        {
            socketfd = sock;
            Uid = Users[socketfd];
        }

        //验证账户密码任务
        int AccountPasswd(Json::Value & root , int & socketfd) {
            std::string UserName , Passwd, rootFolder;
            UserName = root["UserName"].asString();
            Passwd = root["Passwd"].asString();
            int ret = DataBase.AccountPasswd(UserName , Passwd);
            if(ret >= 1)
            {
                //登录成功
                Users[socketfd] = ret;
                Uid = ret;
                std::cout<<"log Uid:"<<Uid<<std::endl;
                //获取根目录文件
                rootFolder = DataBase.DirFiles(Uid,"/");
                root["status"] = Json::Value(0);
                root["AllFiles"] = Json::Value(rootFolder);
                root["Folder"] = Json::Value("/");
                strcpy(buf, root.toStyledString().c_str());
                std::cout<<"logsu :"<<rootFolder<<std::endl;
                send(socketfd,buf,strlen(buf),0);
                return 1;
            }
            else if(ret==0)
            {
                //账号错误
                root["status"] = Json::Value(1);
                strcpy(buf,root.toStyledString().c_str());
                send(socketfd,buf,strlen(buf),0);
                return 0;
            }
            else
            {
                //密码错误
                root["status"] = Json::Value(2);
                strcpy(buf,root.toStyledString().c_str());
                send(socketfd,buf,strlen(buf),0);
                return -1;
            }
        }

        //注册用户
        int Regiester(Json::Value & root , int & socketfd)  {
            std::string UserName , Passwd , Email;
            UserName = root["UserName"].asString();
            Passwd = root["Passwd"].asString();
            Email = root["Email"].asString();

            int ret = DataBase.Register(UserName , Passwd , Email);
            std::cout<<"ret="<<ret<<std::endl;
            if(ret==1)
            {
                //注册成功
                root["status"] = Json::Value(0);
                strcpy(buf, root.toStyledString().c_str());
                send(socketfd,buf,strlen(buf),0);
                return 1;
            }
            else if(ret==0)
            {
                //用户已存在
                root["status"] = Json::Value(1);
                strcpy(buf,root.toStyledString().c_str());
                send(socketfd,buf,strlen(buf),0);
                return 0;
            }
            else if(ret==2)
            {
                //邮箱已被占用
                root["status"] = Json::Value(2);
                strcpy(buf,root.toStyledString().c_str());
                send(socketfd,buf,strlen(buf),0);
                return 2;
            }
            else
            {
                std::cout<<"添加数据失败"<<std::endl;
                return -1;
            }

        }

        //进入目录
        void  DirFiles(Json::Value root , int & socketfd)  {
            std::string  folder,AllFiles;
            folder = root["Folder"].asString();
            AllFiles = DataBase.DirFiles(Uid,folder);
            root["AllFiles"] = Json::Value(AllFiles);
            strcpy(buf,root.toStyledString().c_str());
            send(socketfd,buf,strlen(buf),0);
        }
        //创建新目录
        int CreateNewDir(Json::Value root , int &  socketfd) {
            std::string DirName;
            DirName = root["UserFilePath"].asString();
            if( 1 ==DataBase.CreateNewDir(DirName)) {
                return 1;
            }
            return -1;
        }
        //重命名文件或目录
        int RenameFileName(Json::Value root , int & socketfd) {
            std::string OldName , NewName;
            OldName = root["OldName"].asString();
            NewName = root["NewName"].asString();
            if( 1 == DataBase.RenameFileName(OldName , NewName)) {
                return 1;
            }
            return -1;
        }
        //删除文件或目录
        int DeleteFileORDir(Json::Value root , int & socketfd)  {
            std::string File;
            File = root["UserFilePath"].asString();
            if( 1 == DataBase.DeleteFileORDir(File))  {
                return 1;
            }
            return -1;
        }

        //上传文件
        int UploadFile(Json::Value root , int & socketfd)  {
            std::cout<<"start uid:"<<Uid<<std::endl;
            std::cout<<root.toStyledString()<<std::endl;
            std::string MD5,UserFilePath,File,FilePath;
            //获取上传文件名
            FilePath = root["File"].asString();
            std::string::size_type    position;
            position = FilePath.find_last_of("/");
            File = FilePath.substr(position+1,FilePath.length());

            MD5 = root["Md5"].asString();
            int   have = root["Have"].asInt();

            //上传到客户端的路径
            UserFilePath = root["Path"].asString() + File;

            std::string      UserFileSize;
            std::string      ServIp;
            if(have == 1)
            {
                UserFileSize = root["Size"].asString();
                ServIp = root["Ip"].asString();
            }


            int ret = DataBase.UploadFile(MD5,UserFilePath,Uid,ServIp,UserFileSize,have);
            if(ret == 1)
            {
                //秒传
                root["Have"] = Json::Value(1);
                strcpy(buf,root.toStyledString().c_str());
                send(socketfd,buf,strlen(buf),0);
                return 1;
            }
            else if(ret == 0)
            {
                //返回子服务器信息
                root["Ip"] = Json::Value(WorkIp);
                root["Port"] = Json::Value(WorkPort);
                root["Have"] = Json::Value(0);
                strcpy(buf,root.toStyledString().c_str());
                send(socketfd,buf,strlen(buf),0);
                return 0;
            }
            else if(ret == 2)
            {
                return 2;
            }
            return -1;
        }

        //下载文件
        void DownloadFile(Json::Value root , int & socketfd)  {

            std::string UserFilePath , ServerIp , FileSize,FileMd5;
            UserFilePath = root["Path"].asString();

            MYSQL_RES*    res;
            MYSQL_ROW     row;
            res = DataBase.DownloadFile(UserFilePath,Uid);
            printf("res 2 =%p\n",res);
            while((row = mysql_fetch_row(res))!=NULL)
            {
                ServerIp = row[5];  //服务器文件路径
                FileSize = row[3];  //用户文件大小
                FileMd5 = row[6];   //文件md5
                break;
            }
            mysql_free_result(res);
            root["Ip"] = Json::Value(ServerIp);   //子服务器Ip
            root["Port"] = Json::Value(WORKPORT); //子服务器端口
            root["Size"] = Json::Value(FileSize);
            root["Md5"] = Json::Value(FileMd5);

            strcpy(buf,root.toStyledString().c_str());
            std::cout<<buf<<std::endl;
            //给客户端发过去
            send(socketfd , buf , strlen(buf) , 0);
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

            if( true == DataBase.MonitorFile(UserFileSize , UserFileSize , ServerFilePath , MD5 , flag))  {
                return 1;
            }
            return -1;
        }

        //恢复文件
        void RestoreFile(Json::Value root , int & socketfd)  {
            Json::Value croot;
            std::string UserFilePath , ServerFilePath;
            UserFilePath = root["UserFilePath"].asString();
  //          ServerFilePath = DataBase.DownloadFile(UserFilePath);
            croot["PATH"] = ServerFilePath;
            //给客户端发过去
            std::string s = croot.toStyledString();
            const char * buf = s.c_str();
            send(socketfd , (void *)buf , sizeof(buf) , 0);
        }

        void Banlance(Json::Value root,int & socketfd)
        {
            std::cout<<root.toStyledString();
            WorkIp = root["ip"].asString();
            WorkPort = root["port"].asString();
            std::cout<<"WorkIp:"<<WorkIp<<"  "<<"WorkPort:"<<WorkPort<<std::endl;
        }

        //根据用户给的行为来判断需要调用什么函数
        void UserRequest() {
            Json::Reader reader;
            Json::Value root;
            int status;

            if(reader.parse(buf,root))  {
                status = root["status"].asInt();
                std::cout<<"status:"<<status<<std::endl;
            }
            std::cout<<buf<<std::endl;
            switch (status)
            {
                case 0:
                    std::cout<<"Banlance"<<std::endl;
                    Banlance(root, socketfd);break;
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
//        MyDataBase db[MaxClientConnection];
        char    buf[1024];
        int     socketfd;
        int     Uid;      // 用户数据库ID
    private:
        static int Users[];  //保存登录用户的ID
};

