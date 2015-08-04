/*
 * =====================================================================================
 *
 *       Filename:  Server.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月02日 19时55分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (wangweihao), 578867817@qq.com
 *        Company:  xiyoulinuxgroup
 *
 * =====================================================================================
 */

#include <Server.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

bool func(int fd, std::string json)
{
    std::cout << "文件描述符为:" << fd << " Json:" << json << std::endl;
    return true;
}

workServer::
workServer(std::string m_ip, int m_port):
    ip(m_ip), port(m_port), threadpool(10)
{
    /* 初始化服务端socket配置 */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);
    /*创建普通socket */
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        perror("create socket error\n");
        exit(1);
    }
    /* 绑定端口 */
    int ret = bind(listen_fd, (struct sockaddr*)&server, sizeof(server));
    if(ret == -1)
    {
        perror("bind socket error\n");
        exit(1);
    }
    /* 转换为监听socket，将backlog设置为1024 */
    ret = listen(listen_fd, 1024);
    if(ret == -1)
    {
        perror("listen error\n");
        exit(1);
    }
    epoll_fd = epoll_create(maxCon);
    Register(listen_fd, false);
}

/* destroy function  close socket fd*/
workServer::
~workServer()
{
    close(listen_fd);
    close(epoll_fd);
}


/* set socket fd nonblocking */
int
workServer::
setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    int ret = fcntl(fd, F_SETFL, new_option);
    return old_option;
}


/* register sockfd in epoll */
int 
workServer::
Register(int fd, bool oneshot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(oneshot)
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
    return 0;
}

/* epoll run */
int 
workServer::
Run()
{
    while(1)
    {
        int ret = epoll_wait(epoll_fd, events, maxCon, -1);
        if(ret < 0)
        {
            perror("epoll_wait error\n");
            exit(1);
        }
        for(int i = 0; i < ret; i++)
        {
            int sockfd = events[i].data.fd;
            if(sockfd == listen_fd)
            {
                struct sockaddr_in client_address;
                socklen_t len = sizeof(client_address);
                int connfd = accept(listen_fd, (struct sockaddr*)&client_address, &len);
                Register(connfd, true);
                std::cout << "接受连接并注册" << std::endl;
            }
            else if(events[i].events & EPOLLIN)
            {
                //std::cout << "读事件产生" << std::endl;
                //std::cout << "添加到任务队列中,等待线程处理" << std::endl;
                /* receive handler pass on threadpool */ 
                //Handler hand = std::make_tuple(&workServer::handler_upload, 10, "hello world");
                char buffer[1024];
                recv(sockfd, buffer, 1024, 0);
                std::string s(buffer);
                printf("%s\n", buffer);
                /* download */
                //Handler hand = std::make_tuple(workServer::handler_download, sockfd, s);
                //threadpool.AddTask(std::move(hand));
                /* upload */
                Handler hand = std::make_tuple(workServer::handler_upload, sockfd, s);
                threadpool.AddTask(std::move(hand));

            }
        }
    }
}

/* download file */
bool
workServer::
handler_download(int fd, std::string json)
{
    std::cout << "downdaload:" << fd << " MD:" << json << std::endl;
    std::string filename(json);
    /* sendfile transport file */
    
    std::cout << json << std::endl;
    int r_fd = open(filename.c_str(), O_RDONLY);
    if(r_fd == -1)
    {
        perror("open file error\n");
        exit(1);
    }
    struct stat st;
    
    std::cout << "stat" << std::endl;
    long int size = stat(filename.c_str(), &st);
    if(size < 0)
    {
        perror("gain file stat error\n");
        exit(1);
    }
    
    std::cout << "lseek" << std::endl;
    off_t pos = lseek(r_fd, 0, SEEK_SET);
    if(pos < 0)
    {
        perror("obtain file pointer error\n");
        exit(1);
    }
    
    std::cout << "sendfile" << std::endl;
    //int n = sendfile(fd, r_fd, &pos, st.st_size);
    int n;
    long int s = 0;
    while(n = sendfile(fd, r_fd, &pos, st.st_size))
    {
        s += n;
    }
    if(n == -1)
    {
        perror("send file error\n");
        exit(1);
    }
    
    std::cout << s << " "<< st.st_size <<  std::endl;
    if(n >= st.st_size)
    {
        std::cout << "send file success " << json << std::endl;
    }
}


/* upload file */
bool
workServer::
handler_upload(int fd, std::string json)
{
    std::cout << "upload:" << fd << " MD:" << json << std::endl;
    std::string s("../");
    std::string filename(json);
    /* open file */
    int w_fd = open(filename.c_str(), O_WRONLY | O_CREAT, 00700);
    if(w_fd == -1)
    {
        perror("open file error\n");
        exit(1);
    }
    char buffer[20480];
    while(1)
    {
        bzero(buffer, 20480);
        int n = recv(fd, buffer, 20480, 0);
        if(n < 0)
        {
            perror("recv error\n");
            exit(1);
        }else if(n == 0)
        {
            break;
        }
        ssize_t s_t = write(w_fd, buffer, n);
        if(s_t < 0)
        {
            perror("write error\n");
            exit(1);
        }
    }
}

