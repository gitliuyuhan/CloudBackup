/*************************************************************************
    > File Name: epoll.hpp
    > Author: lewin
    > Mail: lilinhan1303@gmail.com
    > Company:  Xiyou Linux Group
    > Created Time: 2015年07月27日 星期一 08时40分51秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<cstdlib>
#include<cerrno>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<thread>
#include<list>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<atomic>

const int MaxTaskCount = 1000;
template<typename T>
class SyncQueue{
    public:
        SyncQueue(int m_maxSize) : m_maxSize(m_maxSize) , m_stop(false) {}
        ~SyncQueue(){}

        void Put(const T &x ) {
            Add(x);
        }

        void Put( T && x)  {
            Add(std::forward<T>(x));
        }

        void Take(std::list<T> & list) {
            std::unique_lock<std::mutex> locker(m_mutex);
            m_notEmpty.wait( locker , [this]{return m_stop || NotEmpty();});
            if( m_stop ) {
                return;
            }
            list = std::move(m_queue);
            m_notFull.notify_one();
        }

        void Take( T & t ) {
            std::unique_lock<std::mutex> locker(m_mutex);
            m_notEmpty.wait( locker , [this]{ return m_stop || NotEmpty();});
            if(m_stop)  {
                return;
            }
            t = m_queue.front();
            m_queue.pop_front();
            m_notFull.notify_one();
        }

        void Stop()  {
            {
                std::lock_guard<std::mutex> locker(m_mutex);
                m_stop = true;
            }
            m_notFull.notify_all();
            m_notEmpty.notify_all();
        }

        bool Empty(){
            std::lock_guard<std::mutex> locker(m_mutex);
            return m_queue.size();
        }

        bool Full()  {
            std::lock_guard<std::mutex> locker(m_mutex);
            return m_queue.size() == m_maxSize;
        }

        size_t Size() {
            std::lock_guard<std::mutex> locker(m_mutex);
            return m_queue.size();
        }

        int Count()  {
            return m_queue.size();
        }

    private:
        bool NotFull()  {
            bool full = m_queue.size() >= m_maxSize;
            if( full )  {
                std::cout << " The Syncqueue is full ..." << std::endl;
            }
            return !full;
        }

        bool NotEmpty()  {
            bool empty = m_queue.empty();
            if( empty )  {
                std::cout << "The Syncqueue is Empty..." << std::endl;
            }
            return !empty;
        }

        template<typename F>
        void Add( F && x ) {
            std::unique_lock<std::mutex> locker(m_mutex);
            m_notFull.wait(locker,[this]{return m_stop || NotFull();});
            if( m_stop )  {
                return;
            }
            m_queue.push_back(std::forward<F>(x));
            m_notEmpty.notify_one();
        }

        std::list<T> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_notEmpty;
        std::condition_variable m_notFull;
        int m_maxSize;
        bool m_stop;
};

class ThreadPool{
    public:
        using Task = std::function<void()>;
        ThreadPool(int numThreads = std::thread::hardware_concurrency()):m_queue(MaxTaskCount)  {
            Start(numThreads);
        }

        ~ThreadPool(void) {
            StopThreadGroup();
        }


        void AddTask(const Task & t) {
            m_queue.Put(t);
        }

        void Start(int numThreads)  {
            m_running = true;
            for(int i = 0 ; i < numThreads ; ++i ) {
                m_ThreadGroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread , this));
            }
        }

        void RunInThread() {
            while(m_running)  {
                std::list<Task> list;
                m_queue.Take(list);
                for( auto & Task : list )  {
                    if(!m_running)  {
                        return;
                    }
                    Task();
                }
            }
        }

        void StopThreadGroup()  {
            m_queue.Stop();
            m_running = false;

            for( auto thread : m_ThreadGroup )  {
                if(thread)  {
                    thread->join();
                }
                m_ThreadGroup.clear();
            }
        }

        void Stop()
        {
            std::call_once(m_flag,[this]{StopThreadGroup();});
        }

        std::list<std::shared_ptr<std::thread>> m_ThreadGroup;
        SyncQueue<Task> m_queue;
        std::atomic_bool m_running;
        std::once_flag m_flag;
};
