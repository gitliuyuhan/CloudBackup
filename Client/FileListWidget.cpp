/*======================================================
    > File Name: FileListWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月23日 星期四 07时48分29秒
 =======================================================*/

#include"filelistwidget.h"

#include<QLineEdit>
#include<QListWidgetItem>
#include<QPixmap>
#include<QIcon>
#include<QSize>
#include<QMenu>
#include<QAction>
#include<QVBoxLayout>
#include<QFileIconProvider>
#include<QDebug>


#define W_ICONSIZE     96     //图标宽度
#define H_ICONSIZE     96     //图片高度
#define H_ITEMSIZE     120    //单元项高度
#define SPACING        10     //单元项间距

/* 构造函数 */
FileListWidget::FileListWidget(QListWidget* parent):QListWidget(parent)
{
    setIconSize(QSize(W_ICONSIZE,H_ICONSIZE));
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    
    setSpacing(SPACING);

//    setContextMenuPolicy(Qt::DefaultContextMenu);
    connect(this,SIGNAL(ShowFilesSig(QStringList)),this,SLOT(ShowFiles(QStringList)));

}

/* 定位文件 */
void FileListWidget::setFileItem(QListWidgetItem* item)
{
    fileItem = item;
}

/* 添加条目 */
QListWidgetItem*  FileListWidget::AddFileItem(QString filename)
{
	QFileIconProvider   icon_provider;
    QPixmap filePixmap;
    if(filename.at(filename.length()-1)=='/')
    {
        QIcon iconfolder = icon_provider.icon(QFileIconProvider::Folder);
        filePixmap = iconfolder.pixmap(QSize(W_ICONSIZE,H_ICONSIZE));
        filename=filename.left(filename.length()-1);
    }
    else
    {
        QIcon iconfile = icon_provider.icon(QFileIconProvider::File);
        filePixmap = iconfile.pixmap(QSize(W_ICONSIZE,H_ICONSIZE));
    }
    fileItem = new QListWidgetItem(QIcon(filePixmap.scaled(QSize(W_ICONSIZE,H_ICONSIZE))),filename);
    fileItem->setSizeHint(QSize(W_ICONSIZE,H_ITEMSIZE));
    addItem(fileItem);
    return fileItem;
}

/* 编辑文件名字 */
void FileListWidget::EditFileName()
{
    lineEdit = new QLineEdit(fileItem->text());
    lineEdit->setMaximumSize(QSize(96,20));

    setItemWidget(fileItem,lineEdit);
    lineEdit->setFocus();
 
    lineEdit->selectAll();
    connect(lineEdit,SIGNAL(editingFinished()),this,SLOT(SetFileName()));
}

/* 设置文件名字 */
void FileListWidget::SetFileName()
{
    QString s=lineEdit->text();
    if(!s.isEmpty())
        fileItem->setText(s);
    removeItemWidget(fileItem);
}

/* 删除文件或目录 */
void FileListWidget::DeleteFile()
{
    takeItem(currentRow());
}

/* 显现目录下所以文件 */
void FileListWidget::ShowFiles(QStringList  filelist)
{
    clear();
    if(filelist.size()>0)
    {
        int      i;
        for(i=0;i<filelist.size();i++)
        {
            qDebug()<<filelist[i];
            AddFileItem(filelist[i]);
        }
    }
}

/* 新建文件夹槽函数 */
void FileListWidget::AddNewFolder()
{
    AddFileItem(tr("未命名/"));
    EditFileName();
}

/* 文件重命名槽函数 */
void FileListWidget::RenameFile()
{
    EditFileName();
}

/* 发射进入文件信号 */
void FileListWidget::EmitShowFilesSig(QString files)
{
    QStringList     filelist;
    filelist = files.split(" ",QString::SkipEmptyParts);
    emit ShowFilesSig(filelist);
}

/* 鼠标双击事件 */
void FileListWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    qDebug("sssss");
    fileItem = this->itemAt(mapFromGlobal(QCursor::pos()));
    if(fileItem != NULL)
    {
//        QStringList   filelist;
//        filelist<<tr("图片/")<<tr("111.png")<<tr("abc.txt")<<tr("hhh.txt")<<tr("视频/");
        emit EnterFolderSig(fileItem->text());
//        this->ShowFiles(filelist);
    }
}

