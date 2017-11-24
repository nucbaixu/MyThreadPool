#include "stdafx.h"
#include "Thradpool.h" 
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

namespace BX
{
    ThreadPool::ThreadPool(int threadNum)
    {
        isRunning_ = true;
        threadsNum_ = threadNum;
        createThreads();
    }

    ThreadPool::~ThreadPool()
    {
        stop();
        for(std::deque<Task*>::iterator it = taskQueue_.begin(); it != taskQueue_.end(); ++it)
        {
            delete *it;
        }
        taskQueue_.clear();
    }

    int ThreadPool::createThreads()
    {
        for (int i = 0; i < threadsNum_; i++)
        {
			std::thread tem(threadFunc,this);
			threads_.emplace_back(std::move(tem));
        }
        return 0;
    }

    size_t ThreadPool::addTask(Task *task)
    {
       
		mutex_.lock();
        taskQueue_.push_back(task);
        int size = taskQueue_.size();
		mutex_.unlock();
        
		m_notEmpty_condition_.notify_one();
        return size;
    }

	/*返回线程线程池队列大小*/
	size_t ThreadPool::Size()
	{
		mutex_.lock();
        int size = taskQueue_.size();
		mutex_.unlock();

		return size;
	}

    void ThreadPool::stop()
    {
        if (!isRunning_)
        {
            return;
        }

        isRunning_ = false;


        //唤醒全部的线程
		m_notEmpty_condition_.notify_all();

        for (int i = 0; i < threadsNum_; i++)
        {
			threads_[i].join();
        }


		threads_.clear();
    }

    int ThreadPool::size()
    {
      
		mutex_.lock();
        size_t size = taskQueue_.size();
		mutex_.unlock();
        return size;
    }

    Task* ThreadPool::take()
    {
        Task* task = NULL;
        while (!task)
        {
           
			mutex_.lock();

			while (taskQueue_.empty() && isRunning_)
            {
				m_notEmpty_condition_.wait(mutex_);
            }

            if (!isRunning_)
            {               
				mutex_.unlock();
                break;
            }
            else if (taskQueue_.empty())
            {               
				mutex_.unlock();
                continue;
            }

            assert(!taskQueue_.empty());
            task = taskQueue_.front();
            taskQueue_.pop_front();
			mutex_.unlock();          
        }
        return task;
    }

    void* ThreadPool::threadFunc(void* arg)
    {
		auto thid = this_thread::get_id();
        ThreadPool* pool = static_cast<ThreadPool*>(arg);
		
        while (pool->isRunning_)
        {
            Task* task = pool->take();
            if (!task)
            {
				std::cout<<"thread["<<thid<<" ] will exit" <<endl;
				// printf("thread %d will exit\n", 1);
                break;
            }

            assert(task);
            task->run();
        }
        return 0;
    }

}
