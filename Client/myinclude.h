/*======================================================
    > File Name: cinclude.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月12日 星期三 09时19分30秒
 =======================================================*/

#ifndef MYINCLUDE_H
#define MYCINCLUDE_H

/* C/C++头文件 */
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<time.h>
#include<pthread.h>
#include<dirent.h>

#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<map>
#include<vector>
using namespace std;

//自定义函数声明

//获取路径中的目录或文件名
string getRoadFolder(string);

#endif//MYCINCLUDE_H

