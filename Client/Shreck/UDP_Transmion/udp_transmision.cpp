/*************************************************************************
    > File Name: udp_transmision.cpp
    > Author: MiaoShuai
    > Mail: 945970809@qq.com 
    > Created Time: 2015年10月12日 星期一 21时10分19秒
 ************************************************************************/
#include"udp_transmision.h"

//连接状态

int status = UNCONNECT;

//将发送方套接字的建立封装于构造函数
SendFile::SendFile()
{
	string localIpAddress;

	//获得本地IP地址
	localIpAddress = GetLocalIpAddress();
	cout<<"本地地址:"<<localIpAddress<<endl;

	//获得广播地址
	addressBroadcast = GetBroadcastAddress();
	cout<<"广播地址:"<<addressBroadcast<<endl;
	//获取udpAdress地址
	udpPort = atoi(UDP_PORT);
	bzero(&udpAddress,sizeof(udpAddress));
	udpAddress.sin_family = AF_INET;
	inet_pton(AF_INET,addressBroadcast.c_str(),&udpAddress.sin_addr);
	udpAddress.sin_port = htons(udpPort);

	//获取本地地址,port与udp的相同
	localPort = atoi(LOCAL_PORT);
	bzero(&localAddress,sizeof(localAddress));
	localAddress.sin_family = AF_INET;
	inet_pton(AF_INET,localIpAddress.c_str(),&localAddress.sin_addr);
	
	localAddress.sin_port = htons(localPort);
	
	
	
	//获得udp套接字
	udpfd = socket(AF_INET,SOCK_DGRAM,0);

	//重新绑定端口
	int val = 1;
	if(setsockopt(udpfd,SOL_SOCKET,SO_REUSEADDR,(void *)&val,sizeof(int)) < 0)
	{
		cout<<"重置失败"<<endl;
	}


	//绑定套接字
	int ret = 0;
	
	ret = bind(udpfd,(sockaddr *)&localAddress,sizeof(localAddress));
	if(ret < 0)
	{
		cout<<"绑定失败\n"<<endl;
	}
	
	
	//将udpfd的属性设为广播
	int on = 1;
	if((setsockopt(udpfd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on))) == -1)
	{
		cout<<"设置失败"<<endl;
	}

}


//将接收方的套接字建立封装于构造函数中
RecvFile::RecvFile():recvDataSet(ComparePacketNumber)
{
	
	string localIpAddress;
	udpPort = atoi(UDP_PORT);

	
	//获得本地IP地址
	localIpAddress = GetLocalIpAddress();
	cout<<"本地地址"<<localIpAddress<<endl;
	
	//初始化本地接收地址
	bzero(&recvAddress,sizeof(recvAddress));

	//绑定本地接收地址
	recvAddress.sin_family = AF_INET;
	//inet_pton(AF_INET,localIpAddress.c_str(),&recvAddress.sin_addr);
	inet_aton("0.0.0.0",&recvAddress.sin_addr);
	recvAddress.sin_port = htons(udpPort);
	
	//创建套接字
	recvFd = socket(AF_INET,SOCK_DGRAM,0);
	
	
	//绑定套接字
	int ret = 0;
	ret = bind(recvFd,(sockaddr *)&recvAddress,sizeof(recvAddress));
	if(ret < 0)
	{
		cout<<"绑定套接字失败\n"<<endl;
	}
	
	
}

//获得广播地址
string SendFile::GetBroadcastAddress(void)
{
	ifaddrs *ifAddrStruct = NULL;
	
	char addressBuffer[100];
	string interfaceName1 = "wlan0";
	string interfaceName2 = "eth0";
	void *tmpAddressPtr = NULL;

	
	//获取ifconfig相关信息的结构体
	getifaddrs(&ifAddrStruct);

	//从链表中获取广播地址
	while(ifAddrStruct != NULL)
	{
		
		//cout<<ifAddrStruct->ifa_name<<endl;
		if((strcmp(ifAddrStruct->ifa_name,interfaceName1.c_str()) == 0) || (strcmp(ifAddrStruct->ifa_name,interfaceName2.c_str()) == 0)) 
		{
			
			tmpAddressPtr = &((struct sockaddr_in *)(ifAddrStruct->ifa_ifu.ifu_broadaddr))->sin_addr;
			inet_ntop(AF_INET,tmpAddressPtr,addressBuffer,INET_ADDRSTRLEN);
			
			if(addressBuffer[0] == '1' && addressBuffer[1] == '9' && addressBuffer[2] == '2')
			{
				return addressBuffer;
			}
		}
		ifAddrStruct = ifAddrStruct->ifa_next;
	}
		
	return NULL;
}

