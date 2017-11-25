
#include "stdafx.h"
#pragma once  

#include <deque>  
#include <string>  
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;

// 使用C++11 语言规范实现的线程池： 面向对象做法，每一个job都是Task继承类的对象
namespace BX
{
    class Task  
    {  
    public:
        Task(void* arg = NULL, const std::string taskName = "")
            : arg_(arg)
            , taskName_(taskName)
        {
        }
        virtual ~Task()
        {
        }
        void setArg(void* arg)
        {
            arg_ = arg;
        }

        virtual int run() = 0;

    protected:
        void*       arg_;
        std::string taskName_;
    };


    class ThreadPool
    {
    public:
        ThreadPool(int threadNum = 4);
        ~ThreadPool();

    public:
		size_t Size();
        size_t addTask(Task *task);
        void   stop();
        int    size();
        Task*  take();

    private:
        int createThreads();
        static void* threadFunc(void * threadData);

    private:
        ThreadPool& operator=(const ThreadPool&);
        ThreadPool(const ThreadPool&);

    private:
        volatile  bool              isRunning_;
        int                         threadsNum_;

		std::vector<std::thread>    threads_;
        std::deque<Task*>           taskQueue_;
 
		std::mutex              mutex_;
		std::condition_variable_any m_notEmpty_condition_;



    };
}  
