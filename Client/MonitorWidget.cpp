/*======================================================
    > File Name: MonitorWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月30日 星期四 16时27分08秒
 =======================================================*/

#include"monitorwidget.h"

MonitorWidget::MonitorWidget(QTabWidget* parent):QTabWidget(parent)
{
//    setTabShape(QTabWidget::Triangular);
//    setFocusPolicy(Qt::NoFocus);
    setStyleSheet("QTabBar::tab{background: #D1EEEE;min-width:135;min-height:40;} \ QTabBar::tab:hover{background: white} \ QTabBar::tab:selected{background:white;border-color: #EE0000}");
   
    widget1 = new MoniFileWidget;
    widget1->SetSelectedFile("/home/lyh/");
    for(int i=0;i<15;i++)
        widget1->AddListItem();
    widget1->SetSelectedFile("/home/lyh/program/Qt/CloudBackup/image/start.png");
    widget1->AddListItem();

    widget2 = new QWidget;
    widget3 = new QWidget;

    addTab(widget1,tr("正在监控"));
    addTab(widget2,tr("添加监控"));
    addTab(widget3,tr("版本回退"));
}