//获取本地IP地址
string GetLocalIpAddress(void)
{
	ifaddrs *ifAddrStruct = NULL;
	char addressBuffer[100];
	string interfaceName1 = "eth0";
	string interfaceName2 = "wlan0";
	void *tmpAddressPtr = NULL;

	//获取ifconfig相关信息的结构体
	getifaddrs(&ifAddrStruct);

	//从链表中获取广播地址
	while(ifAddrStruct != NULL)
	{	
		
		if((strcmp(ifAddrStruct->ifa_name,interfaceName1.c_str()) == 0) || (strcmp(ifAddrStruct->ifa_name,interfaceName2.c_str()) == 0)) 
		{
			
			tmpAddressPtr = &((struct sockaddr_in *)(ifAddrStruct->ifa_addr))->sin_addr;
			inet_ntop(AF_INET,tmpAddressPtr,addressBuffer,INET_ADDRSTRLEN);
			if(addressBuffer[0] == '1' && addressBuffer[1] == '9' && addressBuffer[2] == '2')
			{
				return addressBuffer;
			}
		}
		ifAddrStruct = ifAddrStruct->ifa_next;
		
	}
	
	return "\0";	
}


//随机生成验证码
string SendFile::GenerateVerificationCode()
{
	std::uniform_int_distribution<unsigned> u(0,9);
	std::default_random_engine e;
	char tempStr[10];
	string identifyCode;
	for(int i=0;i<5;i++)
	{
		tempStr[i] = u(e)+'0';
	}
	tempStr[5] = '\0';
	identifyCode = tempStr;
	
	return identifyCode;
}


//等待接收方回应
void *SendFile::WaitRecipientReply(void *arg)
{
	char recvBuf[BUFFER_SIZE];
	//sockaddr_in address;
	int ret = 0;
	SendFile *sf = (SendFile *)arg;
	socklen_t len = sizeof(sf->recvAddress);
	
	//接收验证码
	ret = recvfrom(sf->udpfd,recvBuf,BUFFER_SIZE-1,0,(sockaddr *)&sf->recvAddress,&len);
	if(ret < 0)
	{
		cout<<"error"<<endl;
	}
	cout<<inet_ntoa(sf->recvAddress.sin_addr)<<endl;
	recvBuf[ret] = '\0';
	sf->identifyCode = recvBuf;
	status = CONNECT;
	return NULL;
}


//持续发送广播
void SendFile::SendBroadcast(void)
{
	pthread_t thid;
	string randomCode;
	
	//创建线程，用于接收客户端回应
	if(pthread_create(&thid,NULL,WaitRecipientReply,(void *)this) != 0)
	{
		cout<<"线程创建失败"<<endl;
	}

	//获取验证码
	randomCode = GenerateVerificationCode();
	cout<<"验证码为："<<randomCode<<endl;

	while(1)
	{
		usleep(AMOUT);		//没1/10S发一次广播，用于寻找接收方
		
		//构建广播包
		UdpDataPacket noDataPacket;	//非数据包
		bzero(noDataPacket.packetData,BUFFER_SIZE);

		strcpy(noDataPacket.packetData,"hello");
		noDataPacket.packetType = SEND_UDP;
		noDataPacket.packetNumber = 0;
		//发送广播包
		sendto(udpfd,&noDataPacket,sizeof(noDataPacket),0,(sockaddr *)&udpAddress,sizeof(udpAddress));
		//cout<<"发送广播"<<endl;
		if(status == CONNECT)
		{
			cout<<"hello"<<endl;
			if(identifyCode == randomCode)
			{
 	  			cout<<"已确认 连接人   ， 现在开始传输文件"<<endl;
				//给接收端发送验证成功的消息
				
				//构建连接成功包
				bzero(noDataPacket.packetData,BUFFER_SIZE);
				strcpy(noDataPacket.packetData,"\0");
				noDataPacket.packetNumber = 0;
				noDataPacket.packetType = SEND_CORRECT;
				//发送连接成功包
				sendto(udpfd,&noDataPacket,sizeof(noDataPacket),0,(sockaddr *)&recvAddress,sizeof(recvAddress));
				break ;    
			}
			else
			{
	 			 status =  UNCONNECT;  
				//给接收端发送验证不成功的消息
				//构建连接不成功包
				bzero(noDataPacket.packetData,BUFFER_SIZE);
				strcpy(noDataPacket.packetData,"\0");
				noDataPacket.packetNumber = 0;
				noDataPacket.packetType = SEND_UNCORRECT;
				
				//发送连接不成功包
				sendto(udpfd,&noDataPacket,sizeof(noDataPacket),0,(sockaddr *)&recvAddress,sizeof(recvAddress));
				//创建线程继续等待对方连接
 				if(pthread_create(&thid,NULL,WaitRecipientReply,(void *)this) != 0)
				{
					cout<<"线程创建失败"<<endl;
				}
	 			cout<<"world"<<endl;
			}
			
		}
	}
	
}


