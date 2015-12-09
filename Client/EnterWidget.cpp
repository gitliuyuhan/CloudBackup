/*======================================================
    > File Name: EnterWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月02日 星期日 20时59分41秒
 =======================================================*/

#include"enterwidget.h"

#include<QVBoxLayout>
#include<QMessageBox>
#include<QDebug>

EnterWidget::EnterWidget(QWidget* parent):QWidget(parent)
{
    QFont  font;
    font.setPointSize(15);
    font.setBold(true);

    loginButton = new QPushButton(tr("登录"));
    loginButton->setStyleSheet("max-width: 150;min-width: 150;color: white;");
//    loginButton->setFlat(true);
    loginButton->setFont(font);
    loginButton->setFocusPolicy(Qt::NoFocus);
    connect(loginButton,SIGNAL(clicked()),this,SLOT(ShowLoginWidget()));

    sendFileButton = new QPushButton(tr("发送文件"));
    sendFileButton->setFont(font);
    loginButton->setFocusPolicy(Qt::NoFocus);
    sendFileButton->setStyleSheet("max-width: 150;min-width: 150;color: white;");
    recvFileButton = new QPushButton(tr("接收文件"));
    recvFileButton->setFont(font);
    loginButton->setFocusPolicy(Qt::NoFocus);
    recvFileButton->setStyleSheet("max-width: 150;min-width: 150;color: white;");

    menuWidget = new QWidget;
    loginWidget = new LoginWidget;
    loginWidget->setMaximumSize(QSize(300,250));
    loginWidget->setHidden(true);

    mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->setHandleWidth(1);

    QVBoxLayout*  centerLayout = new QVBoxLayout;
    centerLayout->addWidget(loginButton);
    centerLayout->addWidget(sendFileButton);
    centerLayout->addWidget(recvFileButton);

    QHBoxLayout*  mainLayout = new QHBoxLayout;
    mainLayout->addStretch();
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();
    menuWidget->setLayout(mainLayout);
    menuWidget->setStyleSheet("background-image:url(:/image/1.jpg);");

    mainSplitter->addWidget(menuWidget);
    mainSplitter->addWidget(loginWidget);
    mainSplitter->setStretchFactor(0,1);
    mainSplitter->setStretchFactor(1,1);
    mainSplitter->setMaximumSize(QSize(480,250));
    mainSplitter->setMinimumSize(QSize(480,250));
  
    QPalette palette;
//    palette.setBrush(QPalette::Background,QBrush(QColor(135,206,255)));  
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/image/2.png")));
    loginWidget->setPalette(palette);

//    loginWidget->setStyleSheet("background-image:url(:/image/2.jpg);"); 
    loginWidget->setAutoFillBackground(true); 
    mainSplitter->show();

    connect(loginWidget,SIGNAL(LoginClose()),mainSplitter,SLOT(close()));
}

/* 显示登录窗口 */
void EnterWidget::ShowLoginWidget()
{
    qDebug("show");
    loginWidget->setHidden(false);
}

