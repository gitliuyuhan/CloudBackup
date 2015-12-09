/*======================================================
    > File Name: finotify.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年10月22日 星期四 18时58分37秒
 =======================================================*/

#ifndef FINOTIFY_H
#define FINOTIFY_H

#include<QApplication>

#include<sys/inotify.h>
#include<limits.h>

#include"myinclude.h"

typedef struct InotifyFilter
{
    string      startdate;     //开始日期
    string      enddate;       //结束日期
    string      suffix;        //后缀
    string      startsize;     //起始文件大小
    string      endsize;       //文件大小
    int         isfolder;      //目录
    int         isfile;        //普通文件
    string      idate;         //文件日期
}InotifyFilter;

class FInotify : public QObject
{
    Q_OBJECT
public:
    //初始化inotify
    FInotify(QObject* parent = 0);
    //添加待监控列表
    void InotifyListAdd(string,InotifyFilter);
    //从监控列表移除
    void InotifyListRm(string);
    //删除监控项
    void InotifyRm(string);
    //增加监控项
    void InotifyAdd(string);
    //开始监控
    void InotifyStart();
    //监控线程
    static void* InotifyThread(void*);
    //监控事件处理
    void InotifyEvent(struct inotify_event*);
signals:
    void VersionAlterSig(string,string,string);
private:
    int                          inotifyFd;       //文件描述符
    map<string,InotifyFilter>    inotifyList;     //监控列表
    map<int,string>              inotifyRunlist;  //正在监控  
};


#endif //FINOTIFY_H
