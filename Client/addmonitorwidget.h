/*======================================================
    > File Name: addmonitor.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年09月28日 星期一 12时50分55秒
 =======================================================*/

#ifndef ADDMONITORWIDGET_H
#define ADDMONITORWIDGET_H

#include"finotify.h"

#include<QWidget>
#include<QListWidget>
#include<QListWidgetItem>
#include<QCheckBox>
#include<QLineEdit>
#include<QLabel>
#include<QString>
#include<QPushButton>

#include"myinclude.h"

/* 添加监控文件 */
class AddMonitorWidget : public QWidget
{
    Q_OBJECT
public:
        AddMonitorWidget(QWidget* parent=0);
        ~AddMonitorWidget(){}

protected:
        //双击事件
//        void mouseDoubleClickEvent(QMouseEvent* event);

private slots:
        void ShowFolderSlot(QListWidgetItem*);
        void SaveSlot(int,QString);
        void okSlot();
signals:
        void AddMonitorItemSig(string,InotifyFilter);
        
private:
        void ShowFolder(string);        

private:
        QLabel*          filterlabel;
        //起始日期
        QLineEdit*       startdate;
        //结束日期
        QLineEdit*       enddate;
        //文件后缀
        QLineEdit*       suffix;
        //起始大小
        QLineEdit*       startsize;
        //截止大小
        QLineEdit*       endsize;
        //选择目录
        QCheckBox*       folderCBox;
        //选择普通文件
        QCheckBox*       fileCBox;
        //文件浏览窗口
        QListWidget*     listwidget;

        set<string>      fileSet;
        //过滤信息
        InotifyFilter    filter;            
};

class  FileItem : public  QWidget
{
    Q_OBJECT
public:
        FileItem(QWidget* parent=0);

protected:
        //双击事件
//        void mouseDoubleClickEvent(QMouseEvent* event);

public slots:
        void stateChangedSlot(int);

signals:
        void SaveSig(int,QString);

public:
        void setItem(QString,QString);
public:
        QCheckBox*       selectCBox;
        QLabel*          namelabel;
        QLabel*          sizelabel;
        QLabel*          datelabel; 
        QLabel*          iconlabel;  
        QString          parentDir;     
};

#endif //ADDMONITORWIDGET_H
