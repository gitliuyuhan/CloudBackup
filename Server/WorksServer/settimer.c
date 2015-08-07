/*
 * =====================================================================================
 *
 *       Filename:  settimer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月05日 08时19分04秒
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
#include <signal.h>
#include <sys/time.h>

void signalHandler(int sig)
{
    switch(sig)
    {
        case SIGALRM:
            printf("caught the SIGALRM signal\n");
            break;
    }
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, signalHandler);

    struct itimerval new_value, old_value;
    new_value.it_value.tv_sec = 0;
    new_value.it_value.tv_usec = 1;
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &new_value, &old_value);	

    while(1);

	return EXIT_SUCCESS;
}


