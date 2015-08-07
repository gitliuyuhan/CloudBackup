/*======================================================
    > File Name: ApplyDialog.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月04日 星期二 08时01分21秒
 =======================================================*/

#include"applydialog.h"
#include"connectserv.h"

#include<QGridLayout>
#include<QVBoxLayout>

#include<json/json.h>
#include<iostream>
#include<string>

#define     BUFSIZE    1024

using namespace std;

ApplyDialog::ApplyDialog(QDialog* parent):QDialog(parent)
{
    warnLabel = new QLabel;
    warnLabel->setStyleSheet("font-size: 14px;");

    titleLabel = new QLabel(tr("新用户注册"));
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 17px;");
    nameLabel = new QLabel(tr("输入帐号："));
    nameLineEdit = new QLineEdit;
    nameLineEdit->setStyleSheet("min-width:200;");
    namewarnLabel = new QLabel;
    passwdLabel1 = new QLabel(tr("输入密码："));
    passwdLabel2 = new QLabel(tr("再次输入密码："));
    pwdLineEdit1 = new QLineEdit;
    pwdLineEdit1->setEchoMode(QLineEdit::Password);
    pwdLineEdit2 = new QLineEdit;
    pwdLineEdit2->setEchoMode(QLineEdit::Password);
    pwdwarnLabel = new QLabel;
    
    mailLabel = new QLabel(tr("输入邮箱："));
    mailLineEdit = new QLineEdit;
    mailwarnLabel = new QLabel;
    applyButton = new QPushButton(tr("提交"));
    applyButton->setStyleSheet("max-width:150;background: #CAFF70;");

    QGridLayout*  editLayout = new QGridLayout;
    editLayout->addWidget(nameLabel,0,0,1,2);
    editLayout->addWidget(nameLineEdit,0,2,1,8);
    editLayout->addWidget(namewarnLabel,0,10);
    editLayout->addWidget(passwdLabel1,1,0,1,2);
    editLayout->addWidget(pwdLineEdit1,1,2,1,8);
    editLayout->addWidget(passwdLabel2,2,0,1,2);
    editLayout->addWidget(pwdLineEdit2,2,2,1,8);
    editLayout->addWidget(pwdwarnLabel,2,10);
    editLayout->addWidget(mailLabel,3,0,1,2);
    editLayout->addWidget(mailLineEdit,3,2,1,8);
    editLayout->addWidget(mailwarnLabel,3,10);

    QHBoxLayout* applyLayout = new QHBoxLayout;
    applyLayout->addStretch();
    applyLayout->addWidget(applyButton);
    applyLayout->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(editLayout);
    mainLayout->addWidget(warnLabel);
    mainLayout->addLayout(applyLayout);

    this->setLayout(mainLayout);

    connect(applyButton,SIGNAL(clicked()),this,SLOT(Applying()));
}

void ApplyDialog::Applying()
{
    string    strname,strpwd,strmail,strjson;
    QString   qjson;
    QString   qname,qpwd1,qpwd2,qmail;
    QString   hook(tr("√"));
    qname = nameLineEdit->text();
    if(qname.isEmpty())
    {
        namewarnLabel->setStyleSheet("color: red;font-size: 14px;");
        namewarnLabel->setText(tr("帐号不能为空"));
        return;
    }
    else
    {
        namewarnLabel->clear();
    }
    strname = qname.toStdString();

    qpwd1 = pwdLineEdit1->text();
    qpwd2 = pwdLineEdit2->text();
    if(qpwd1 != qpwd2)
    {
        pwdwarnLabel->setStyleSheet("color: red;font-size: 14px;");
        pwdwarnLabel->setText(tr("两次密码输入不一致"));
        pwdLineEdit1->clear();
        pwdLineEdit2->clear();
        return;
    }
    else if(qpwd1.isEmpty()||qpwd2.isEmpty())
    {
        pwdwarnLabel->setStyleSheet("color: red;font-size: 14px;");
        pwdwarnLabel->setText(tr("密码不能为空"));
        return;
    }
    else
    {
        pwdwarnLabel->setStyleSheet("color: green;font-size: 14px;");
        pwdwarnLabel->setText(hook);
    }
    strpwd = qpwd1.toStdString();

    qmail = mailLineEdit->text();
    if(qmail.isEmpty())
    {
        mailwarnLabel->setStyleSheet("color: red;font-size: 14px;");
        mailwarnLabel->setText(tr("邮箱不能为空"));
        return;
    }
    else
    {
        mailwarnLabel->clear();
    }
    strmail = qmail.toStdString();

    Json::Value   json;
    char          buf[BUFSIZE];
    string        strbuf;
    json["status"] = Json::Value(2);
    json["UserName"] = Json::Value(strname);
    json["Passwd"] = Json::Value(strpwd);
    json["Email"] = Json::Value(strmail);

    strbuf = json.toStyledString();

    warnLabel->setStyleSheet("color: green;");
    warnLabel->setText(tr("正在提交注册信息。。。"));
    
    if(sockfd==-1)
    {
        int ret = ConnectServer(qIp,qPort);
        if(ret<0)
        {
            warnLabel->setStyleSheet("color: red");
            warnLabel->setText(tr("未连接到服务器！"));
            sockfd = -1;
            return;
        }
        if(ret>0)
        {
            sockfd  = ret;
        }
    }
    int ret = send(sockfd,strbuf.c_str(),strlen(strbuf.c_str()),0);
    
    if(ret<0)
    {
        warnLabel->setStyleSheet("color: red;");
        warnLabel->setText(tr("提交信息失败"));
    }
    
    ret = recv(sockfd,buf,BUFSIZE,0);
    if(ret==0)
    {
        warnLabel->setText(tr("服务器关闭"));
        warnLabel->setStyleSheet("color: red");
        return;
    }
    else if(ret<0)
    {
        warnLabel->setText(tr("注册异常"));
        warnLabel->setStyleSheet("color: red;");
        return;
    }
    else
    {
        buf[strlen(buf)] = '\0';
        strjson = buf;
        Json::Reader       reader;
        reader.parse(strjson,json);
        int  status = json["status"].asInt();
        switch(status)
        {
            case 0:
                qjson = QString::fromStdString(strjson);
                emit ApplyOk(qIp,qPort,sockfd,qname,qjson);
                close();
                break;
            case 1:
                namewarnLabel->setText(tr("帐号已存在"));
                namewarnLabel->setStyleSheet("color: red;");
                break;
            case 2:
                namewarnLabel->setText(hook);
                namewarnLabel->setStyleSheet("color: green;");
                mailwarnLabel->setText(tr("邮箱已存在"));
                mailwarnLabel->setStyleSheet("color: red;");
                break;
        }
    }
    
    return;
}
