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
    void AddListItem();
private:
    QString             str;
};

#endif//MONIFILEWIDGET_H
