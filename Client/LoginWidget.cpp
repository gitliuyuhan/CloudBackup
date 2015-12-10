/*======================================================
    > File Name: LoginWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月02日 星期日 10时55分36秒
 =======================================================*/

#include"loginwidget.h"
#include"connectserv.h"

#include<QGridLayout>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QDebug>

#include<iostream>
#include<string>
#include<fstream>
#include<json/json.h>

#include<sys/socket.h>
#include<sys/types.h>

#define      BUFSIZE   1024

using  namespace std;

LoginWidget :: LoginWidget(QWidget* parent):QWidget(parent)
{
    QFont    font;
    font.setPointSize(10);
    warnLabel = new QLabel;
    warnLabel->setFont(font);
    warnLabel->setStyleSheet("max-height: 15;min-height: 15;color: red;");
    nameLabel = new QLabel(tr("帐号："));
   
    passwdLabel = new QLabel(tr("密码："));
    nameComboBox = new QComboBox;
    nameComboBox->setEditable(true);
    nameComboBox->setStyleSheet("min-height: 27;");

    passwdLineEdit = new QLineEdit;
    passwdLineEdit->setStyleSheet("min-height: 27;");
    passwdLineEdit->setEchoMode(QLineEdit::Password);
    applyPushButton = new QPushButton(tr("注册帐号"));
    applyPushButton->setStyleSheet("max-width: 70;");
    applyPushButton->setFlat(true);
    modpwdPushButton = new QPushButton(tr("忘记密码"));
    modpwdPushButton->setStyleSheet("max-width: 70;");
    modpwdPushButton->setFlat(true);
    rempwdCheckBox = new QCheckBox(tr("记住密码"));
    autolgCheckBox = new QCheckBox(tr("自动登录"));
    loginPushButton = new QPushButton(tr("登录"));
    
    font.setPointSize(14);
    font.setBold(true);
    loginPushButton->setFont(font);
    loginPushButton->setFlat(false);
    loginPushButton->setStyleSheet("background: #1E90FF;color:white;");
    connect(loginPushButton,SIGNAL(clicked()),this,SLOT(Loginning()));

    hidePushButton = new QPushButton(this);
    hidePushButton->setFlat(true);
    //透明
    hidePushButton->setStyleSheet("background: transparent");
    hidePushButton->setIcon(QIcon(":/image/hide.png"));
    connect(hidePushButton,SIGNAL(clicked()),this,SLOT(HideWidget()));

    settingButton = new QToolButton(this);
    settingButton->setIcon(QIcon(":/image/set.png"));
    settingButton->setStyleSheet("border-style: none;");

    applyDialog = new ApplyDialog;
    connect(applyPushButton,SIGNAL(clicked()),this,SLOT(ShowApplyDialog()));
    connect(applyDialog,SIGNAL(ApplyOk(QString,QString,int,QString,QString)),this,SLOT(CloseWidget(QString,QString,int,QString,QString)));
    
    QGridLayout  *leftLayout = new QGridLayout;
    leftLayout->addWidget(warnLabel,0,1,1,4);
    leftLayout->addWidget(nameLabel,1,0);
    leftLayout->addWidget(nameComboBox,1,1,1,4);
    leftLayout->addWidget(applyPushButton,1,5,1,2);
    leftLayout->addWidget(passwdLabel,2,0);
    leftLayout->addWidget(passwdLineEdit,2,1,1,4);
    leftLayout->addWidget(modpwdPushButton,2,5,1,2);
    leftLayout->addWidget(rempwdCheckBox,3,0,1,2);
    leftLayout->addWidget(autolgCheckBox,3,3,1,2);
//    leftLayout->addWidget(loginPushButton,4,0,1,6);

    QVBoxLayout *mainLayout = new QVBoxLayout;
 //   mainLayout->addStretch();
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(loginPushButton);

    hidePushButton->setGeometry(288,105,15,40);
    hidePushButton->setIconSize(QSize(15,40));
    settingButton->setGeometry(275,0,25,25);
    settingButton->setIconSize(QSize(25,25));
    connect(settingButton,SIGNAL(clicked()),this,SLOT(SetServDialog()));
//    settingButton->show();
    setLayout(mainLayout);


//获取服务器IP和端口
    ifstream   filein("./etc/service.txt");
    string     ip,port;
    filein>>ip>>port;
    filein.close();
    servIp = QString::fromStdString(ip);
    servPort = QString::fromStdString(port);
    sockfd = -1;
}

/* 隐藏窗口 */
void LoginWidget::HideWidget()
{
    this->setHidden(true);
}

