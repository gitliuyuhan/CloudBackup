/*======================================================
    > File Name: AddMonitorWidget.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年09月28日 星期一 15时55分25秒
 =======================================================*/

#include"addmonitorwidget.h"

#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFileIconProvider>
#include<QPixmap>
#include<QIcon>
#include<QCursor>
#include<QDateTime>

extern QString userName;

AddMonitorWidget::AddMonitorWidget(QWidget* parent):QWidget(parent)
{
    filterlabel = new QLabel(tr("过滤"));
    listwidget = new QListWidget;

    QHBoxLayout*   rowlayout1 = new QHBoxLayout;
    rowlayout1->addWidget(filterlabel);
    QHBoxLayout*   rowlayout2 = new QHBoxLayout;
    rowlayout2->addWidget(new QLabel(tr("日期：")));
    rowlayout2->addWidget(new QLabel(tr("从")));
    startdate = new QLineEdit;
    startdate->setStyleSheet("max-width: 150;");
    rowlayout2->addWidget(startdate);
    rowlayout2->addWidget(new QLabel(tr("到")));
    enddate = new QLineEdit;
    enddate->setStyleSheet("max-width: 150;");
    rowlayout2->addWidget(enddate);
    rowlayout2->addStretch();
    QHBoxLayout*  rowlayout3 = new QHBoxLayout;
    rowlayout3->addWidget(new QLabel(tr("后缀：")));
    rowlayout3->addWidget(new QLabel(tr(".")));
    suffix = new QLineEdit;
    suffix->setStyleSheet("max-width: 150;");
    rowlayout3->addWidget(suffix);
    rowlayout3->addStretch();
    QHBoxLayout*  rowlayout4 = new QHBoxLayout;
    rowlayout4->addWidget(new QLabel(tr("大小：")));
    rowlayout4->addWidget(new QLabel(tr("从")));
    startsize = new QLineEdit;
    startsize->setStyleSheet("max-width: 150;");
    endsize = new QLineEdit;
    endsize->setStyleSheet("max-width: 150;"); 
    rowlayout4->addWidget(startsize);
    rowlayout4->addWidget(new QLabel(tr("到")));
    rowlayout4->addWidget(endsize);
    rowlayout4->addStretch();
    QHBoxLayout*  rowlayout5 = new QHBoxLayout;
    folderCBox = new QCheckBox(tr("目录"));
    fileCBox = new QCheckBox(tr("文件"));
    rowlayout5->addWidget(folderCBox);
    rowlayout5->addWidget(fileCBox);
    rowlayout5->addStretch();
    QHBoxLayout*  rowlayout6 = new QHBoxLayout;
    QPushButton*  okbutton = new QPushButton(tr("保存"));
    QPushButton*  cancelbutton = new QPushButton(tr("取消"));
    rowlayout6->addWidget(cancelbutton);
    rowlayout6->addWidget(okbutton);
    QVBoxLayout*  mainlayout = new QVBoxLayout;
    mainlayout->addLayout(rowlayout1);
    mainlayout->addLayout(rowlayout2);
    mainlayout->addLayout(rowlayout3);
    mainlayout->addLayout(rowlayout4);
    mainlayout->addLayout(rowlayout5);
    mainlayout->addWidget(listwidget);
    mainlayout->addLayout(rowlayout6);
    this->setLayout(mainlayout);

    this->ShowFolder("/home/lyh");

    connect(listwidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(ShowFolderSlot(QListWidgetItem*)));
    connect(cancelbutton,SIGNAL(clicked()),this,SLOT(close()));
    connect(okbutton,SIGNAL(clicked()),this,SLOT(okSlot()));
}

FileItem::FileItem(QWidget* parent):QWidget(parent)
{
}

void FileItem::setItem(QString name,QString par)
{
    this->parentDir = par;
    string    s=(par + name ).toStdString();
    
    struct stat   buf;

    stat(s.c_str(),&buf);
    //文件大小
    off_t     size=buf.st_size;
    //时间数
    time_t    timeoff=buf.st_mtime;
    //文件时间
    string    time=ctime(&timeoff);
    //文件类型
    mode_t    mode=buf.st_mode;
    
    selectCBox = new QCheckBox("");
    selectCBox->setStyleSheet("max-width:15;");
    
    QFileIconProvider  icon_provider;
    QPixmap            filePixmap;
    if(S_ISDIR(mode))
    {
        QIcon  iconfolder = icon_provider.icon(QFileIconProvider::Folder);
        filePixmap = iconfolder.pixmap(QSize(28,28));
    }
    else
    {
        QIcon  iconfile = icon_provider.icon(QFileIconProvider::File);
        filePixmap = iconfile.pixmap(QSize(28,28));
    }
    namelabel = new QLabel;
    iconlabel = new QLabel;
    iconlabel->setPixmap(filePixmap);
    iconlabel->setStyleSheet("max-width:30;");
    namelabel->setText(name);
    char   sizebuf[16];
    sprintf(sizebuf,"%ld",size);
    string   str=sizebuf;

    sizelabel = new QLabel(QString::fromStdString(str));
    datelabel = new QLabel(QString::fromStdString(time));

    QHBoxLayout   *mainlayout = new QHBoxLayout;
    mainlayout->addWidget(selectCBox);
    mainlayout->addWidget(iconlabel);
    mainlayout->addWidget(namelabel);
    mainlayout->addWidget(sizelabel);
    mainlayout->addWidget(datelabel);

    this->setLayout(mainlayout);

    connect(selectCBox,SIGNAL(stateChanged(int)),this,SLOT(stateChangedSlot(int)));
    
}

