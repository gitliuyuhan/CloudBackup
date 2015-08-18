#!/usr/bin/env python
#coding=utf-8

import socket
import select
import Queue

#创建服务端socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#设置为非阻塞
server.setblocking(False)
#设置可重用端口
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#设置ip和端口并绑定
server_address = ('192.168.20.150', 12345)
server.bind(server_address)
#监听并设置监听队列大小
server.listen(10)

inputs = [server]
outputs = []
message_queues = {}

timeout = 20

result = []

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
            connection, client_address = s.accept()
            print " connection from ", client_address
            connection.setblocking(False)
            inputs.append(connection)
            message_queues[connection] = Queue.Queue()
        else:
            date = s.recv(1024)
            result.append(date)
            if date:
                print " received", date, "from", s.getpeername()
                message_queues[s].put(date)
                if s not in outputs:
                    outputs.append(s)
            else:
                print " closing", client_address
                if s in outputs:
                    outputs.remove(s)
                inputs.remove(s)
                s.close()
                del message_queues[s]
    #处理写事件集合
   # for s in writable:
   #     try:
   #         next_msg = message_queues[s].get_nowait()
   #     except Queue.Empty:
   #         print " ", s.getpeername(), "queue empty"
   #         outputs.remove(s)
   #     else:
   #         print "sending", next_msg, "to ", s.getpeername()
   #         s.send(next_msg)

    #处理异常
    for s in exceptional:
        print "exception condition on", s.getpeername()
        inputs.remove(s)
        if s in outputs:
            outputs.remove(s)
        s.close()
        del message_queues[s]
    print "date......................................... ", result


