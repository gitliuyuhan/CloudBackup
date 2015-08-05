/*======================================================
    > File Name: applydialog.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月04日 星期二 01时03分39秒
 =======================================================*/

#ifndef APPLYDIALOG_H
#define APPLYDIALOG_H

#include<QDialog>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>


class ApplyDialog : public QDialog
{
    Q_OBJECT
public:
    ApplyDialog(QDialog* parent=0);
    ~ApplyDialog(){}
signals:
    //注册成功
    void ApplyOk();
private slots:
    //注册
    void Applying();
private:
    QLabel*        nameLabel;
    QLineEdit*     nameLineEdit;
    QLabel*        namewarnLabel;
    QLabel*        passwdLabel1;
    QLabel*        passwdLabel2;
    QLabel*        pwdwarnLabel;
    QLineEdit*     pwdLineEdit1;
    QLineEdit*     pwdLineEdit2;
    QLabel*        mailLabel;
    QLineEdit*     mailLineEdit;
    QLabel*        mailwarnLabel;
    QLabel*        titleLabel;
    QPushButton*   applyButton;
};

#endif//APPLYDIALOG_H
