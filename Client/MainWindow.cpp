/*======================================================
    > File Name: MainWindow.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月29日 星期三 14时25分20秒
 =======================================================*/

#include"mainwindow.h"
#include"connectserv.h"

#include<QHBoxLayout>
#include<QString>

#include<json/json.h>
#include<string>

#define  BUFSIZE    1024

extern int         sockFd;
extern QString     servIp;
extern QString     servPort;
extern QString     userName;

MainWindow::MainWindow(QMainWindow* parent):QMainWindow(parent)
{
    fileWidget = new FileWidget;
    monitorWidget = new MonitorWidget;
    udFileWidget = new UDFileWidget;

    mainSplitter = new QSplitter(Qt::Vertical);
    QSplitter  *baseSplitter = new QSplitter(Qt::Horizontal);
    //项目列表
    QFont   font;
    font.setPointSize(12);

    projectMenu = new QListWidget;
    projectMenu->setIconSize(QSize(25,25));
    projectMenu->setFrameShape(QListWidget::NoFrame);
/*
    QListWidget::item:selected{
         background: green;     
    }
    */
    projectMenu->setStyleSheet("QListWidget{background: #F0F8FF;} \ QListWidget::item:selected{background-color:#CFCFCF;color:black;} ");//F6FAFD F0FFF0
    
 //   projectMenu->setStyleSheet("QListWidget::item:selected{background-color:red;}");
    projectMenu->setFocusPolicy(Qt::NoFocus); //去掉鼠标选中的虚线框

    QListWidgetItem   *item1 = new QListWidgetItem(QIcon(":/image/file.png"),tr("我的文件"));
    item1->setFont(font);
    item1->setSizeHint(QSize(0,40));   
    projectMenu->addItem(item1);

    QListWidgetItem   *item2 = new QListWidgetItem(QIcon(":/image/monitor.png"),tr("文件监控"));
    item2->setFont(font);
    item2->setSizeHint(QSize(0,40));
    projectMenu->addItem(item2);

    QListWidgetItem   *item4 = new QListWidgetItem(QIcon(":/image/trans.jpg"),tr("传输列表"));
    item4->setFont(font);
    item4->setSizeHint(QSize(0,40));
    projectMenu->addItem(item4);

    QListWidgetItem   *item3 = new QListWidgetItem(QIcon(":/image/message.png"),tr("消息通知"));
    item3->setFont(font);
    item3->setSizeHint(QSize(0,40));
    projectMenu->addItem(item3);

    QListWidgetItem   *itemQuit = new QListWidgetItem;
    itemQuit->setSizeHint(QSize(0,40));
    projectMenu->addItem(itemQuit);
    QPushButton       *quitButton = new QPushButton(tr("退出帐号"));
    quitButton->setFont(font);
    quitButton->setFocusPolicy(Qt::NoFocus);
    quitButton->setStyleSheet("background-color: lawngreen;border:0");
    connect(quitButton,SIGNAL(clicked()),mainSplitter,SLOT(close()));
    projectMenu->setItemWidget(itemQuit,quitButton);

    stackWidget = new QStackedWidget(this);
    stackWidget->addWidget(fileWidget);
    stackWidget->addWidget(monitorWidget);
    stackWidget->addWidget(udFileWidget);
    
    connect(projectMenu,SIGNAL(currentRowChanged(int)),stackWidget,SLOT(setCurrentIndex(int)));
    projectMenu->setCurrentRow(0);
 
    //顶部窗口
    topWidget = new QWidget;
    font.setPointSize(18);
    QLabel*        titleLabel = new QLabel(tr("云备份"));
    font.setBold(true);  //粗体
    titleLabel->setFont(font);
    titleLabel->setStyleSheet("color: white");
    titleLabel->setAlignment(Qt::AlignCenter);//字体居中

    QHBoxLayout*   topLayout = new QHBoxLayout;
    topLayout->addWidget(titleLabel);
    topWidget->setMaximumHeight(56);
    topWidget->setStyleSheet("background: #63B8FF;");
    topWidget->setLayout(topLayout);

    //下部窗口
    baseSplitter->setHandleWidth(1); //分割线宽度
    baseSplitter->addWidget(projectMenu);
    baseSplitter->addWidget(stackWidget);
    baseSplitter->setStretchFactor(0,1);
    baseSplitter->setStretchFactor(1,4);


    //主窗口
//    mainSplitter->setStyleSheet("QSplitter::handle { background-color: grey }"); //设置分界线的样式
    mainSplitter->setHandleWidth(1);
    mainSplitter->addWidget(topWidget);
    mainSplitter->addWidget(baseSplitter);
    mainSplitter->resize(QSize(800,600));

}

/* 显示主界面 */
void MainWindow::ShowMainWindow(QString ip,QString port,int sock,QString sname,QString sjson)
{
    mainSplitter->show();
    servIp = ip;
    servPort = port;
    sockFd = sock;
    userName = sname;
    string                strjson = sjson.toStdString();
    string                str;
    Json::Value           json;
    Json::Reader          reader;
    if(reader.parse(strjson,json))
    {
        str = json["AllFiles"].asString();
    }
    this->fileWidget->EmitShowFilesSig(QString::fromStdString(str));
    pthread_create(&thid,NULL,RecvThread,this);
}

/* 接收消息线程 */
void* MainWindow::RecvThread(void* arg)
{
    MainWindow*       cthis = (MainWindow*)arg;
    char       buf[BUFSIZE];
    int        ret = -1;
    Json::Reader      reader;
    Json::Value       json;
    int        mark = -1;
    
    while(1)
    {
        memset(buf,'\0',sizeof(buf));

        ret = recv(sockFd,buf,BUFSIZE,0);
        if(ret<=0)
            continue;
        buf[strlen(buf)] = '\0';
        cout<<"recv: "<<buf<<endl;
        if(reader.parse(buf,json))
        {
            mark = json["status"].asInt();
        }
        string    strbuf = buf;
        string    strfolder,strfiles;
        switch(mark)
        {
            case 3:
                //发射进入目录信号
                strfolder = json["Folder"].asString();
                strfiles = json["AllFiles"].asString();
                cthis->fileWidget->EmitShowFilesSig(QString::fromStdString(strfiles));
                cthis->fileWidget->EmitSetFolderSig(QString::fromStdString(strfolder));
                break;
            case 7:
                //发射上传文件信号
                cout<<"recv 7\n";
                cthis->udFileWidget->EmitUpFileSig(strbuf);
                cout<<"7\n";
                break;
            case 8:
                //发射下载文件信号
                cthis->udFileWidget->EmitDownFileSig(strbuf);
                break;
        }
    }
    pthread_exit(0);
}

