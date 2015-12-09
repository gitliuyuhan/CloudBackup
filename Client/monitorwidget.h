/*======================================================
    > File Name: monitorwidget.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月30日 星期四 16时21分47秒
 =======================================================*/

#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include"monifilewidget.h"
#include"versionwidget.h"
#include"myinclude.h"
#include"finotify.h"

#include<QTabWidget>
#include<QWidget>
#include<QPushButton>

class MonitorWidget : public QTabWidget
{
    Q_OBJECT
public:
    MonitorWidget(QTabWidget* parent=0);
    ~MonitorWidget(){}
private slots:
    //开始或暂停监控
    void StartMonitorSlot(QString,int);
    void ShowAddMonitorSlot();
    void AddMonitorItemSlot(string,InotifyFilter);
private:
    MoniFileWidget*   widget1;
    FInotify          fnotify;
//    QWidget*          widget2;
    QPushButton*      addMonitorButton;
    VersionWidget*    widget3;
};

#endif//MONITORWIDGET_H

