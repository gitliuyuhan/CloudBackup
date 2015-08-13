/*======================================================
    > File Name: uditemwidget.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年08月13日 星期四 13时56分34秒
 =======================================================*/

#ifndef UDITEMWIDGET_H
#define UDITEMWIDGET_H

#include<QWidget>
#include<QPushButton>
#include<QLabel>
#include<QProgressBar>
#include<QString>

class UDItemWidget : public QWidget
{
    Q_OBJECT
public:
    UDItemWidget(int ty=2,QWidget* parent=0);
    ~UDItemWidget(){}
public:
    void setFileLabel(QString);
    //状态信号
    void EmitStateSig(QString);
    //发射进度范围信号
    void EmitProBarRange(int,int);
    //发射进度值信号
    void EmitProBarValue(int);
signals:
    //状态信号
    void setStateSig(QString);
    //进度范围信号
    void ProBarRangeSig(int,int);
    //进度值信号
    void ProBarValueSig(int);
public slots:
    //设置状态
    void setState(QString);
    //设置进度条范围
    void SetProBarRange(int,int);
    //设置进度值
    void SetProBarValue(int);
private:
    int              type;
    QLabel*          stateLabel;
    QLabel*          fileLabel;
    QProgressBar*    proBar;
    QPushButton*     stopButton;
    QPushButton*     delButton;
};

#endif//UDITEMWIDGET_H