//将文件名从文件的绝对路径中提取出来
string SendFile::extractFileName(string filename)
{
	int pos = filename.find_last_of('/');

	string s(filename.substr(pos + 1));

	return s;
}


//开始传文件
void SendFile::StartSendFile(char *filename)
{
	//char recvBuf[BUFFER_SIZE];
	int fd;
	int ret;
	
	//发送广播，找到指定用户，与其建立连接
	SendBroadcast();
	//usleep(100);

	//给接收端发送所传文件名
	string fileName = extractFileName(filename);
	sendto(udpfd,fileName.c_str(),sizeof(fileName),0,(sockaddr *)&recvAddress,sizeof(recvAddress));

	//发送文件
	
	if((fd = open(filename,O_RDONLY)) < 0)
	{
		cout<<"文件打开失败";
	}
	cout<<"开始发送"<<endl;
	int i = 0;
	int j = 0;
	int sum = 0;

	//构建数据包
	UdpDataPacket dataPacket;	//数据包
	dataPacket.packetType = SEND_DATA;
	dataPacket.packetNumber = ++i;

	//从文件中读出数据并发给接收端
	while((ret = read(fd,dataPacket.packetData,BUFFER_SIZE-1)) > 0)
	{
		cout<<"包数:"<<++j<<"     "<<"ret"<<ret<<endl;
		++j;
		sum = sum+ret;
		sendto(udpfd,&dataPacket,sizeof(dataPacket),0,(sockaddr *)&recvAddress,sizeof(recvAddress));
		bzero(dataPacket.packetData,BUFFER_SIZE);
		dataPacket.packetNumber = ++i;
	}
	cout<<"sum = "<<sum<<endl;
	cout<<"包总数:"<<j;
	//构建终止包
	bzero(dataPacket.packetData,BUFFER_SIZE);
	strcpy(dataPacket.packetData,"\0");
	dataPacket.packetNumber = 0;
	dataPacket.packetType = SEND_END;	
	
	//发送终止包
	sleep(5);
	sendto(udpfd,&dataPacket,sizeof(dataPacket),0,(sockaddr *)&recvAddress,sizeof(recvAddress));
	close(fd);



}

