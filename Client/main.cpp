/*======================================================
    > File Name: main.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月23日 星期四 10时15分07秒
 =======================================================*/

#include"mainwindow.h"
#include"enterwidget.h"

#include<QApplication>
#include<QTextCodec>

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale()); //设置编码

    MainWindow    *mainWindow = new MainWindow;

    EnterWidget   enterWidget;
    
    QObject::connect(enterWidget.loginWidget,SIGNAL(LoginOk(QString,QString,int,QString,QString)),mainWindow,SLOT(ShowMainWindow(QString,QString,int,QString,QString)));

    
    return app.exec();
}
