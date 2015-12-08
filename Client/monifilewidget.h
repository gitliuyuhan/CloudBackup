/*======================================================
    > File Name: monifilewidget.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月30日 星期四 20时40分39秒
 =======================================================*/

#ifndef MONIFILEWIDGET_H
#define MONIFILEWIDGET_H

#include<QListWidget>
#include<QPushButton>
#include<QLabel>
#include<QWidget>
#include<QString>

class MoniFileWidget : public QListWidget
{
    Q_OBJECT
public:
    MoniFileWidget(QListWidget* parent=0);
    ~MoniFileWidget(){}
public:
    void SetSelectedFile(QString);
    void AddListItem(QString);
private slots:
    void EmitStartSigSlot(QString,int);
signals:
    void StartSig(QString,int);
private:
    QString             str;
};

class MoniItemWidget : public QWidget
{
    Q_OBJECT
public:
    MoniItemWidget(QString,QString,QWidget* parent=0);
    ~MoniItemWidget(){}
private slots:
    void runButtonSlot();
    void delButtonSlot();
signals:
    //发射开始或暂停信号
    void StartSig(QString,int);

private:
    int                 state;
    QLabel*             fileLabel;
    QLabel*             dateLabel;
    QPushButton*        runButton;
    QPushButton*        delButton;
};

#endif//MONIFILEWIDGET_H
