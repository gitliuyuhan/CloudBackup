/*======================================================
    > File Name: DownFileDialog.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月28日 星期二 10时01分56秒
 =======================================================*/

#include"downfiledialog.h"
#include"connectserv.h"

#include<QVBoxLayout>
#include<QHBoxLayout>
#include<json/json.h>

/* 构造函数 */
DownFileDialog::DownFileDialog(QDialog*  parent):QDialog(parent)
{
    //文件浏览对话框
    fileDialog = new QFileDialog;

    //只显示目录
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
 
    saveLabel = new QLabel(tr("路径："));
    saveLineEdit = new QLineEdit;
    saveButton = new QPushButton(tr("保存"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(DownLoadFile()));

    connect(fileDialog,SIGNAL(currentChanged(const QString &)),this,SLOT(LinePathShow(const QString &)));

    QHBoxLayout*  baseLayout = new QHBoxLayout;
    baseLayout->addWidget(saveLabel);
    baseLayout->addWidget(saveLineEdit);
    baseLayout->addWidget(saveButton);

    QVBoxLayout*  mainLayout = new QVBoxLayout;
    mainLayout->addWidget(fileDialog);
    mainLayout->addLayout(baseLayout);
    this->setLayout(mainLayout);

    connect(fileDialog,SIGNAL(accepted()),this,SLOT(close()));
    connect(fileDialog,SIGNAL(rejected()),this,SLOT(close()));
}

/* 显示选择的路径，槽函数*/
void DownFileDialog::LinePathShow(const QString &path)
{
    saveLineEdit->setText(path);
}

/* 下载文件 */
void DownFileDialog::DownLoadFile()
{
    QString     qpath = saveLineEdit->text();
    QString     filename = fileDialog->selectedFiles()[0];
    string      strname =filename.toStdString();
    
//    string      strPath = qpath.toStdString();
    string     strPath = "/home/lyh/lyh";
    this->close();
    QString     workIp,workPort;
    string      ip,port;
    cout<<"Ip:";
    cin>>ip;
    workIp = QString::fromStdString(ip);
    cout<<"port";
    cin>>port;
    workPort = QString::fromStdString(port);

 
    QLabel       *warnLabel = new QLabel(tr("正在下载。。。"));
    QDialog      *warnDialog = new QDialog;
    QHBoxLayout  *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(warnLabel);
    warnDialog->setLayout(mainLayout);    
    warnLabel->setAlignment(Qt::AlignCenter);
    warnDialog->resize(300,200);
    warnDialog->show();


    int  sockFd;
    int  ret = ConnectServer(workIp,workPort);
    if(ret<0)
    {
        warnLabel->setText(tr("下载失败"));
        return;
    }
    sockFd = ret;
    int      len=160000;
    setsockopt(sockFd,SOL_SOCKET,SO_RCVBUF,&len,sizeof(int));

    //下载md5值对应的文件
    string           fileMd5;  //下载文件的Md5
    cout<<"要下载文件的Md5值:";
    cin>>fileMd5;
    Json::Value           json;
    json["mark"] = Json::Value(1);
    json["md5"] = Json::Value(fileMd5);
    string      namebuf;
    namebuf = (json.toStyledString()).c_str();

    //发送md5文件名
    if(send(sockFd,namebuf.c_str(),strlen(namebuf.c_str()),0)<0)
    {
        warnLabel->setText(tr("上传失败"));
        return;
    }

    warnLabel->setText(tr("正在下载。。。"));
    int     fileFd;
    cout<<"Path:"<<strPath<<endl;
    if((fileFd = openfile(strPath.c_str())) < 0)
    {
        warnLabel->setText(tr("文件保存失败"));
        return;
    }
    cout<<"fileFd = "<<fileFd<<endl;

    //使用splice拷贝文件
    int pipeFd[2];

    //创建管道
    ret = pipe(pipeFd);
    if(ret < 0)
    {
        warnLabel->setText(tr("下载失败"));
    }

    int sum = 0;

    off_t lenth = 0;
    //从发送端套接字将文件读入到管道
    while(1)
    {

        ret = splice(sockFd,NULL,pipeFd[1],NULL,50000,SPLICE_F_MOVE);
        sum = sum + ret;

        //cout<<"ret11 = "<<ret<<endl;
        //从管道读出内容写入文件
        ret = splice(pipeFd[0],NULL,fileFd,&lenth,50000,SPLICE_F_MOVE);
        lenth  = lenth + ret;

        cout<<"ret22 = "<<ret<<endl;
        if(ret <= 0)
        {
            break;
        }
    }


    if(ret == -1)
    {
        warnLabel->setText(tr("下载出错"));
    }

    cout<<"sum = "<<sum<<endl;
    warnLabel->setText(tr("下载完成"));
}
