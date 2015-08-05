/*======================================================
    > File Name: ApplyDialog.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月04日 星期二 08时01分21秒
 =======================================================*/

#include"applydialog.h"

#include<QGridLayout>
#include<QVBoxLayout>

ApplyDialog::ApplyDialog(QDialog* parent):QDialog(parent)
{
    titleLabel = new QLabel(tr("新用户注册"));
    titleLabel->setAlignment(Qt::AlignCenter);
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
    pwdwarnLabel = new QLabel(tr("两次密码输入不一致"));
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
    mainLayout->addLayout(applyLayout);

    this->setLayout(mainLayout);

    connect(applyButton,SIGNAL(clicked()),this,SLOT(Applying()));
}

void ApplyDialog::Applying()
{
    //注册成功
    emit ApplyOk();
    close();
}
