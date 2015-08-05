/*======================================================
    > File Name: enterwidget.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月02日 星期日 20时51分15秒
 =======================================================*/

#ifndef ENTERWIDGET_H
#define ENTERWIDGET_H

#include"loginwidget.h"

#include<QWidget>
#include<QSplitter>

class EnterWidget : public QWidget
{
    Q_OBJECT
public:
    EnterWidget(QWidget* parent=0);
    ~EnterWidget(){}
    LoginWidget*   loginWidget;
private slots:
    void ShowLoginWidget();
private:
    QSplitter*     mainSplitter;
    QWidget*       menuWidget;
//    LoginWidget*   loginWidget;
    QPushButton*   loginButton;
    QPushButton*   sendFileButton;
    QPushButton*   recvFileButton;
};

#endif//ENTERWIDGET_H
