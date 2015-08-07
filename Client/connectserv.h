/*======================================================
    > File Name: connectserv.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月06日 星期四 00时12分08秒
 =======================================================*/

#ifndef CONNECTSERV_H
#define CONNECTSERV_H

#include<QString>

#include<iostream>

#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


int  ConnectServer(QString,QString);
void closefd(int);

#endif//CONNECTSERV_H
