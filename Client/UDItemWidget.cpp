/*======================================================
    > File Name: UDItemWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月13日 星期四 15时23分34秒
 =======================================================*/

#include"uditemwidget.h"
#include"myinclude.h"

#include<QHBoxLayout>

UDItemWidget::UDItemWidget(int ty,QWidget* parent):QWidget(parent)
{
    type = ty;
    QHBoxLayout*     itemLayout = new QHBoxLayout;

    stateLabel = new QLabel(tr("正在上传"));
    if(type==1)
    {
        stateLabel->setText(tr("正在下载"));
    }
    stateLabel->setStyleSheet("max-width:80;");
    stateLabel->setAlignment(Qt::AlignCenter);

    fileLabel = new QLabel;

    proBar = new QProgressBar;
    proBar->setValue(0);

    stopButton = new QPushButton(tr("停止"));
    stopButton->setIcon(QIcon(":/image/stop.png"));
    stopButton->setFlat(true);
    stopButton->setStyleSheet("max-width:70");

    delButton = new QPushButton(tr("删除"));
    delButton->setIcon(QIcon(":/image/remove.png"));
    delButton->setFlat(true);
    delButton->setStyleSheet("max-width:70");

    itemLayout->addWidget(stateLabel);
    itemLayout->addWidget(fileLabel);
    itemLayout->addWidget(proBar);
    itemLayout->addWidget(stopButton);
    itemLayout->addWidget(delButton);
    this->setLayout(itemLayout);

    connect(this,SIGNAL(setStateSig(QString)),this,SLOT(setState(QString)));
    connect(this,SIGNAL(ProBarRangeSig(int,int)),this,SLOT(SetProBarRange(int,int)));
    connect(this,SIGNAL(ProBarValueSig(int)),this,SLOT(SetProBarValue(int)));
}

//设置传输的文件内容
void UDItemWidget::setFileLabel(QString str)
{
    fileLabel->setText(str);
}

//发射状态信号
void UDItemWidget::EmitStateSig(QString s)
{
    emit setStateSig(s);
}

//设置状态
void UDItemWidget::setState(QString s)
{
    stateLabel->setText(s);
}

//发射进度条范围信号
void UDItemWidget::EmitProBarRange(int start,int sum)
{
    emit ProBarRangeSig(start,sum);
}

//发射进度值信号
void UDItemWidget::EmitProBarValue(int value)
{
    emit ProBarValueSig(value);
}

//设置进度条范围
void UDItemWidget::SetProBarRange(int start,int sum)
{
    proBar->setRange(start,sum);
}

//设置进度条值
void UDItemWidget::SetProBarValue(int value)
{
    proBar->setValue(value);
}
