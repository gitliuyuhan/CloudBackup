#!/usr/bin/env python
#coding=utf-8

import socket
import select
import time
import threading
import re



#创建服务端socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#设置为非阻塞
server.setblocking(False)
#设置可重用端口
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#设置ip和端口并绑定
server_address = ('192.168.20.184', 12345)
server.bind(server_address)
#监听并设置监听队列大小
server.listen(10)

#事件集合
inputs = [server]
outputs = []

#子服务器connection
workServer_fd = []

#select 超时时间
timeout = 20

result = []

#保存所有子服务器的负载
mp = {}

#设置连接主服务器的客户端
host = '192.168.20.184'
port = 10000
addr = (host, port)
tcpMaster = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
tcpMaster.connect(addr)

#创建定时发送信号线程
def threadFunc():
    while 1:
        #将结果-->负载最轻的发送给主服务器
        if mp: 
            #给接受到的所有子服务器的负载进行排序
            sorted(mp.iteritems(), key = lambda d:d[1], reverse = False)
            
            #print ret1
            print '---------------------------------'
            print mp
            tmp = mp.keys()[0]
            m_ip = tmp[0]
            print m_ip
            m_port = tmp[1]
            print m_port
            ret1 = '{status:0, ip:' + str(m_ip) + ', port:' + str(m_port) + '}'
            print ret1
            print '---------------------------------'
            ret = 'hello world'
            #清空dict中的子服务器内容，准备下一次采集负载
            mp.clear();
            #发送给主服务器结果-->负载最轻的子服务器
            tcpMaster.send(ret1)
        
        #向集合中每个workerServer发送信号
        print "send signal"
        for fd in workServer_fd:
            #发送给子服务器信号
            fd.send("send signal")
        #定时为4秒
        time.sleep(5)

#创建线程，执行定时发送信号任务
tid = threading.Thread(target = threadFunc)
tid.start()


#提取数据
def parse(s, addr):
    #按照split分割
    t = s.split('split')
    print t[0]
    print t[1]
    print t[2]
    print t[3]
    #获得cpu使用率
    cre = re.search('ni,(.+?)id', t[0])
    ##cre = re.search('id', t[0])
    s1 = cre.group(1)
    ss1 = s1.split(' ')
    print ss1[1]
    i0 = float(ss1[1])
    #获得内存空闲大小
    mre = re.search('used,(.+?)free,', t[1])
    ##print mre.group(1)
    s2 = mre.group(1)
    ss2 = s2.split(' ')
    ##print ss2[3]
    i2 = int(ss2[3])
    #print i2
    #print type(i2)
    #i1 = int(mre.group(1))
    #获得带宽占用 rxkB/s txkB/s
    ss3 = t[2].split('     ')
    tp = []
    for i3 in range(len(ss3)):
        if ss3[i3] not in ' ':
           tp.append(ss3[i3]) 
    #    print i3,ss3[i3]
    #for ttt in tp:
    #    print ttt
    rx = float(tp[2])
    tx = float(tp[3])
    #print rx, tx
    #获得磁盘使用率
    s4 = t[3].split(' ')
    for i4 in range(len(s4)):
        print i4, s4[i4]
    ior = float(s4[23])
    iow = float(s4[31])

    print 'cpu:%lf mem:%d (rxkB:%lf txkB:%lf) (ior:%lf iow:%lf)' % (i0, i2, rx, tx, ior, iow)
    print addr
    mp[addr] = rx + tx + ior + iow
    print mp

#select事件集合，收集workerServer发送的负载内容，解析后发送给masterServer
while 1:
    #print 'waiting for next event'
    #得到select返回的3个事件集合，读写异常
    readable, writable, exceptional = select.select(inputs, outputs, inputs, timeout)
    #当时间到了，如果没有时间发生
    if not (readable or writable or exceptional):
        print 'time out'
        continue;
    #处理读事件集合
    for s in readable:
        #说明有连接加入
        if s is server:
            #获得连接和客户端ip
            connection, client_address = s.accept()
            #加入WorkServer集合中，定时向WorkServer发送信号
            workServer_fd.append(connection)
            #connection.send('send')
            print " connection from ", client_address
            #设置非阻塞
            connection.setblocking(False)
            inputs.append(connection)
        else:
            date = s.recv(1024)
            result.append(date)
            #print type(date) 
            parse(date, s.getpeername())
            #parse(date)
            if date:
                print " received date from ", s.getpeername()
                #print date
                if s not in outputs:
                    outputs.append(s)
            else:
                print " closing", client_address
                if s in outputs:
                    outputs.remove(s)
                inputs.remove(s)
                s.close()
                del message_queues[s]
    #处理异常
    for s in exceptional:
        print "exception condition on", s.getpeername()
        inputs.remove(s)
        if s in outputs:
            outputs.remove(s)
        s.close()

tid.join()
