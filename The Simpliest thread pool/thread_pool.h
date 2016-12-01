//
// Created by philipp on 04.11.16.
//
#pragma once

#include <iostream>
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
class thread_pool {
public:
    explicit thread_pool(size_t threadNumber) {
        //std::cout << "INIT\n";
        open_ = true;
        for (size_t i = 0; i < threadNumber; ++i) {
            workingThreads_.push_back(std::thread(&thread_pool::taskSolving, this));
        }
        //std::cout << "INIT_end\n";
    }

    std::future<Value> submit(std::function<Value()> function) {
        //std::cout << "subm\n";
        if (!open_) {
            throw std::runtime_error("Cant add task after shutdown");
        }
        //std::cout << "subm_aft_if\n";
        std::unique_lock<std::mutex> locker(mutex_);
        tasksQueue_.emplace(std::packaged_task<Value()>(function));
        std::future<Value> returnFuture = tasksQueue_.back().get_future();
        locker.unlock();
        isEmpty_.notify_all();
        //std::cout << "after_subm\n";
        return returnFuture;
    }

    void shutdown() {
        //std::cout << "shut\n";
        open_ = false;
        isEmpty_.notify_all();
        for (size_t i = 0; i < workingThreads_.size(); ++i) {
            workingThreads_[i].join();
        }
        //std::cout << "shut_end\n";
    }
private:
    void taskSolving() {
        //std::cout << "solving\n";
        while(open_) {
            std::unique_lock<std::mutex> locker(mutex_);
            isEmpty_.wait(locker, [&](){ return !open_ || !tasksQueue_.empty(); });
            while (!tasksQueue_.empty()) {
                std::packaged_task<Value()> currentTask(std::move(tasksQueue_.front()));
                tasksQueue_.pop();
                locker.unlock();
                currentTask();
                locker.lock();
            }
            locker.unlock();
        }
        //std::cout << "solved\n";
    }
    std::vector<std::thread> workingThreads_;
    std::queue<std::packaged_task<Value()>> tasksQueue_;
    bool open_;
    std::condition_variable isEmpty_;
    std::mutex mutex_;
};
