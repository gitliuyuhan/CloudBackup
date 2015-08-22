/*======================================================
    > File Name: FilebrowseWidget.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月22日 星期三 16时04分08秒
 =======================================================*/

#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include<QListWidget>
#include<QListWidgetItem>
#include<QStringList>

/* 文件列表类 */
class FileListWidget : public QListWidget
{
    Q_OBJECT
public:
        FileListWidget(QListWidget* parent=0);
        ~FileListWidget(){}
protected:
        //鼠标双击事件
        void mouseDoubleClickEvent(QMouseEvent* event);

public:
        //定位文件项目
        void setFileItem(QListWidgetItem*);

public slots:
        //新建文件夹
        void AddNewFolder(); 
        //文件重命名
        void RenameFile();
        //删除文件或目录
        void DeleteFile();

private slots:
        //设置文件名字
        void SetFileName();
        //上一层或下一层目录
        void ShowFiles(QStringList);
 
signals:
        void EnterFolderSig(QString);
        void ShowFilesSig(QStringList);
        
public:
        //编辑文件名字
        void EditFileName();
        //下载
//        void DownloadFile();
        //添加条目
        QListWidgetItem* AddFileItem(QString);
        void EmitShowFilesSig(QString);
private:
        //文件或目录项
        QListWidgetItem  *fileItem;
        //文件名字编辑框
        QLineEdit       *lineEdit;
};

#endif //FILELISTWIDGET_H


