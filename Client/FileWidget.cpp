/*======================================================
    > File Name: FileWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月23日 星期四 10时12分12秒
 =======================================================*/

#include"filewidget.h"
#include"downfiledialog.h"
#include"upfiledialog.h"

#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QContextMenuEvent>
#include<QMouseEvent>
#include<QDebug>
#include<QStringList>

/* 构造函数 */
FileWidget::FileWidget(QWidget* parent) : QWidget(parent)
{
    fileListWidget = new FileListWidget;
    fileListWidget->setFrameShape(QListWidget::NoFrame);//无边框
   
    uploadButton = new QPushButton(tr("上传"));
    lastFolderButton = new QPushButton(tr("上一级目录"));
    rootFolderButton = new QPushButton(tr("根目录"));
    currentFolderLabel = new QLabel(tr("> /"));

    connect(uploadButton,SIGNAL(clicked()),this,SLOT(ShowUpFileDialog()));

    setContextMenuPolicy(Qt::DefaultContextMenu);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(uploadButton);
    topLayout->addWidget(lastFolderButton);
    topLayout->addWidget(rootFolderButton);
    topLayout->addWidget(currentFolderLabel);
    topLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(fileListWidget);
    setLayout(mainLayout);
}

/* 新建文件夹槽函数 */
void FileWidget::NewFolderSlot()
{
    fileListWidget->AddNewFolder();
}

/* 弹出右键菜单 */
void FileWidget::contextMenuEvent(QContextMenuEvent* event) //重写鼠标右击菜单虚函数
{
    addFolderAction = new QAction(QIcon(":/image/newfolder.png"),tr("新建文件夹"),this);
    uploadAction = new QAction(QIcon(":/image/upload.png"),tr("上传文件"),this);
    connect(uploadAction,SIGNAL(triggered()),this,SLOT(ShowUpFileDialog()));

    widgetMenu = new QMenu(this);
    widgetMenu->addAction(addFolderAction);
    connect(addFolderAction,SIGNAL(triggered()),this,SLOT(NewFolderSlot()));

    widgetMenu->addAction(uploadAction);

    QListWidgetItem*    item1 = fileListWidget->itemAt(mapFromGlobal(QCursor::pos()));
    if(item1!=NULL)
    {
        fileListWidget->setFileItem(item1);
        QMenu* fileMenu = new QMenu(this);
        QAction* downloadAction = new QAction(QIcon(":/image/download.png"),tr("下载"),this);
        fileMenu->addAction(downloadAction);
        connect(downloadAction,SIGNAL(triggered()),this,SLOT(ShowDownFileDialog()));

        QAction* renameAction = new QAction(QIcon(":/image/rename.png"),tr("重命名"),this);
        fileMenu->addAction(renameAction);
        connect(renameAction,SIGNAL(triggered()),fileListWidget,SLOT(RenameFile()));

        QAction* deleteAction = new QAction(QIcon(":/image/delete.png"),tr("删除"),this);
        fileMenu->addAction(deleteAction);
        connect(deleteAction,SIGNAL(triggered()),fileListWidget,SLOT(DeleteFile()));

        fileMenu->exec(QCursor::pos());
    }
    else
    {    
        widgetMenu->exec(QCursor::pos());
    }
}


/* 保存下载文件对话框槽函数 */
void FileWidget::ShowDownFileDialog()
{
    DownFileDialog*   downDialog = new DownFileDialog;
    downDialog->show();
}

/* 上传文件对话框槽函数 */
void FileWidget::ShowUpFileDialog()
{
    UpFileDialog*     upDialog = new UpFileDialog;
    upDialog->show();
}
