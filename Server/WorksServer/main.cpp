/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月02日 20时10分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (wangweihao), 578867817@qq.com
 *        Company:  xiyoulinuxgroup
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Server.h"

int main(int argc, char *argv[])
{
    std::string ip(argv[1]);
    int port = atoi(argv[2]);
    std::string l_ip(argv[3]);
    int l_port = atoi(argv[4]);
	workServer server(ip, port, l_ip, l_port);
    server.Run();

	return EXIT_SUCCESS;
}


