/*======================================================
    > File Name: LoginWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月02日 星期日 10时55分36秒
 =======================================================*/

#include"loginwidget.h"

#include<QGridLayout>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QDebug>

LoginWidget :: LoginWidget(QWidget* parent):QWidget(parent)
{
    QFont    font;
    font.setPointSize(10);
    warnLabel = new QLabel(tr("帐号不存在"));
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
    applyPushButton->setStyleSheet("max-width: 58;");
    applyPushButton->setFlat(true);
    modpwdPushButton = new QPushButton(tr("忘记密码"));
    modpwdPushButton->setStyleSheet("max-width: 58;");
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
    connect(applyPushButton,SIGNAL(clicked()),applyDialog,SLOT(show()));
    connect(applyDialog,SIGNAL(ApplyOk()),this,SLOT(close()));

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
//    settingButton->show();
    setLayout(mainLayout);
    
}

/* 隐藏窗口 */
void LoginWidget::HideWidget()
{
    this->setHidden(true);
}

/* 开始登录，槽函数 */
void LoginWidget::Loginning()
{
    qDebug("ssss");
    emit LoginOk();
}

/* 关闭窗口时发信号 */
void LoginWidget::closeEvent(QCloseEvent*   event)
{
    emit LoginOk();
}