/* 开始登录，槽函数 */
void LoginWidget::Loginning()
{
    string  jsonStr;

    qDebug()<<servIp<<servPort;

    QString     qstr;
    string      name,passwd;

    qstr = nameComboBox->currentText();
    if(qstr.isEmpty())
    {
        warnLabel->setText(tr("帐号不能为空"));
        return;
    }
    name = qstr.toStdString();
    qstr = passwdLineEdit->text();
    if(qstr.isEmpty())
    {
        warnLabel->setText(tr("密码不能为空"));
        return;
    }
    passwd = qstr.toStdString();

    Json::Value   json;
    json["status"] = Json::Value(1);
    json["UserName"] = Json::Value(name);
    json["Passwd"] = Json::Value(passwd);

    char   buffer[BUFSIZE];
    string buf = json.toStyledString();

    warnLabel->clear();
    warnLabel->setText(tr("正在登陆。。。"));

    if(sockfd == -1 )
    {
        int ret = ConnectServer(servIp,servPort);
        if(ret<0)
        {
            warnLabel->setText(tr("未连接到服务器！"));
            sockfd = -1;
            return;
        }
        if(ret >0 )
        {
            sockfd = ret;
        }
    }

    int ret = send(sockfd,buf.c_str(),strlen(buf.c_str()),0);
    
    if(ret<0)
    {
        warnLabel->setText(tr("登陆失败"));
        return;
    }
    warnLabel->setText(tr("正在登陆。。。"));
    ret = recv(sockfd,buffer,BUFSIZE,0);
    if(ret==0)
    {
        warnLabel->setText(tr("服务器关闭"));
        return;
    }
    else if(ret<0)
    {
        warnLabel->setText(tr("登陆异常"));
        return;
    }
    else
    {
        buffer[strlen(buffer)] = '\0';
        jsonStr = buffer;
        Json::Reader   reader;
        reader.parse(jsonStr,json);
        int  status = json["status"].asInt();
        QString    strjson;
        cout<<buffer<<endl;
        switch(status)
        {
            case 0:
                cout<<"0:"<<status<<endl;
                strjson = QString::fromStdString(jsonStr);
                emit LoginOk(servIp,servPort,sockfd,QString::fromStdString(name),strjson);
                emit LoginClose();
                break;
            case 1:
                cout<<"1:"<<status<<endl;
                warnLabel->setText(tr("帐号不存在"));
                break;
            case 2:
                warnLabel->setText(tr("密码错误"));
                break;
        }
        return;
    }
}

/* 关闭窗口时发信号 */
void LoginWidget::closeEvent(QCloseEvent*   event)
{
    emit LoginClose();
    if(sockfd != -1)
        closefd(sockfd);
}

/* 配置服务器信息 */
void LoginWidget::SetServDialog()
{
    QLabel*       ipLabel = new QLabel(tr("IP："));
    this->ipLineEdit = new QLineEdit;
    ipLineEdit->setText(servIp);
    QLabel*       portLabel = new QLabel(tr("端口："));
    this->portLineEdit = new QLineEdit;
    portLineEdit->setText(servPort);
    portLineEdit->setStyleSheet("max-width: 60;");
    QPushButton*  okButton = new QPushButton(tr("保存"));
    QPushButton*  cancelButton = new QPushButton(tr("取消"));

    this->setDialog = new QDialog;
    QGridLayout*  mainLayout = new QGridLayout;

    mainLayout->addWidget(ipLabel,0,0,1,1);
    mainLayout->addWidget(ipLineEdit,0,1,1,5);
    mainLayout->addWidget(portLabel,0,6,1,1);
    mainLayout->addWidget(portLineEdit,0,7,1,2);
    mainLayout->addWidget(okButton,2,1,1,2);
    mainLayout->addWidget(cancelButton,2,6,1,2);
    setDialog->setLayout(mainLayout);
    setDialog->show();

    connect(cancelButton,SIGNAL(clicked()),setDialog,SLOT(close()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(SaveSetServ()));
}

/* 保存修改后的服务器信息 */
void LoginWidget::SaveSetServ()
{
    servIp = ipLineEdit->text();
    servPort = portLineEdit->text();
    string   strip,strport;
    strip = servIp.toStdString();
    strport = servPort.toStdString();

    ofstream  fileout("./etc/service.txt");
    fileout<<strip<<" "<<strport;
    fileout.close();
    setDialog->close();
    closefd(sockfd);
    sockfd = ConnectServer(servIp,servPort);
}

void LoginWidget::ShowApplyDialog()
{
    applyDialog->qIp = servIp;
    applyDialog->qPort = servPort;
    applyDialog->sockfd = sockfd;
    applyDialog->show();
}

/* 注册成功后关闭窗口 */
void LoginWidget::CloseWidget(QString ip,QString port,int sock,QString name,QString sjson)
{
    emit LoginOk(ip,port,sock,name,sjson);
    close();
}
