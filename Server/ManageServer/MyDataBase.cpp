/*************************************************************************
  > File Name: MyDataBase.cpp
  > Author: lewin
  > Mail: lilinhan1303@gmail.com
  > Company:  Xiyou Linux Group
  > Created Time: 2015年08月02日 星期日 17时45分16秒
 ************************************************************************/


#include<iostream>
#include<set>
#include<string>
#include<cstdlib>
#include<cerrno>
#include<mysql/mysql.h>
#include"jsoncpp-src-0.5.0/include/json/json.h"
#include<sstream>
#include<stdio.h>


#define HOST "localhost"
#define DATABASE "CloudBackup"
#define USER "root"
#define PASSWD "liuyuhan"
#define PORT 3306

class MyDataBase {
    public:
        MyDataBase() :res(NULL) {
            mysql_init(&mysql);
            /*连接数据库*/
            if(!mysql_real_connect(&mysql , HOST , USER , PASSWD , DATABASE , 0 , NULL , 0))  {
                std::cout << "连接数据库发生错误!"  << std::endl;
                return;
            }
            std::cout << "连接数据库成功!" << std::endl;

            std::string name = "lyh";
            std::string sql = "select * from UserInfo where Account = \"" +name+"\"";
            mysql_real_query(&mysql , sql.c_str() , sql.length());
            res = mysql_store_result(&mysql);

            if(res == NULL)
            {
                std::cout<<"数据库无数据\n";
            }
            else
            {
                while((row = mysql_fetch_row(res)) != NULL) {
                    std::cout << row[0] <<" " << row[1] << "  " << row[2] <<"  "<<row[3]<<std::endl;
                }
            }
            mysql_free_result(res);
        }
        ~MyDataBase() {
            std::cout << "exit" << std::endl;
            mysql_close(&mysql);
        }

        /*查询数据库  参数为sql语句*/
        void MySqlQuery(std::string & sql)  {
            std::cout << sql <<std::endl;
            int err = mysql_real_query(&mysql , sql.c_str(),sql.length());
            if(0 != err){
                std::cout << "Error Making Query:" << mysql_error(&mysql) << std::endl;
            } else {
                res = mysql_store_result(&mysql);
                printf("res &= %p\n",res);
            }
        }

        /*验证账户和密码 返回-1代表登录失败 1为登录成功*/
        int AccountPasswd(std::string UserName , std::string PassWord) {
            std::cout << UserName << "  " << PassWord << std::endl;
            std::string sql = "select * from UserInfo where Account=\"" + UserName+"\"";
         //   MySqlQuery(sql);
            std::cout<<sql<<std::endl;
            std::cout<<mysql_real_query(&mysql , sql.c_str(),sql.length());
            res = mysql_store_result(&mysql);
            printf("res= %p\n",res);
            while((row = mysql_fetch_row(res)) != NULL) {
                //判断UserName和Account是否相等
                if(UserName != row[1] || PassWord != row[2])  {
                    mysql_free_result(res);
                    return -1;
                }
                else {
                    mysql_free_result(res);
                    std::cout<<"Uid="<<atoi(row[0])<<std::endl;
                    return atoi(row[0]);
                }
            }
            mysql_free_result(res);
            return 0;
        }