//开始接收文件
void RecvFile::StartRecvFile(void)
{
	char recvBuf[BUFFER_SIZE];
	int ret;
	socklen_t len = sizeof(sendAddress);
	string identifyCode;
	//string correct = "correct\n";

	cout<<"开始接收数据"<<endl;

	//构建接收数据包
	UdpDataPacket dataPacket;		//接收数据包

	
	//cout<<"dizhi:"<<inet_ntoa(recvAddress.sin_addr)<<endl;
	//cout<<"port1:"<<ntohs(recvAddress.sin_port)<<endl;
	//接收广播与对方建立连接
	ret = recvfrom(recvFd,&dataPacket,sizeof(dataPacket),0,(sockaddr *)&sendAddress,&len);
	cout<<"请输入验证码:";
	//输入验证码
	while(1)
	{
		
		cin>>identifyCode;
		sendto(recvFd,identifyCode.c_str(),identifyCode.size(),0,(sockaddr *)&sendAddress,sizeof(sendAddress));
		//usleep(1000);
		bzero(&dataPacket,sizeof(dataPacket));
		//接收hello,world
		//将滞留在缓冲中的广播数据循环读取完
		do
		{	bzero(&dataPacket,sizeof(dataPacket));
			ret = recvfrom(recvFd,&dataPacket,sizeof(dataPacket),0,(sockaddr *)&sendAddress,&len);
			if(ret <= 0)
			{
				cout<<"error"<<endl;
			}
			dataPacket.packetData[ret] = '\0';
			
		}while(dataPacket.packetData[0] == 'h');
		
		if(dataPacket.packetType == SEND_CORRECT)
		{
			cout<<"连接建立成功"<<endl;
			break;
		}
		cout<<"您的输入有误，请重新输入:"<<endl;

	}
	//获取要接收的文件名
	bzero(&recvBuf,sizeof(recvBuf));
	ret = recvfrom(recvFd,recvBuf,BUFFER_SIZE-1,0,(sockaddr *)&sendAddress,&len);
	recvBuf[ret] = '\0';
	string filename = recvBuf;
	char fileName[40] = "/tmp/";
	strcat(fileName,filename.c_str());
	
	cout<<inet_ntoa(sendAddress.sin_addr)<<endl;

	
	//设置接收缓冲区
	int on = 1024*1024*5;
	unsigned length = sizeof(on);
	if(setsockopt(recvFd,SOL_SOCKET,SO_RCVBUF,&on,len) == -1)
	{
		cout<<"error"<<endl;
	}
	
	int fd;
	
	//打开并创建与上面收到文件名一样的文件
	if((fd = open(fileName,O_WRONLY | O_CREAT | O_EXCL,S_IRUSR,S_IWUSR,S_IXUSR)) < 0)
	{
		cout<<"文件打开失败"<<endl;
	}
	
	bzero(&recvBuf,sizeof(recvBuf));
	cout<<"开始写入文件"<<endl;
	int i=0;
	int j=0;
	int sum = 0;
	bzero(&dataPacket,sizeof(dataPacket));
	//循环读取套接字中的数据，并写入文件
	while((ret = recvfrom(recvFd,&dataPacket,sizeof(dataPacket),0,(sockaddr *)&sendAddress,&len)) > 0)
	{	
		sum = sum+ret;
		j++;
		cout<<"包数:"<<++j<<"     "<<"ret"<<ret<<"     "<<"包号:"<<dataPacket.packetNumber<<endl;
		//判断数据包是否是终止包，若是则退出循环
		if(dataPacket.packetType == SEND_END)
		{
			cout<<"文件接收成功"<<endl;
			break;
		}
		
		//如果是数据包，则将其写入文件，或存入recvDataSet
		if(dataPacket.packetType == SEND_DATA)
		{
			if(dataPacket.packetNumber == (i+1))						//如果序号为上一个写入文件的序号加1则写入文件
			{
				ret =  write(fd,dataPacket.packetData,BUFFER_SIZE-1);
			
				if(ret < 0)
				{	
					cout<<"写入文件失败"<<endl;
				}
	 			i++;
			}
	
			else if(!recvDataSet.empty())		//如果recvDataSet非空
			{
				for( auto it:recvDataSet) 		//循环判断其中的元素序号是否是上一个写入文件序号加一
				{
				  	if(it.packetNumber == (i +1))	//若是写入文件
					{
				 	 	ret = write(fd,dataPacket.packetData,BUFFER_SIZE-1);
						if(ret < 0)
						{
							cout<<"写入文件失败"<<endl;
						}
						i++;
						//将recvDataSet中的此数据包删除
						recvDataSet.erase(it);
	 					break;
					}
					//不是则退出循环
					else
					{
						break;
	 				}
	 			}
			}
			//不是则插入recvDataSet中
			else
			{
				recvDataSet.insert(dataPacket);
			}
			
		}
		bzero(&dataPacket,sizeof(dataPacket));
	}
	cout<<"sum = "<<sum<<endl;
	cout<<"包总数:"<<j<<endl;
	close(fd);
	close(recvFd);


}

bool ComparePacketNumber(const UdpDataPacket &pack1,const UdpDataPacket &pack2)
{
	return pack1.packetNumber < pack2.packetNumber;
}

