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
#include <json/json.h>
#include <unistd.h>
#include <fcntl.h>


/* constructor  */
workServer::
workServer(std::string m_ip, int m_port, std::string l_ip, int l_port):
    ip(m_ip), port(m_port), threadpool(10), loadClient(l_ip, l_port)
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

    /* set recv buffer size */
    int recvSize = 200000;
    int recvLen = sizeof(recvSize);
    int recv_sret = setsockopt(listen_fd, SOL_SOCKET, SO_RCVBUF, &recvSize, sizeof(recvSize));
    if(recv_sret < 0)
    {
        perror("recv buffer setsockopt error\n");
        exit(1);
    }
    /* get recv buffer size */
    int recv_gret = getsockopt(listen_fd, SOL_SOCKET, SO_RCVBUF, &recvSize, (socklen_t*)&recvLen);
    if(recv_gret < 0)
    {
        perror("recv buffer getsockopt error\n");
        exit(1);
    }
    std::cout << "size:" << recvSize << std::endl;

    /* set send buffer size */
    int sendSize = 200000;
    int sendLen = sizeof(sendSize);
    int send_sret = setsockopt(listen_fd, SOL_SOCKET, SO_SNDBUF, &sendSize, sizeof(sendSize));
    if(send_sret < 0)
    {
        perror("send buffer setsockopt error\n");
        exit(1);
    }
    /* get send buffer size */
    int send_gret = getsockopt(listen_fd, SOL_SOCKET, SO_SNDBUF, &sendSize, (socklen_t*)&sendLen);
    if(send_gret < 0)
    {
        perror("send buffer getsockopt error\n");
        exit(1);
    }
    std::cout << "size:" << sendSize << std::endl;

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

    /* create epoll fd and Register fd epoll collection */
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
    /* json parse */
    Json::Reader reader;
    Json::Value value;

    /* mark and md5 */
    int mark = 0;
    std::string md5;

    /* main loop */
    while(1)
    {
        int ret = epoll_wait(epoll_fd, events, maxCon, -1);
        if(ret < 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
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
                //std::cout << "添加到任务队列中,等待线程处理" << std::endl;
                
                /* receive handler pass on threadpool */ 
                char buffer[1024];
                recv(sockfd, buffer, 1024, 0);
                printf("%s\n", buffer);
                std::string jjson(buffer);
                std::cout << "jjson:" << jjson << std::endl;
                if(reader.parse(jjson, value))
                {
                    if(!value["mark"].isNull())
                    {
                        /* obtain mark */
                        mark = value["mark"].asInt();
                        /* obtain file md5 */
                        md5 = value["md5"].asString();
                        std::cout << mark << " " << md5 << std::endl; 
                    }
                }

                /* judge mark */
                if(mark == 1)
                {
                    std::cout << "准备开始上传啦" << std::endl;
                    /* download */
                    Handler hand = std::make_tuple(workServer::handler_download, sockfd, md5);
                    threadpool.AddTask(std::move(hand));
                }else if(mark == 2)
                {
                    /* upload */
                    std::cout << "upload ..." << std::endl;
                    Handler hand = std::make_tuple(workServer::handler_upload, sockfd, md5);
                    threadpool.AddTask(std::move(hand));
                }
            }
        }
    }
}



/* download file */
bool
workServer::
handler_download(int fd, std::string md5)
{
    std::cout << "要开始发送文件啦" << std::endl;
    std::cout << "downdaload:" << fd << " MD:" << md5 << std::endl;
    std::string filename(md5);
    /* sendfile transport file */
    
    std::cout << filename << std::endl;
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
      //if(n == -1)
      //{
      //    std::cout << "sendfile error " << std::endl;
      //    break;
      //}
     // pos += n;
    }
    
    if(n == -1)
    {
        perror("send file error");
        exit(1);
    }else{
        std::cout << "send size:" << n << std::endl;
    }

    if(n >= st.st_size)
    {
        std::cout << "send file success " << md5 << std::endl;
    }
}



/* upload file */
bool
workServer::
handler_upload(int fd, std::string md5)
{
    std::cout << "upload:" << fd << " MD:" << md5 << std::endl;
    std::string filename(md5);
    
    /* open file */
    int w_fd = open(filename.c_str(), O_WRONLY | O_CREAT, 00700);
    std::cout << "open file" << std::endl;
    if(w_fd == -1)
    {
        perror("open file error\n");
        exit(1);
    }

    char buffer[2048];
    while(1)
    {
        bzero(buffer, 2048);
        int ret = recv(fd, buffer, 2048, 0);
        if(ret == -1)
        {
            continue;
        }else if(ret == 0)
        {
            std::cout << "success " << std::endl;
            break;
        }
        std::cout << ret << std::endl;
        write(w_fd, buffer, ret);
    }


/*  
    int pipefd[2];
    int ret = pipe(pipefd);
    std::cout << "start upload ...." << std::endl;
    int ret2;
    while(1)
    {
        //int t = recv(fd, bf, 1000, 0);
        //printf("t = %d\n", t);
        ret = splice(fd, NULL, pipefd[1], NULL, 32768,
                SPLICE_F_MORE | SPLICE_F_MOVE);
        std::cout << "recv:" << ret << std::endl;
        if(ret == 0)
        {
            break;
        }
        else if(ret == -1)
        {
            //perror("splice error1\n");
            continue;
            //exit(1);
        }else{
            ret2 = splice(pipefd[0], NULL, w_fd, NULL, ret,
                    SPLICE_F_MORE | SPLICE_F_MOVE);
        }
        if(ret2 == -1)
        {
            //perror("splice error2\n");
            continue;
            //exit(1);
        }
    }
    */
}

