/*======================================================
    > File Name: loginwidget.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月02日 星期日 09时08分13秒
 =======================================================*/

#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include"applydialog.h"

#include<QWidget>
#include<QLabel>
#include<QPushButton>
#include<QToolButton>
#include<QCheckBox>
#include<QComboBox>
#include<QLineEdit>
#include<QString>

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    LoginWidget(QWidget* parent=0);
    ~LoginWidget(){}
signals:
    //登录成功
    void LoginOk(QString,QString,int,QString,QString); 
    //关闭窗口   
    void LoginClose();

public slots:
    void CloseWidget(QString,QString,int,QString,QString);

protected:
    virtual void closeEvent(QCloseEvent*);    
private slots:
    //隐藏窗口
    void HideWidget();
    //开始登录
    void Loginning();
    //设置服务器信息
    void SetServDialog();
    //保存修改后的服务器信息
    void SaveSetServ();
    //注册
    void ShowApplyDialog();
private:
    QLabel*         warnLabel;
    QLabel*         nameLabel;
    QLabel*         passwdLabel;
    QComboBox*      nameComboBox;
    QLineEdit*      passwdLineEdit;
    QPushButton*    applyPushButton;
    QPushButton*    modpwdPushButton;
    QCheckBox*      rempwdCheckBox;
    QCheckBox*      autolgCheckBox;
    QPushButton*    loginPushButton;
    QPushButton*    hidePushButton;
    QToolButton*    settingButton;

    ApplyDialog*    applyDialog;
    QDialog*        setDialog;

    //IP编辑框
    QLineEdit*      ipLineEdit;
    //端口编辑框
    QLineEdit*      portLineEdit;  

public:
    //服务器IP
    QString         servIp; 
    //服务器端口  
    QString         servPort;
    //连接套接字
    int             sockfd;
};

#endif//LOGINWIDGET_H
