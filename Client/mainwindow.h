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

#include<QMainWindow>
#include<QStackedWidget>
#include<QSplitter>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QMainWindow* parent=0);
    ~MainWindow(){}
private:
    FileWidget*       fileWidget;
    MonitorWidget*    monitorWidget;
    QListWidget*      projectMenu;
    QStackedWidget*   stackWidget;
    QWidget*          topWidget;
public:
    QSplitter*        mainSplitter;
};

#endif//MAINWINDOW_H
