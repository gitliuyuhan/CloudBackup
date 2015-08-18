/*======================================================
    > File Name: mainwindow.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月29日 星期三 14时07分53秒
 =======================================================*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"filewidget.h"
#include"monitorwidget.h"
#include"enterwidget.h"
#include"udfilewidget.h"
#include"myinclude.h"

#include<QMainWindow>
#include<QStackedWidget>
#include<QSplitter>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QMainWindow* parent=0);
    ~MainWindow(){}

public slots:
    void ShowMainWindow(QString,QString,int,QString,QString);
public:
    static void* RecvThread(void* arg);
private:
    //入口界面
    EnterWidget*      enterWidget;
    //文件管理
    FileWidget*       fileWidget;
    //文件监控
    MonitorWidget*    monitorWidget;
    //传输列表
    UDFileWidget*     udFileWidget;
    //项目菜单
    QListWidget*      projectMenu;
    //窗口动态切换
    QStackedWidget*   stackWidget;
    //主界面顶部分割窗口
    QWidget*          topWidget;

    //线程ID
    pthread_t         thid;
public:
    //窗口分割器
    QSplitter*        mainSplitter;
};

#endif//MAINWINDOW_H
