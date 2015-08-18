/*************************************************************************
    > File Name: trance.cpp
    > Author: MiaoShuai
    > Mail: 945970809@qq.com 
    > Created Time: 2015年08月04日 星期二 20时01分54秒
 ************************************************************************/

#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>
#include<assert.h>
//#include<pthread.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/sendfile.h>
#include<string.h>
#include<sys/stat.h>

using namespace std;

void RequstDownload(int sockFd,std::string filename)
{
	std::cout<<"开始下载"<<endl;
	
	cout<<"sockFd = "<<sockFd<<endl;
	//打开并创建与上面filename相同的文件
	int fileFd;
	if((fileFd = open(filename.c_str(),O_WRONLY | O_CREAT | O_EXCL,S_IRUSR | S_IWUSR | S_IXUSR)) < 0)
	{
		std::cout<<"文件打开失败"<<endl;
		exit(1);
	}
	cout<<"fileFd = "<<fileFd<<endl;
	
	//获取文件属性
	struct stat statFile;
	fstat(sockFd,&statFile);
	
	//使用splice拷贝文件
	int pipeFd[2];
	int ret;

	//创建管道
	ret = pipe(pipeFd);
	if(ret < 0)
	{
		std::cout<<"管道创建失败"<<endl;
	}
	
	int sum = 0;

	//从发送端套接字将文件读入到管道
	while((ret = splice(sockFd,NULL,pipeFd[1],NULL,32768,SPLICE_F_MOVE)) > 0)
	{
		sum = sum + ret;
		
		//cout<<"ret11 = "<<ret<<endl;
		//从管道读出内容写入文件
		ret = splice(pipeFd[0],NULL,fileFd,NULL,32768,SPLICE_F_MOVE);
		
		//cout<<"ret22 = "<<ret<<endl;
		if(ret <= 0)
		{
			break;
		}
	}

	if(ret == -1)
	{
		cout<<"splice调用失败"<<endl;
	}

	cout<<"sum = "<<sum<<endl;

	if(sum == statFile.st_size)
	{
		cout<<"文件写入成功"<<endl;
	}
}



void RequestUpload(int sockFd,string filename)
{
	std::cout<<"开始上传"<<endl;

	//打开文件
	int fileFd;

	if((fileFd = open(filename.c_str(),O_RDONLY)) == -1)
	{
		std::cout<<"文件打开失败"<<endl;
	}

	//获取文件属性
	struct stat statFile;
	cout<<"111"<<endl;
	fstat(fileFd,&statFile);

	//用sendfile将文件发给接收端
	
	cout<<"statFile.st_size = "<<statFile.st_size<<endl;
	int sum = 0;
	ssize_t ret = 0;
	cout<<"nihao"<<endl;
	cout<<"sockFd = "<<sockFd<<endl;
	cout<<"fileFd = "<<fileFd<<endl;


	off_t pos = lseek(fileFd,0,SEEK_SET);
	if(pos < 0)
	{
		cout<<"error"<<endl;
	}

	ret = sendfile(sockFd,fileFd,&pos,statFile.st_size);
	cout<<"statFile.st_size = "<<statFile.st_size<<endl;
	cout<<"ret = "<<ret<<endl;
	sum = sum + ret;
	close(fileFd);

	cout<<"sum = "<<sum<<endl;

	if(ret == -1)
	{
		std::cout<<"使用sendfile失败"<<endl;
	}

	if(sum >= statFile.st_size)
	{
		std::cout<<"文件上传成功"<<endl;
	}
}


int main(int argc,char **argv)

{	
	if(argc < 3)
	{
		std::cout<<"您输入的参数有误"<<endl;
		exit(1);
	}

	const char *ip = argv[1];
	int port = atoi(argv[2]);
	sockaddr_in address;

	bzero(&address,sizeof(address));

	address.sin_family = AF_INET;
	inet_pton(AF_INET,ip,&address.sin_addr);
	address.sin_port = htons(port);

	int connFd = socket(AF_INET,SOCK_STREAM,0);
	assert(connFd >= 0);

	int ret = 0;

	ret = connect(connFd,(sockaddr *)&address,sizeof(address));

	char s[50] = "{\"mark\":1,\"md5\":\"/home/shreck/haha.jpg\"}"; 

	//std::cout<<"1.选择上传\n"<<"2.选择下载"<<endl;
	assert(ret >= 0);
	send(connFd,s,50,0);
	//int x;
	//cin>>x;
	string filename = "/tmp/haha.jpg";
	//if(x == 1)
	//{
		//RequestUpload(connFd,filename);
	//}

	//else if(x == 2)
	//{
		RequstDownload(connFd,filename);
	//}


	

	
	return 0;
}
