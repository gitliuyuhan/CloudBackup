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
#include<QMessageBox>
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
    //关闭对话框
    this->close();
    extern int   sockFd;
    //Json格式
    Json::Value     json;
    json["status"] = Json::Value(8);
    json["File"] = Json::Value(file.toStdString());
    json["Path"] = Json::Value(filePath.toStdString());
    json["SavePath"] = Json::Value((saveLineEdit->text()).toStdString());

    //Json串
    string     jsonBuf = json.toStyledString();

    //给主服务器发送请求
    int  ret = send(sockFd,jsonBuf.c_str(),strlen(jsonBuf.c_str()),0);
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
