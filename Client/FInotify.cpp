/*======================================================
    > File Name: FInotify.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年10月24日 星期六 20时39分54秒
 =======================================================*/

#include"myinclude.h"
#include"finotify.h"

#define LEN  (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

FInotify :: FInotify(QObject* parent) : QObject(parent)
{
    inotifyFd = inotify_init(); 
    if(inotifyFd == -1)
        printf("inotify init error!\n");
    else
        InotifyStart();
}

void* FInotify::InotifyThread(void* arg)
{
    FInotify*                p = (FInotify*)arg;
    ssize_t                  numRead;
    char                     buf[LEN];
    char*                    q;
    struct inotify_event*    event;

    for(;;)
    {
        numRead = read(p->inotifyFd,buf,LEN);
        if(numRead > 0)
        {
            for(q = buf;q < buf+numRead;)
            {
                event = (struct inotify_event*)q;
                p->InotifyEvent(event);
                q += sizeof(struct inotify_event) + event->len;
            }
        }
    }
}

void FInotify::InotifyStart()
{
    pthread_t       thid;
    if(!pthread_create(&thid,NULL,InotifyThread,this))
    {
        //线程结束后自动释放资源
        pthread_detach(thid);
        cout<<"start inotify"<<endl;
    }
    else
    {
        cout<<"Monitor thread created error!"<<endl;
    }
}

void FInotify::InotifyEvent(struct inotify_event*  i)
{
    map<int,string>::iterator    it;
    it = inotifyRunlist.find(i->wd);  
    if(it == inotifyRunlist.end())
        return;
    string file;
    file = it->second + "/" + i->name;

    //过滤
    //如果是隐藏文件
    string  tmpname = i->name;
    if(tmpname[0] == '.' || tmpname == "4913" || tmpname[tmpname.size()-1]=='~')
        return;

    //旧文件
    string  oldfile = "./etc/lyh" + getRoadFolder(it->second) + "/" + tmpname;

    //命令
    string  command;

    //更新变更记录
    ofstream   out;
    out.open("./etc/lyh/alter.txt",ios::app);

    //时间秒数
    time_t     t;
    time(&t);
    //时间转字符串
    char  nowtime[20];
    sprintf(nowtime,"%ld",t);
    
    int   ret; 
    string  type,dsc,dst;

    if( (i->mask & IN_MODIFY) && (i->len > 0 ) )
    {
        cout<<"修改文件："<<file<<endl;
        command = "cp " + oldfile + " " + "./etc/lyh/history/" + nowtime;
        ret = system(command.c_str());
        command = "cp " + file + " " + oldfile;
        ret = system(command.c_str());

        type = "modify";
        dsc = file;
        dst = nowtime;
        out<<type<<" "<<dsc<<" "<<dst<<endl;
        emit(VersionAlterSig(type,dsc,dst));

//        out<<"modify "<<file<<" "<<nowtime<<endl;
    }

    if((i->mask & IN_CREATE) && (i->len > 0 ))
    {
        if(i->mask & IN_ISDIR)
            cout<<"创建目录："<<file<<endl;
        else
            cout<<"创建文件："<<file<<endl;
        command = "cp " + file + " " + oldfile;
        ret = system(command.c_str());

        type = "add";
        dsc = it->second;
        dst = file;
        out<<type<<" "<<dsc<<" "<<dst<<endl;
        emit(VersionAlterSig(type,dsc,dst));

//        out<<"add "<<it->second<<" "<<file<<endl;
    }
    if((i->mask & IN_DELETE) && (i->len > 0))
    {
        if(i->mask & IN_ISDIR)
            cout<<"删除目录："<<file<<endl;
        else
            cout<<"删除文件："<<file<<endl;
        command = "mv " + oldfile + " " + "./etc/lyh/recycle/" + nowtime;
        ret = system(command.c_str());

        type = "delete";
        dsc = file;
        dst = nowtime;
        out<<type<<" "<<dsc<<" "<<dst<<endl;
        emit(VersionAlterSig(type,dsc,dst));

//        out<<"delete "<<file<<" "<<nowtime<<endl;           
    }
    out.close();
}

void FInotify::InotifyListAdd(string str,InotifyFilter  ft)
{
    this->inotifyList.insert(pair<string,InotifyFilter>(str,ft));
}

void FInotify::InotifyAdd(string s)
{
    int           wd;
    wd = inotify_add_watch(inotifyFd,s.c_str(),IN_ALL_EVENTS);
    if(wd != -1)
    {
        inotifyRunlist.insert(pair<int,string>(wd,s));
    }
}

void  FInotify::InotifyRm(string  s)
{
    int                          wd;
    map<int,string>::iterator    it;
    for(it = inotifyRunlist.begin();it != inotifyRunlist.end();it++)
    {
        if(it->second == s)
        {
            wd = it->first;
            break;
        }
    }

    if(inotify_rm_watch(inotifyFd,wd)==0)
    {
        s = it->second;
        inotifyRunlist.erase(it);
        InotifyListRm(s);
    }
}

void FInotify::InotifyListRm(string s)
{
    map<string,InotifyFilter>::iterator  it;
    it = inotifyList.find(s);
    if(it != inotifyList.end())
    {
        inotifyList.erase(it);
    }
}
