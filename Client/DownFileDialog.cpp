/*======================================================
    > File Name: DownFileDialog.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月28日 星期二 10时01分56秒
 =======================================================*/

#include"downfiledialog.h"

#include<QVBoxLayout>
#include<QHBoxLayout>

/* 构造函数 */
DownFileDialog::DownFileDialog(QDialog*  parent):QDialog(parent)
{
    //文件浏览对话框
    fileDialog = new QFileDialog;

    //只显示目录
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
 
    saveLabel = new QLabel(tr("路径："));
    saveLineEdit = new QLineEdit;
    saveButton = new QPushButton(tr("保存"));

    connect(fileDialog,SIGNAL(currentChanged(const QString &)),this,SLOT(LinePathShow(const QString &)));

    QHBoxLayout*  baseLayout = new QHBoxLayout;
    baseLayout->addWidget(saveLabel);
    baseLayout->addWidget(saveLineEdit);
    baseLayout->addWidget(saveButton);

    QVBoxLayout*  mainLayout = new QVBoxLayout;
    mainLayout->addWidget(fileDialog);
    mainLayout->addLayout(baseLayout);
    this->setLayout(mainLayout);

    connect(fileDialog,SIGNAL(accepted()),this,SLOT(close()));
    connect(fileDialog,SIGNAL(rejected()),this,SLOT(close()));
}

/* 显示选择的路径，槽函数*/
void DownFileDialog::LinePathShow(const QString &path)
{
    saveLineEdit->setText(path);
}