void FileItem::stateChangedSlot(int state)
{
    QString   file = this->parentDir + this->namelabel->text();
    emit SaveSig(state,file);
}

void AddMonitorWidget::okSlot()
{
    QDateTime time = QDateTime::currentDateTime();
    QString  stime = time.toString("yyyy-MM-dd|hh:mm:ss|ddd");

    ofstream   out;
    out.open("./etc/lyh/record.txt",ios::app);
    set<string>::iterator it;
    for(it = fileSet.begin();it != fileSet.end();it++)
    {
        cout<<*it<<endl;
        //文件名
        out<<*it<<" ";

        if(startdate->text() == "")
        {
            out<<"null"<<" ";
            filter.startdate = "null";
        }
        else
        {
            out<<(startdate->text()).toStdString()<<" ";
            filter.startdate = (startdate->text()).toStdString();
        }
        if(enddate->text() == "")
        {
            out<<"null"<<" ";
            filter.enddate = "null";
        }
        else
        {
            out<<(enddate->text()).toStdString()<<" ";
            filter.enddate = (enddate->text()).toStdString();
        }
        if(suffix->text() == "")
        {
            out<<"null"<<" ";
            filter.suffix = "null";
        }
        else
        {
            out<<(suffix->text()).toStdString()<<" ";
            filter.suffix = (suffix->text()).toStdString();
        }
        if(startsize->text() == "")
        {
            out<<"null"<<" ";
            filter.startsize = "null";
        }
        else
        {
            out<<(startsize->text()).toStdString()<<" ";
            filter.startsize = (startsize->text()).toStdString();
        }
        if(endsize->text() == "")
        {
            out<<"null"<<" ";
            filter.endsize = "null";
        }
        else
        {
            out<<(endsize->text()).toStdString()<<" ";
            filter.endsize = (endsize->text()).toStdString();
        }
        out<<folderCBox->isChecked()<<" ";
        filter.isfolder = folderCBox->isChecked();
        out<<fileCBox->isChecked()<<" ";
        filter.isfile = fileCBox->isChecked();

        out<<stime.toStdString()<<"\n";
        filter.idate = stime.toStdString();

        //拷贝监控文件的副本
        string  command = "cp -r " + (*it) + "  ./etc/lyh" + getRoadFolder(*it);
        system(command.c_str()); 

        emit(AddMonitorItemSig(*it,filter));
    }
    out.close();

    this->close();
}

void AddMonitorWidget::ShowFolder(string path)
{
    DIR  *dirp=NULL;
    dirp = opendir(path.c_str());
    if(dirp == NULL)
        return ;
    struct dirent  *dp;
    listwidget->clear();
    string     name;
    string     par;
    
    if(path != "/")
    {
        par = path + "/";
    }

    while(dp = readdir(dirp))
    {
        name = dp->d_name;
        if(name != "." && name != ".." && name[0] == '.')
            continue;
        QListWidgetItem*  item = new QListWidgetItem;
        item->setSizeHint(QSize(0,40));
        FileItem    *itemWidget = new FileItem(this);
        itemWidget->setItem(QString::fromStdString(name),QString::fromStdString(par));
        if(name == ".")
            listwidget->insertItem(0,item);
        else if(name == "..")
            listwidget->insertItem(1,item);
        else
            listwidget->addItem(item); 
        listwidget->setItemWidget(item,itemWidget);
        connect(itemWidget,SIGNAL(SaveSig(int,QString)),this,SLOT(SaveSlot(int,QString)));
    }
}

void AddMonitorWidget::ShowFolderSlot(QListWidgetItem*  item)
{
    FileItem*  w = (FileItem*)(listwidget->itemWidget(item));
    QString path = w->parentDir + w->namelabel->text();
    this->ShowFolder(path.toStdString());
}

void AddMonitorWidget::SaveSlot(int state,QString file)
{
    if(state == 2)
    {
        fileSet.insert(file.toStdString());
        cout<<state<<" "<<file.toStdString()<<endl;
    }
    if(state == 0)
    {
        fileSet.erase(file.toStdString());
        cout<<state<<" "<<file.toStdString()<<endl;
    }
}

