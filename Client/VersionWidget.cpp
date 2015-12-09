/*======================================================
    > File Name: MoniFileWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月30日  星期四 21时06分26秒
 =======================================================*/

#include"versionwidget.h"
#include"myinclude.h"

#include<QHBoxLayout>
#include<QIcon>

//版本回退项目构造函数
VersionItemWidget::VersionItemWidget(QWidget* parent):QWidget(parent)
{
    typeLabel = new QLabel;
    typeLabel->setStyleSheet("max-width:80");
    dscLabel = new QLabel;
//    dscLabel->setStyleSheet("max-width:200");
    dstLabel = new QLabel;
//    dscLabel->setStyleSheet("max-width: 196");
    
    cancelButton = new QPushButton(tr("回退"),this);
    cancelButton->setIcon(QIcon(":/image/cancel.png"));
    cancelButton->setFlat(true);
    cancelButton->setStyleSheet("max-width:70;"); //设置宽度
    
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelButtonSlot()));
    
    downButton = new QPushButton(tr("下载"),this);
    downButton->setIcon(QIcon(":/image/down.png"));
    downButton->setFlat(true);
    downButton->setStyleSheet("max-width:70");
    connect(downButton,SIGNAL(clicked()),this,SLOT(downButtonSlot()));

    QHBoxLayout*   mainLayout = new QHBoxLayout;
    mainLayout->addWidget(typeLabel);
    mainLayout->addWidget(dscLabel);
    mainLayout->addWidget(dstLabel);
    mainLayout->addWidget(cancelButton);
    mainLayout->addWidget(downButton);

    this->setLayout(mainLayout);
}

//回退按钮槽函数
void VersionItemWidget::cancelButtonSlot()
{
}

//下载按钮槽函数
void VersionItemWidget::downButtonSlot()
{}

//项目初始化
void VersionItemWidget::versionItemInit(QString type,QString dsc,QString dst)
{
    typeLabel->setText(type);
    dscLabel->setText(dsc);
    dstLabel->setText(dst);
}

//版本回退窗口构造函数
VersionWidget::VersionWidget(QListWidget* parent):QListWidget(parent)
{
    string      type,dsc,dst;
    ifstream    in("./etc/lyh/alter.txt");
    while(!in.eof())
    {
        in>>type;
        if(in.fail())
            break;
 //       in>>type;
        in>>dsc;
        in>>dst;

        AddListItem(QString::fromStdString(type),QString::fromStdString(dsc),QString::fromStdString(dst));
    }
    in.close();
}

//添加变更项目槽函数
void VersionWidget::VersionAlterSlot(string type,string dsc,string dst)
{
    AddListItem(QString::fromStdString(type),QString::fromStdString(dsc),QString::fromStdString(dst));
}

void VersionWidget::AddListItem(QString type,QString dsc,QString dst)
{
    QListWidgetItem* item = new QListWidgetItem(this);
    item->setSizeHint(QSize(0,40));
    
    VersionItemWidget*  itemWidget = new VersionItemWidget(this);
    itemWidget->versionItemInit(type,dsc,dst);

    addItem(item);
    setItemWidget(item,itemWidget);
}
