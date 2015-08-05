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

#include<QTabWidget>
#include<QWidget>

class MonitorWidget : public QTabWidget
{
    Q_OBJECT
public:
    MonitorWidget(QTabWidget* parent=0);
    ~MonitorWidget(){}
private:
    MoniFileWidget*   widget1;
    QWidget*   widget2;
    QWidget*   widget3;
};

#endif//MONITORWIDGET_H

