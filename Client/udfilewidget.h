/*======================================================
    > File Name: udfilewidget.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月10日 星期一 16时21分35秒
 =======================================================*/

#ifndef UDFILEWIDGET_H
#define UDFILEWIDGET_H

#include"myinclude.h"

#include<QTabWidget>
#include<QListWidget>
#include<QString>
#include<QMetaType>
#include<QListWidgetItem>

class UDFileWidget : public QTabWidget
{
    Q_OBJECT
public:
    UDFileWidget(QTabWidget*  parent = 0);
    ~UDFileWidget(){}
signals:
    void UpFileSig(string);
    void DownFileSig(string);
public slots:
    void UpFile(string);
    void DownFile(string);
public:
    //触发信号
    void EmitUpFileSig(string);
    void EmitDownFileSig(string);
private:
    static void* UpFileThread(void* arg);
    static void* DownFileThread(void* arg);
private:
    QListWidget*       upListWidget;
    QListWidget*       downListWidget;
};

//线程参数结构体
struct  Param
{
    UDFileWidget*      p;
    string             strjson;
    QListWidgetItem*   item;
};

#endif//UDFILEWIDGET_H
