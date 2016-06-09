#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>
#include <vector>
#include <deque>
#include <condition_variable>

class ThreadPool; // forward declare

class Worker {
public:
    Worker(ThreadPool &s): pool(s) { }
    void operator()();
private:
    ThreadPool &pool;
};

class ThreadPool {
public:
    ThreadPool(size_t threads);
    template<class F> void enqueue(F f);
    ~ThreadPool();
private:
    friend class Worker;

    std::vector<std::thread> workers;
    std::deque<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable cond;
    bool stop;
};

#endif
