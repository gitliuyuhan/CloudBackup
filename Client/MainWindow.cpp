/*======================================================
    > File Name: MainWindow.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月29日 星期三 14时25分20秒
 =======================================================*/

#include"mainwindow.h"

#include<QHBoxLayout>

MainWindow::MainWindow(QMainWindow* parent):QMainWindow(parent)
{
    fileWidget = new FileWidget;
    monitorWidget = new MonitorWidget;

    mainSplitter = new QSplitter(Qt::Vertical);
    QSplitter  *baseSplitter = new QSplitter(Qt::Horizontal);
    //项目列表
    QFont   font;
    font.setPointSize(15);

    projectMenu = new QListWidget;
    projectMenu->setIconSize(QSize(25,25));
    projectMenu->setFrameShape(QListWidget::NoFrame);
    projectMenu->setStyleSheet("background: #F0FFF0");//F6FAFD
    projectMenu->setFocusPolicy(Qt::NoFocus); //去掉鼠标选中的虚线框

    QListWidgetItem   *item1 = new QListWidgetItem(QIcon(":/image/file.png"),tr("我的文件"));
    item1->setFont(font);
    projectMenu->addItem(item1);

    QListWidgetItem   *item2 = new QListWidgetItem(QIcon(":/image/monitor.png"),tr("文件监控"));
    item2->setFont(font);
    projectMenu->addItem(item2);

    QListWidgetItem   *item3 = new QListWidgetItem(QIcon(":/image/message.png"),tr("消息通知"));
    item3->setFont(font);
    projectMenu->addItem(item3);

    stackWidget = new QStackedWidget(this);
    stackWidget->addWidget(fileWidget);
    stackWidget->addWidget(monitorWidget);
    
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
//    mainSplitter->show();
}

