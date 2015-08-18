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
#include<QLineEdit>
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
    void EmitProBarRange(int,float,int);
    //发射进度值信号
    void EmitProBarValue(float,float);
signals:
    //状态信号
    void setStateSig(QString);
    //进度范围信号
    void ProBarRangeSig(int,float,int);
    //进度值信号
    void ProBarValueSig(float,float);
public slots:
    //设置状态
    void setState(QString);
    //设置进度条范围
    void SetProBarRange(int,float,int);
    //设置进度值
    void SetProBarValue(float,float);
private:
    int              type;
    QLabel*          stateLabel;
    QLineEdit*       fileLabel;
    QProgressBar*    proBar;
    QPushButton*     stopButton;
    QPushButton*     delButton;
    QLabel*          rateLabel;
    QString          size;
    QString          rate;
};

#endif//UDITEMWIDGET_H
