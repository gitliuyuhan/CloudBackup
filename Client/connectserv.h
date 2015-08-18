/*======================================================
    > File Name: connectserv.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月06日 星期四 00时12分08秒
 =======================================================*/

#ifndef CONNECTSERV_H
#define CONNECTSERV_H

#include"myinclude.h"

#include<QString>

int  ConnectServer(QString,QString);
void closefd(int);
int  openfile(const char*);
int  openfile(const char* pathname,int flags);

#endif//CONNECTSERV_H
