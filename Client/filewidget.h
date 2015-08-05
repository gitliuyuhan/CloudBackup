/*======================================================
    > File Name: filewidget.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月23日 星期四 10时06分29秒
 =======================================================*/

#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include"filelistwidget.h"

#include<QWidget>
#include<QMenu>
#include<QAction>
#include<QPushButton>
#include<QLabel>

/* 文件管理窗口类 */
class FileWidget : public QWidget
{
    Q_OBJECT
public:
    FileWidget(QWidget* parent=0);
    ~FileWidget(){}

protected:
    virtual void contextMenuEvent(QContextMenuEvent*); //重写鼠标右击菜单虚函数
    
private slots:
    //新建文件夹槽函数
    void NewFolderSlot();
    //文件下载保存对话框
    void ShowDownFileDialog();
    //下载文件对话框
    void ShowUpFileDialog();

private:
    //文件浏览类对象
    FileListWidget*   fileListWidget;
    //右击菜单
    QMenu*            widgetMenu;
    //新建文件夹菜单项
    QAction*          addFolderAction;
    //上传文件菜单项
    QAction*          uploadAction;
    //上传按钮
    QPushButton*      uploadButton;
    //上一级目录
    QPushButton*      lastFolderButton;
    //根目录按钮
    QPushButton*      rootFolderButton;
    //当前目录标签
    QLabel*           currentFolderLabel;

};

#endif //FileWidget_H
