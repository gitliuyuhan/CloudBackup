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

    fileLabel = new QLineEdit;
    fileLabel->setFrame(false);
    fileLabel->setReadOnly(true);
    fileLabel->setAlignment(Qt::AlignCenter);

    proBar = new QProgressBar;
    proBar->setValue(0);
    proBar->setStyleSheet("max-width:180;min-width:180");

    rateLabel = new QLabel;
    rateLabel->setStyleSheet("max-width:120;min-width:120");

    stopButton = new QPushButton(tr("停止"));
    stopButton->setIcon(QIcon(":/image/stop.png"));
    stopButton->setFlat(true);
    stopButton->setStyleSheet("max-width:60");

    delButton = new QPushButton(tr("删除"));
    delButton->setIcon(QIcon(":/image/remove.png"));
    delButton->setFlat(true);
    delButton->setStyleSheet("max-width:60");

    itemLayout->addWidget(stateLabel);
    itemLayout->addWidget(fileLabel);
    itemLayout->addWidget(proBar);
    itemLayout->addWidget(rateLabel);
    itemLayout->addWidget(stopButton);
    itemLayout->addWidget(delButton);
    this->setLayout(itemLayout);

    connect(this,SIGNAL(setStateSig(QString)),this,SLOT(setState(QString)));
    connect(this,SIGNAL(ProBarRangeSig(int,float,int)),this,SLOT(SetProBarRange(int,float,int)));
    connect(this,SIGNAL(ProBarValueSig(float,float)),this,SLOT(SetProBarValue(float,float)));
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
void UDItemWidget::EmitProBarRange(int start,float sum,int base)
{
    emit ProBarRangeSig(start,sum,base);
}

//发射进度值信号
void UDItemWidget::EmitProBarValue(float value,float t)
{
    emit ProBarValueSig(value,t);
}

//设置进度条范围
void UDItemWidget::SetProBarRange(int start,float sum,int base)
{
    if(base == 0)
    {
        proBar->setRange(0,100);
        proBar->setValue(100);
        rateLabel->setText("");
        return;
    }
    proBar->setRange(start,(int)sum);
    switch(base)
    {
        case 1:
           size = QString::number(sum,'f',1) + "B | ";
           break;
        case 2:
           size = QString::number(sum,'f',1) + "K | ";
           break;
        case 3:
           size = QString::number(sum,'f',1) + "M | ";
           break;
        case 4:
           size = QString::number(sum/1024,'f',1) + "G | ";
           break;
    }
    rateLabel->setText(size+"0K/s");
}

//设置进度条值
void UDItemWidget::SetProBarValue(float value,float t)
{
    proBar->setValue((int)value);
    if(t>1024)
        rate = QString::number(t/1024,'f',1) + "M/s";
    else
        rate = QString::number(t,'f',1) + "K/s";
    rateLabel->setText(size+rate);
}
