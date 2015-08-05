/*======================================================
    > File Name: MoniFileWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月30日  星期四 21时06分26秒
 =======================================================*/

#include"monifilewidget.h"

#include<QHBoxLayout>
#include<QIcon>

MoniFileWidget::MoniFileWidget(QListWidget* parent):QListWidget(parent)
{
}

void MoniFileWidget::SetSelectedFile(QString s)
{
    str = s;
}

void MoniFileWidget::AddListItem()
{
    QLabel*  fileLabel = new QLabel(str,this);
    QLabel*  dateLabel = new QLabel(tr("2015.07.28  12:12:05"),this);
    dateLabel->setStyleSheet("max-width: 196");
    QListWidgetItem* item = new QListWidgetItem(this);
    item->setSizeHint(QSize(0,40));
    QWidget*  itemWidget = new QWidget(this);
    QPushButton*  runButton = new QPushButton(tr("开始"),this);
    runButton->setIcon(QIcon(":/image/start.png"));
    runButton->setFlat(true);
    runButton->setStyleSheet("max-width:70;"); //设置宽度
    QPushButton*  delButton = new QPushButton(tr("删除"),this);
    delButton->setIcon(QIcon(":/image/remove.png"));
    delButton->setFlat(true);
    delButton->setStyleSheet("max-width:70");

    QHBoxLayout*   mainLayout = new QHBoxLayout;
    mainLayout->addWidget(fileLabel);
    mainLayout->addWidget(dateLabel);
    mainLayout->addWidget(runButton);
    mainLayout->addWidget(delButton);

    itemWidget->setLayout(mainLayout);

    addItem(item);
    setItemWidget(item,itemWidget);
}
