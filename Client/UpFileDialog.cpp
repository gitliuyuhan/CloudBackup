/*======================================================
    > File Name: UpFileDialog.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月28日 星期二 21时09分41秒
 =======================================================*/

#include"upfiledialog.h"
#include"connectserv.h"

#include<QHBoxLayout>
#include<QDebug>
#include<QLabel>
#include<QMessageBox>
#include<json/json.h>

UpFileDialog::UpFileDialog(QFileDialog* parent) : QFileDialog(parent)
{
    //设置当前目录
    this->setDirectory(".");
    //文件过滤器
    this->setFilter("*");

    connect(this,SIGNAL(currentChanged(const QString&)),this,SLOT(GetSelectedPath(const QString&)));
    connect(this,SIGNAL(accepted()),this,SLOT(UpLoadFile()));
}

void UpFileDialog::UpLoadFile()
{
    extern   int  sockFd;
    cout<<sockFd<<endl;
    Json::Value   json;
    json["status"] = Json::Value(7);
    json["File"] = Json::Value(currentPath.toStdString());
    string        jsonBuf = json.toStyledString();
    int ret = send(sockFd,jsonBuf.c_str(),strlen(jsonBuf.c_str()),0);
    if(ret<0)
    {
        QMessageBox  messageBox;
        messageBox.setText(tr("请求失败！"));
        cout<<"请求失败\n";
        messageBox.show();
        return;
    }
    cout<<"已发送请求\n";
}

void UpFileDialog::GetSelectedPath(const QString &str)
{
    currentPath = str;
}
