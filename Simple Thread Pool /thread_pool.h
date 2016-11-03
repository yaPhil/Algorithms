//
// Created by philipp on 03.11.16.
//
#pragma once

#include <future>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <exception>
#include <stdexcept>

template <class Value>
class WorkingThread
{
public:
    WorkingThread() :
        thread_ (&WorkingThread::threadFunction_, this),
        enabled_(true)
    {}
    size_t getTaskNumbers_() {
        //std::unique_lock<std::mutex> locker(mutex_);
        return taskQueue_.size();
    }

    bool isFree_() {
        //std::unique_lock<std::mutex> locker(mutex_);
        return taskQueue_.empty();
    }

    std::future<Value> addTask_(std::function<Value()> newTask) {
        //std::unique_lock<std::mutex> locker(mutex_);
        taskQueue_.push(std::move(std::packaged_task<Value()>(std::move(newTask))));
        //hasWork_.notify_one();
        return taskQueue_.back().get_future();
    }
    void kill() {
        enabled_ = false;
        hasWork_.notify_one();
        thread_.join();
    }
    ~WorkingThread() {
        enabled_ = false;
        hasWork_.notify_one();
        thread_.join();
    }
private:
    void threadFunction_() {
        while(enabled_) {
            //std::unique_lock<std::mutex> locker(mutex_);
            //hasWork_.wait(locker, [&]() { return !enabled_ || !taskQueue_.empty(); });
            while (!taskQueue_.empty()) {
                //locker.unlock();
                taskQueue_.front()();
                //locker.lock();
                taskQueue_.pop();
            }
        }
    }
    std::thread thread_;
    std::condition_variable hasWork_;
    std::mutex mutex_;
    bool enabled_;
    std::queue<std::packaged_task<Value()>> taskQueue_;
};

template <class Value>
class thread_pool
{
public:
    explicit thread_pool(size_t threadNumber) {
        open = true;
        for(size_t i = 0; i < threadNumber; ++i) {
            workingThreads_.push_back(new WorkingThread<Value>());
        }
    }
    std::future<Value> submit(std::function<Value()> function){
        if (!open)
            throw new std::runtime_error("Cant add task after shutdown");
        size_t minNumberTasks = 2000000000;
        WorkingThread<Value>* minThread = nullptr;
        for(size_t i = 0; i < workingThreads_.size(); ++i) {
            if (workingThreads_[i]->isFree_()) {
                return workingThreads_[i]->addTask_(function);
            }
            if (workingThreads_[i]->getTaskNumbers_() < minNumberTasks) {
                minNumberTasks = workingThreads_[i]->getTaskNumbers_();
                minThread = workingThreads_[i];
            }
        }
        return minThread->addTask_(function);
    }

    void shutdown()
    {
        open = false;
        for(size_t i = 0; i < workingThreads_.size(); ++i) {
            workingThreads_[i]->kill();
        }
    }

private:
    std::vector<WorkingThread<Value>*> workingThreads_;
    bool open;
};

