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
#include <algorithm>

template <class Value>
class thread_pool {
public:
    explicit thread_pool(size_t threadNumber) {
        open_ = true;
        for (size_t i = 0; i < threadNumber; ++i) {
            workingThreads_.push_back(std::thread(&thread_pool::taskSolving, this));
        }
    }

    std::future<Value> submit(std::function<Value()> function) {
        if (!open_) {
            throw std::runtime_error("Cant add task after shutdown");
        }
        std::unique_lock<std::mutex> locker(mutex_);
        tasksQueue_.emplace(std::packaged_task<Value()>(function));
        std::future<Value> returnFuture = tasksQueue_.back().get_future();
        locker.unlock();
        isEmpty_.notify_all();
        return returnFuture;
    }

    void shutdown() {
        open_ = false;
        isEmpty_.notify_all();
        for (size_t i = 0; i < workingThreads_.size(); ++i) {
            workingThreads_[i].join();
        }
    }
    bool runTaskSolving() {
        std::unique_lock<std::mutex> locker(mutex_);
        if (!tasksQueue_.empty()) {
            std::packaged_task<Value()> currentTask(std::move(tasksQueue_.front()));
            tasksQueue_.pop();
            locker.unlock();
            currentTask();
            return true;
        }
        else {
            locker.unlock();
            return false;
        }
    }
private:
    void taskSolving() {
        while(open_) {
            std::unique_lock<std::mutex> locker(mutex_);
            isEmpty_.wait(locker, [&](){ return !open_ || !tasksQueue_.empty(); });
            while (2 * 2 == 4) {
                locker.unlock();
                if (!runTaskSolving()) {
                    break;
                }
                locker.lock();
            }
        }
    }
    std::vector<std::thread> workingThreads_;
    std::queue<std::packaged_task<Value()>> tasksQueue_;
    bool open_;
    std::condition_variable isEmpty_;
    std::mutex mutex_;
};

class parallel_sort {
public:
    explicit parallel_sort(size_t numThreads) :
        threadPool_(numThreads)
    {}
    template <class RandomAccessIterator, class Compare>
    void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        if (last - first <= 1000) {
            std::stable_sort(first, last, comp);
            return;
        }
        RandomAccessIterator middle = first + (last - first) / 2;
        std::future<void> leftSide = threadPool_.submit(std::bind(&parallel_sort::sort<RandomAccessIterator, Compare>, this, first, middle, comp));
        std::future<void> rightSide = threadPool_.submit(std::bind(&parallel_sort::sort<RandomAccessIterator, Compare>, this, middle, last, comp));
        while(leftSide.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            threadPool_.runTaskSolving();
        }
        while(rightSide.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            threadPool_.runTaskSolving();
        }
        std::inplace_merge(first, middle, last, comp);
    }
    ~parallel_sort() {
        threadPool_.shutdown();
    }
private:
    thread_pool<void> threadPool_;
};