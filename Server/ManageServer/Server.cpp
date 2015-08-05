/*************************************************************************
    > File Name: Run.cpp
    > Author: lewin
    > Mail: lilinhan1303@gmail.com
    > Company:  Xiyou Linux Group
    > Created Time: 2015年07月28日 星期二 17时29分21秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<cstdlib>
#include<cerrno>
#include<functional>
#include<thread>
//#include"allHeader.hpp"
#include"Epoll.hpp"
#include"ThreadPoll.hpp"
#include"Mission.cpp"

int main( int argc , char * argv[] )  {
    if( 3 != argc ) {
        std::cout << "服务器配置有误！"<<std::endl;
        return -1;
    }
    Mission mission(argv[1] , argv[2]);
    std::thread EpollThread(&Mission::EpollMission , mission , argv[1] , argv[2]);
    EpollThread.join();
    return EXIT_SUCCESS;
}