        /*注册用户  先判断有没有此用户*/
        int Register(std::string UserName , std::string PassWord , std::string Email)  {
            std::string sql = "select * from UserInfo where Account=\""+ UserName+"\" OR Email=\"" + Email + "\";";
         //   MySqlQuery(sql);
            std::cout<<sql<<std::endl;
            mysql_query(&mysql,sql.c_str());
            res = mysql_store_result(&mysql);
            std::cout<<UserName<<"  "<<PassWord<<"  "<<Email<<std::endl;
            int i = 0 ;

            if(res == NULL)
            {
                mysql_free_result(res);
                return -1;
            }

            while ( (row = mysql_fetch_row(res)) != NULL)  {
                std::cout<<"开始验证用户注册信息\n";
                if( row[1] == UserName ) {
                    std::cout << "该用户已存在" << std::endl;
                    mysql_free_result(res);
                    return 0;
                }
                else if(row[3] == Email)
                {
                    std::cout<<"邮箱已被占用"<< std::endl;
                    mysql_free_result(res);
                    return 2;
                }
            }
            mysql_free_result(res);
            std::cout<<"注册信息合法\n";
            sql = "insert into UserInfo(Account,Password,Email) values(\"" + UserName +"\",\""+PassWord+"\",\""+Email+"\");";
            std::cout<<sql<<std::endl;
            if( 0 == mysql_query(&mysql , sql.c_str())) {
                //[未完成]数据成功的log
                return 1;
            }
            else {
                //[未完成]数据失败的log
                //mysql_close(&mysql);
                return -1;
            }
        }
        //进入目录  在数据库中查找符合条件的目录  并且将该目录下的一级文件或者目录
        std::string  DirFiles(int uid,std::string dir)  {
            char   sqlbuf[1024];
            int    len=0;
            sprintf(sqlbuf,"select distinct UserFilePath from UserFileInfo where Uid = %d AND UserFilePath Like '%s%%'",uid,dir.c_str());
            std::string  sql = sqlbuf;
            MySqlQuery(sql);
            std::string     str,files="";
            std::set<std::string>   stringSet;
            int             pos;
            std::cout<<"folder:\n";
            while((row = mysql_fetch_row(res))!=NULL)
            {
                str = row[0];
                std::cout<<str<<std::endl;
                str = str.erase(0,dir.length());
                pos = str.find_first_of('/',0);
                if(pos >= 0)
                {
                    str = str.substr(0,pos+1);
                }
                stringSet.insert(str);
            }
            mysql_free_result(res);
         
            if(stringSet.empty())
            {
                return files;
            }
            std::set<std::string>::iterator    t = stringSet.begin();
            files = *t;
            t++;
            for(;t!=stringSet.end();t++)
            {
                std::cout<<"s:"<<*t<<std::endl;
                files = files + " " + *t;
            }
            std::cout<<"files:"<<files<<std::endl;
            return files;
        }

        //新建目录
        int CreateNewDir(std::string DirFiles)  {
            std::ostringstream str;
            str << Uid;

            std::string idstr(str.str());
            std::string sql = "insert into UserFileInfo(Uid ,UserFileSize , UserFilePath) values( "+idstr+",0,"+DirFiles+");";
            if(0 != mysql_query(& mysql,sql.c_str())) {
                return -1;
            }
            return 1;
        }

        //重命名文件或目录
        int RenameFileName(std::string OldName , std::string NewName)  {
            std::string sql = "update UserFileInfo set UserFilePath="+NewName+"where UserFilePath="+OldName+";";
            if(0 != mysql_query(&mysql,sql.c_str()))  {
                return -1;
            }
            return 1;
        }

        //删除目录或文件
        int DeleteFileORDir(std::string File)  {
            std::string sql = "select MD5 from UserFileInfo where UserFilePath="+File+");";
            MySqlQuery(sql);
            row = mysql_fetch_row(res);
            std::string str = row[0];
            sql = "select * from MD5 where MD5="+str+";";
            MySqlQuery(sql);
            int i = 1;
            std::string temp;
            while((row = mysql_fetch_row(res))) {
                if( i%3 == 0 && strcmp(row[0],"1") == 0)  {
                    temp = row[0];
                    sql = "delete from UserFileInfo where UserFilePath="+File+";";
                    mysql_query(&mysql , sql.c_str());
                }
                i++;
            }
            int num = atoi(temp.c_str());
            num--;

            std::ostringstream string;
            string << num;

            std::string ss(string.str());
            sql = "update MD5 set ReferCount="+ss+"where MD5="+str+";";
            if( 0 != mysql_query(&mysql , sql.c_str()))  {
                return -1;
            }else {
                return 1;
            }

        }

