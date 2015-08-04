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

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    LoginWidget(QWidget* parent=0);
    ~LoginWidget(){}
signals:
    void LoginOk();    

protected:
    virtual void closeEvent(QCloseEvent*);    
private slots:
    //隐藏窗口
    void HideWidget();
    //开始登录
    void Loginning();
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
};

#endif//LOGINWIDGET_H
