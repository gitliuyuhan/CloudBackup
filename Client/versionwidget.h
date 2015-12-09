/*======================================================
    > File Name: monifilewidget.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月30日 星期四 20时40分39秒
 =======================================================*/

#ifndef VERSIONWIDGET_H
#define VERSIONWIDGET_H

#include<QListWidget>
#include<QPushButton>
#include<QLabel>
#include<QWidget>
#include<QString>

#include"myinclude.h"

class VersionWidget : public QListWidget
{
    Q_OBJECT
public:
    VersionWidget(QListWidget* parent=0);
    ~VersionWidget(){}
public:
    //添加条目
    void AddListItem(QString,QString,QString);
public slots:
    void VersionAlterSlot(string,string,string);
};

class VersionItemWidget : public QWidget
{
    Q_OBJECT
public:
    VersionItemWidget(QWidget* parent=0);
    ~VersionItemWidget(){}
private slots:
    //回退
    void cancelButtonSlot();
    //下载
    void downButtonSlot();
public:    
    //项目初始化
    void versionItemInit(QString,QString,QString);

private:
    QLabel*             typeLabel; //操作类型
    QLabel*             dscLabel;  //源对象
    QLabel*             dstLabel;  //目的对象
    QPushButton*        cancelButton; //回退
    QPushButton*        downButton;   //下载
};

#endif//VERSIONWIDGET_H
