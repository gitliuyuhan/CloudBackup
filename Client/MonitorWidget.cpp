/*======================================================
    > File Name: MonitorWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月30日 星期四 16时27分08秒
 =======================================================*/

#include"monitorwidget.h"
#include"addmonitorwidget.h"

#include<QVBoxLayout>

MonitorWidget::MonitorWidget(QTabWidget* parent):QTabWidget(parent)
{
//    setTabShape(QTabWidget::Triangular);
//    setFocusPolicy(Qt::NoFocus);
    setStyleSheet("QTabBar::tab{background: #D1EEEE;min-width:135;min-height:40;} \ QTabBar::tab:hover{background: white} \ QTabBar::tab:selected{background:lightseagreen;border-color: #EE0000}");
   
    widget1 = new MoniFileWidget;
    widget1->setFocusPolicy(Qt::NoFocus);
    widget1->setFrameShape(QListWidget::NoFrame);//无边框
    widget1->setStyleSheet("QListWidget::item:selected{background-color:#C1FFC1;color:black;} ");

    //开始或暂停监控
    connect(widget1,SIGNAL(StartSig(QString,int)),this,SLOT(StartMonitorSlot(QString,int)));
    
    InotifyFilter               itmp;
    string       s,sname;
    int          b;
    ifstream     in("./etc/lyh/record.txt");    //监控文件
    while(!in.eof())
    {
        in>>s;
        if(in.fail())
            break;
        cout<<s<<endl;
        sname = s;   
        widget1->SetSelectedFile(QString::fromStdString(s));
        
        //以下读取过滤
        in>>s;
        itmp.startdate = s;
        in>>s;
        itmp.enddate = s;
        in>>s;
        itmp.suffix = s;
        in>>s;
        itmp.startsize = s;
        in>>s;
        itmp.endsize = s;
        in>>b;
        itmp.isfolder = b;
        in>>b;
        itmp.isfile = b;
        
        //读取起始监控日期
        in>>s;
        cout<<s<<endl;
        itmp.idate = s;
        widget1->AddListItem(QString::fromStdString(s));

        //加入监控列表
        fnotify.InotifyListAdd(sname,itmp);
    }
    in.close();
    
    addMonitorButton = new QPushButton(tr("添加监控项"));
    addMonitorButton->setFocusPolicy(Qt::NoFocus);
    addMonitorButton->setStyleSheet("background-color:white");

    connect(addMonitorButton,SIGNAL(clicked()),this,SLOT(ShowAddMonitorSlot()));
 //   widget2 = new QWidget;
    widget3 = new VersionWidget;
    widget3->setFocusPolicy(Qt::NoFocus);
    widget3->setFrameShape(QListWidget::NoFrame);//无边框
    widget3->setStyleSheet("QListWidget::item:selected{background-color:#C1FFC1;color:black;} ");

    connect(&fnotify,SIGNAL(VersionAlterSig(string,string,string)),widget3,SLOT(VersionAlterSlot(string,string,string)));

    setFocusPolicy(Qt::NoFocus);
    addTab(widget1,tr("监控列表"));
    addTab(addMonitorButton,tr("添加监控"));
    addTab(widget3,tr("版本回退"));
}

//add monitor
void MonitorWidget::ShowAddMonitorSlot()
{
    AddMonitorWidget*      w = new AddMonitorWidget;
    connect(w,SIGNAL(AddMonitorItemSig(string,InotifyFilter)),this,SLOT(AddMonitorItemSlot(string,InotifyFilter)));
    w->resize(800,600);
    w->show();
}

//添加监控项槽
void MonitorWidget::AddMonitorItemSlot(string name,InotifyFilter filter)
{
    widget1->SetSelectedFile(QString::fromStdString(name));
    widget1->AddListItem(QString::fromStdString(filter.idate));
    fnotify.InotifyListAdd(name,filter);
}

//开始或暂停监控
void MonitorWidget::StartMonitorSlot(QString file,int state)
{
    switch(state)
    {
        case 0:
            fnotify.InotifyRm(file.toStdString());
            break;
        case 1:
            fnotify.InotifyAdd(file.toStdString());
            break;
    }
}