        //上传文件
        int UploadFile(std::string MD5,std::string UserFilePath,int uid,std::string ServIp,std::string FileSize,int have)  {
            char     sqlbuf[1024];
            if( have == 1)
            {  
                sprintf(sqlbuf,"insert into UserFileInfo(Uid,UserFilePath,UserFileSize,ServerIp,MD5,Flag) values('%d','%s','%s','%s','%s','%d')",uid,UserFilePath.c_str(),FileSize.c_str(),ServIp.c_str(),MD5.c_str(),1);
                mysql_real_query(&mysql,sqlbuf,strlen(sqlbuf));
                memset(sqlbuf,'\0',sizeof(sqlbuf));
                sprintf(sqlbuf,"insert into MD5Info(MD5) values('%s')",MD5.c_str());
                mysql_real_query(&mysql,sqlbuf,strlen(sqlbuf));
                return 2;
            }

            std::cout<<"uid="<<uid<<std::endl;
            //查询Md5是否存在
            std::string sql = "select MD5 from MD5Info where MD5 = \""+MD5+"\"";
            MySqlQuery(sql);
            row = mysql_fetch_row(res);
            if(row == NULL)
            {
                mysql_free_result(res);
                return 0;  //文件不存在
            }
            mysql_free_result(res);
            sql = "update MD5Info set Count = Count+1 where MD5 = \"" + MD5 + "\"";
            if(mysql_real_query(&mysql,sql.c_str(),sql.length()) != 0)
            {
                std::cout<<"Md5 引用计数增加失败\n";
                return -1;
            }
            sql = "select ServerIp,UserFileSize  from UserFileInfo where MD5 = \"" + MD5 + "\"";
            MySqlQuery(sql);
            row = mysql_fetch_row(res);
            std::string  spath = row[0];
            std::string  UserFileSize = row[1];
            std::cout<<"spath:"<<spath<<"   "<<"UserFileSize:"<<UserFileSize<<std::endl;
            mysql_free_result(res);

            //添加一条数据
            sprintf(sqlbuf,"insert into UserFileInfo(Uid,UserFilePath,UserFileSize,ServerIp,MD5,Flag) values('%d','%s','%s','%s','%s','%d')",uid,UserFilePath.c_str(),UserFileSize.c_str(),spath.c_str(),MD5.c_str(),1);
            if(mysql_real_query(&mysql,sqlbuf,strlen(sqlbuf)) != 0)
            {
                std::cout<<"文件信息添加失败\n";
                sql = "update MD5Info set Count = Count-1 where MD5 = \"" + MD5 + "\"";
                mysql_real_query(&mysql,sql.c_str(),sql.length());
                return -1;
            }
            std::cout<<"文件信息添加成功\n";
            return 1;

        }
        //下载文件
        MYSQL_RES*   DownloadFile(std::string UserFilePath,int id)  {
            char   sqlbuf[1024];
            sprintf(sqlbuf,"select * from UserFileInfo where Uid = '%d' AND UserFilePath = '%s'",id,UserFilePath.c_str());
            std::string sql = sqlbuf;
            MySqlQuery(sql);
            return res;
        }

        //监控文件
        int MonitorFile(std::string UserFilePath , std::string UserFileSize , std::string ServerFilePath , std::string MD5 , int flag)  {
  //          int Flag = UploadFile(UserFilePath , UserFileSize , ServerFilePath , MD5 , flag);
            int  Flag;   //临时添加
            if( 1 == Flag )   {
                return true;
            }
            return false;
        }

        //恢复文件
        std::string RestoreFile(std::string UserFilePath)  {
//            std::string temp = DownloadFile(UserFilePath);
             std::string   temp; //暂时并没有什么卵用
            return temp;
        }
    private:
        MYSQL mysql;  //连接数据库的变量
        MYSQL_RES *res;  //存放查询结果的变量
        MYSQL_ROW row;
        int       Uid;
};

