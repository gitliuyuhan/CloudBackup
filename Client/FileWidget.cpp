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
#include"connectserv.h"

#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QContextMenuEvent>
#include<QMouseEvent>
#include<QDebug>
#include<QStringList>
#include<json/json.h>

extern int sockFd;

/* 构造函数 */
FileWidget::FileWidget(QWidget* parent) : QWidget(parent)
{
    fileListWidget = new FileListWidget;
    fileListWidget->setFrameShape(QListWidget::NoFrame);//无边框
   
    uploadButton = new QPushButton(tr("上传"));
    lastFolderButton = new QPushButton(tr("上一级目录"));
    rootFolderButton = new QPushButton(tr("根目录"));

    connect(rootFolderButton,SIGNAL(clicked()),this,SLOT(EnterRootFolder()));
    connect(lastFolderButton,SIGNAL(clicked()),this,SLOT(EnterProFolder()));

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

    connect(fileListWidget,SIGNAL(EnterFolderSig(QString)),this,SLOT(SetCurrentFolder(QString)));
    connect(this,SIGNAL(SetFolder(QString)),this,SLOT(SetCurFolder(QString)));
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

    //判断鼠标点击位置是否为item
    QListWidgetItem*    item1 = fileListWidget->itemAt(mapFromGlobal(QCursor::pos()));
    if(item1!=NULL)
    {
        this->selectedFile = item1->text();
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


/* 下载文件对话框槽函数 */
void FileWidget::ShowDownFileDialog()
{
    DownFileDialog*   downDialog = new DownFileDialog;
    downDialog->file = selectedFile;
    downDialog->filePath = (currentFolderLabel->text()).replace(0,2,"") + selectedFile;
    downDialog->show();
}

/* 上传文件对话框槽函数 */
void FileWidget::ShowUpFileDialog()
{
    UpFileDialog*     upDialog = new UpFileDialog;
    QString str = currentFolderLabel->text();
    str.replace("> ","");
    upDialog->clientPath = str;
    upDialog->show();
}

/* 进入某个目录 */
void FileWidget::EnterFolder(string folder)
{
    Json::Value       json;
    json["status"] = Json::Value(3);
    json["Folder"] = Json::Value(folder);
    string buf = json.toStyledString();
    send(sockFd,buf.c_str(),buf.length(),0);
}

//设置双击进入目录
void FileWidget::SetCurrentFolder(QString folder)
{
 //   currentFolderLabel->setText((currentFolderLabel->text())+folder+"/");
    
    QString   str = currentFolderLabel->text() + folder + "/";
    str.replace("> ","");
    EnterFolder(str.toStdString());
}

void FileWidget::EmitSetFolderSig(QString folder)
{
    emit SetFolder(folder);
}

//设置当前目录
void FileWidget::SetCurFolder(QString folder)
{
    currentFolderLabel->setText("> " + folder);
}

//进入根目录
void FileWidget::EnterRootFolder()
{
    EnterFolder("/");
}

//进入上层目录
void FileWidget::EnterProFolder()
{
    QString      qstr = currentFolderLabel->text();
    qstr.replace("> ","");
    string       str = qstr.toStdString();
    if(str.compare("/") != 0)
    {
        int    i;
        for(i=str.length()-2;i >= 0;i--)
        {
            if(str.at(i) == '/')
                break;
        }
        str = str.substr(0,i+1);
    }
    EnterFolder(str);
}

/* 发射进入目录信号 */
void FileWidget::EmitShowFilesSig(QString files)
{
    fileListWidget->EmitShowFilesSig(files);
}
