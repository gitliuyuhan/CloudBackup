/*======================================================
    > File Name: MoniFileWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月30日  星期四 21时06分26秒
 =======================================================*/

#include"monifilewidget.h"
#include"myinclude.h"

#include<QHBoxLayout>
#include<QIcon>

//监控项目构造函数
MoniItemWidget::MoniItemWidget(QString s,QString date,QWidget* parent):QWidget(parent)
{
    //设置为暂停状态
    state = 0;

    fileLabel = new QLabel(s,this);
    dateLabel = new QLabel(date,this);
    dateLabel->setStyleSheet("max-width: 196");
    
    runButton = new QPushButton(tr("开始"),this);
    runButton->setIcon(QIcon(":/image/start.png"));
    runButton->setFlat(true);
    runButton->setStyleSheet("max-width:70;"); //设置宽度
    //点击开始或暂停
    connect(runButton,SIGNAL(clicked()),this,SLOT(runButtonSlot()));
    
    delButton = new QPushButton(tr("删除"),this);
    delButton->setIcon(QIcon(":/image/remove.png"));
    delButton->setFlat(true);
    delButton->setStyleSheet("max-width:70");
    //点击删除
    connect(delButton,SIGNAL(clicked()),this,SLOT(delButtonSlot()));

    QHBoxLayout*   mainLayout = new QHBoxLayout;
    mainLayout->addWidget(fileLabel);
    mainLayout->addWidget(dateLabel);
    mainLayout->addWidget(runButton);
    mainLayout->addWidget(delButton);

    this->setLayout(mainLayout);
}

//开始按钮槽函数
void MoniItemWidget::runButtonSlot()
{
    if(state)
    {
        runButton->setText(tr("开始"));
        runButton->setIcon(QIcon(":/image/start.png"));
        state = 0;
        //暂停
        emit(StartSig(fileLabel->text(),state));
    }
    else
    {
        runButton->setText(tr("暂停"));
        runButton->setIcon(QIcon(":/image/stop.png"));
        state = 1;
        //开始
        emit(StartSig(fileLabel->text(),state));
    }
}

//删除按钮槽函数
void MoniItemWidget::delButtonSlot()
{}

//监控窗口构造函数
MoniFileWidget::MoniFileWidget(QListWidget* parent):QListWidget(parent)
{
}

//发射开始暂停信号
void MoniFileWidget::EmitStartSigSlot(QString file,int state)
{
    emit(StartSig(file,state));
}

void MoniFileWidget::SetSelectedFile(QString s)
{
    str = s;
}

void MoniFileWidget::AddListItem(QString date)
{
    QListWidgetItem* item = new QListWidgetItem(this);
    item->setSizeHint(QSize(0,40));
    
    MoniItemWidget*  itemWidget = new MoniItemWidget(str,date,this);

    addItem(item);
    connect(itemWidget,SIGNAL(StartSig(QString,int)),this,SLOT(EmitStartSigSlot(QString,int)));
    setItemWidget(item,itemWidget);
}
