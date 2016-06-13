#include <thread>
#include <mutex>
#include <vector>
#include <deque>
#include <condition_variable>

#include "threadpool.h"

void Worker::operator()()
{
    std::function<void()> task;
    while (true)
    {
        std::unique_lock<std::mutex> locker(pool.queue_mutex);
        pool.cond.wait(locker, [this](){ return !pool.tasks.empty() || pool.stop; });
        if (pool.stop)
        {
          locker.unlock();
          return;
        }
        task = pool.tasks.front();
        pool.tasks.pop_front();
        locker.unlock();
        task();
        //notify condition_variable
        pool.notifier->notify_one();
    }
}

void ThreadPool::setNotifier(std::condition_variable* c){
    notifier = c;
}

ThreadPool::ThreadPool(size_t threads): stop(false)
{
    for (size_t i = 0; i < threads; ++i)
        workers.push_back(std::thread(Worker(*this)));
}

ThreadPool::~ThreadPool()
{
    stop = true; // stop all threads
    cond.notify_all();
    for (auto &thread: workers)
        thread.join();
}

// template<class F>
// void ThreadPool::enqueue(F f)
// {
//     std::unique_lock<std::mutex> lock(queue_mutex);
//     tasks.push_back(std::function<void()>(f));
//     lock.unlock();
//     cond.notify_one();
// }
