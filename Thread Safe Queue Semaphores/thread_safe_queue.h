//
// Created by philipp on 09.12.16.
//
#include "semaphore.h"
#include <deque>
#include <thread>
#include <exception>
#include <stdexcept>

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
    explicit thread_safe_queue(size_t capacity) :
            has_(0), remaind_(0), blocker_(1) {
        isWork_ = true;
        currentSize_ = 0;
        maxSize_ = capacity;
    }
    void enqueue(Value v) {
        blocker_.aquire();
        if(!isWork_) {
            throw std::runtime_error("i'm shutdowned");
        }
        while(true) {
            if (queue_.size() < maxSize_) {
                queue_.push_back(std::move(v));
                ++currentSize_;
                has_.release();
                break;
            } else {
                blocker_.release();
                remaind_.aquire();
                blocker_.aquire();
            }
        }
        blocker_.release();
    }
    void pop(Value& v) {
        blocker_.aquire();
        if(!isWork_ && queue_.size() == 0) {
            throw std::runtime_error("i'm shutdowned");
        }
        while(true) {
            if (queue_.size() > 0) {
                v = std::move(queue_.front());
                --currentSize_;
                queue_.pop_front();
                remaind_.release();
                break;
            } else {
                blocker_.realese();
                has_.aquire();
                blocker_.aquire();
            }
        }
        blocker_.release();
    }
    void shutdown() {
        isWork_ = false;
        remaind_.release();
        has_.release();
    }

private:
    semaphore has_;
    semaphore remaind_;
    semaphore blocker_;
    Container queue_;
    bool isWork_;
    int currentSize_;
    int maxSize_;
};
