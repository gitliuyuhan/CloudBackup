/*************************************************************************
  > File Name: MyDataBase.cpp
  > Author: lewin
  > Mail: lilinhan1303@gmail.com
  > Company:  Xiyou Linux Group
  > Created Time: 2015年08月02日 星期日 17时45分16秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<cstdlib>
#include<cerrno>
#include<mysql/mysql.h>
#include"jsoncpp-src-0.5.0/include/json/json.h"

#define HOST "localhost"
#define DATABASE ""
#define USER ""
#define PASSWD ""
#define PORT 3306

class MyDataBase {
    public:
        MyDataBase() :res(NULL) {
            mysql_init(&mysql);
            /*连接数据库*/
            if(!mysql_real_connect(&mysql , HOST , USER , DATABASE , PASSWD , DATABASE , PORT , NULL , 0))  {
                std::cout << "连接数据库发生错误!"  << std::endl;
                return;
            }
            std::cout << "连接数据库成功!" << std::endl;
        }
        ~MyDataBase() {
            mysql_close(&mysql);
        }

        /*查询数据库  参数为sql语句*/
        void MySqlQuery(std::string & sql)  {
            if((t = mysql_real_query(&mysql , sql , (unsigned int)strlen(sql))))  {
                std::cout << "Error Making Query:" << mysql_error(&mysql) << std::endl;
            } else {
                res = mysql_store_result(&mysql);
            }
        }

        /*验证账户和密码 返回-1代表登录失败 1为登录成功*/
        int AccountPasswd(std::string UserName , std::string PassWord) {
            std::string sql = "select * from UserInfo where Acount=" + UserName+";";
            MySqlQuery(sql);
            while(row = mysql_fetch_row(res)) {
                //判断UserName和Account是否相等
                if(UserName != row[1] || PassWord != row[2])  {
                    return -1;
                }
                else {
                    Uid = row[0];
                    return 1;
                }
            }
            return -1;
        }

        /*注册用户  先判断有没有此用户*/
        int Register(std::sting UserName , std::string PassWord , std::string Email)  {
            std::string sql = "select * from UserInfo where Acount=" + UserName+";";
            MySqlQuery(sql);
            if(res->row_count != NULL)  {
                std::cout << "该用户已存在" << std::endl;
                return 0;
            }else {
                sql = "insert into UserInfo(Account,Password,Email) values(" + UserName +","+PassWord+","+Email+");";
                if( 0 == mysql_query(&mysql , sql.c_str())) {
                    //[未完成]数据成功的log
                    return 1;
                }else {
                    //[未完成]数据失败的log
                    mysql_close(&mysql);
                    return -1;
                }
            }
            return 0;
        }
        //进入目录  在数据库中查找符合条件的目录  并且将该目录下的一级文件或者目录
        std::string DirFiles(std::string dir)  {
            Json::Value root;
            std::string sql = "select * from UserInfo where UserFilePath like " + dir + "$;";
            MySqlQuery(sql);
            int i = 0;
            int num = dir.length();
            std::string temp , str;
            while(row = mysql_fetch_row(res)) {
                str = row;
                int loc = str.find('/' , num);
                if(loc)  {
                    temp = str.substr(num , loc - num + 1);
                }else {
                    temp = str.substr(num - 1 , str.rfind("\0"));
                }
                std::string numstr = std::to_string(i);
                root[numstr] = temp;
            }
            return root.toStyledString();
        }

        //新建目录
        int CreateNewDir(std::string DirFiles)  {
            std::string sql = "insert into UserFileInfo(Uid ,UserFileSize , UserFilePath) values("+Uid+","+0+","+DirFiles+");";
            if(0 != mysql_query(sql)) {
                return -1;
            }
            return 1;
        }

        //重命名文件或目录
        int RenameFileName(std::string OldName , std::string NewName)  {
            std::string sql = "update UserFileInfo set UserFilePath="+"NewName "+"where UserFilePath="+OldName+";";
            if(0 != mysql_query(&mysql,sql))  {
                return -1;
            }
            return 1;
        }

        //删除目录或文件
        int DeleteFileORDir(std::string File)  {
            std::string sql = "select MD5 from UserFileInfo where UserFilePath="+File+");";
            MySqlQuery(&mysql , sql);
            row = mysql_fetch_row(res);
            sql = "select * from MD5 where MD5="+row[0]+";";
            MySqlQuery(&mysql , sql);
            row = mysql_fetch_row(res);
            if( row[2] = 1 )  {
                sql = "delete from UserFileInfo where UserFilePath="+File+";";
                mysql_query(&mysql , sql);
            }
            sql = "update MD5 set ReferCount="+std::to_string(row[2]-1)+"where Mid="+std::to_string(row[0])+";";
            if( 0 != mysql_query(&mysql , sql))  {
                return -1;
            }else {
                return 1;
            }

        }

        //上传文件
        int UploadFile(std::string UserFilePath , std::string UserFileSize , std::string ServerFilePath , std::string MD5 , int flag)  {
            std::string sql = "select MD5 from UserFileInfo where UserFilePath="+UserFilePath+");";
            MySqlQuery(&mysql , sql);
            row = mysql_fetch_row(res);
            sql = "select * from MD5 where MD5="+row[0]+";";
            MySqlQuery(&mysql , sql);
            row = mysql_fetch_row(res);
            if(row[2] >= 1)  {
                sql = "update MD5 set ReferCount="+std::to_string(row[2]+1)+"where Mid="+std::to_string(row[0])+";";
                mysql_query(&mysql , sql);
            }else {
                std::string sql = "insert into UserFileInfo(Uid , UserFilePath , UserFileSize , ServerFilePath , MD5 , Flag) values("+std::to_string(Uid)+","+UserFileSize+","+std::to_string(UserFileSize)+","+ServerFilePath+","+MD5+","+std::to_string(flag)+");";
                if( 0 != mysql_query(&mysql , sql))  {
                    return -1;
                }
                return 1;
            }
        }
        //下载文件
        std::string DownloadFile(std::string UserFilePath)  {
            std::string sql = "select ServerFilePath from UserFileInfo where UserFilePath="+UserFilePath+";";
            MySqlQuery(sql);
            std::string ServerPath;
            while(row = mysql_fetch_row(res)) {
                ServerPath = row[0];
            }
            return ServerPath;
        }

        //监控文件
        int MonitorFile(std::string UserFilePath , std::string UserFileSize , std::string ServerFilePath , std::string MD5 , int flag)  {
            int flag = UploadFile(UserFilePath , UserFileSize , ServerPath , MD5 , flag);
            if( 1 == flag )   {
                return true;
            }
            return false;
        }

        //恢复文件
        std::string RestoreFile(std::string UserFilePath)  {
            std::string temp = DownloadFile(UserFilePath);
            return temp;
        }
    private:
        MYSQL mysql;  //连接数据库的变量
        MYSQL_RES *res;  //存放查询结果的变量
        MYSQL_ROW row;
        int Uid;  //用户ID
};

