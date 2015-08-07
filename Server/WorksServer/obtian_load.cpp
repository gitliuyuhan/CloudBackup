/*
 * =====================================================================================
 *
 *       Filename:  obtian_load.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月05日 16时28分11秒
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
#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
	char *p = "sar -n DEV 2 | grep wlan0 > bandwitch.txt";
    system(p);

	return EXIT_SUCCESS;
}


