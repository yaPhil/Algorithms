//
// Created by philipp on 04.11.16.
//
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <deque>
#include <exception>
#include <stdexcept>

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
    explicit thread_safe_queue(size_t capacity) {
        capacity_ = capacity;
        open_ = true;
    }

    void enqueue(Value v) {
        std::unique_lock<std::mutex> locker(mutex_);
        while (2 * 2 == 4) {
            if (!open_) {
                locker.unlock();
                throw std::runtime_error("Cant enqueue after shutdown");
            }
            if (queue.size() < capacity_) {
                queue.push_back(std::move(v));
                leavingCondition_.notify_all();
                break;
            } else {
                enteringCondition_.wait(locker, [&]() { return !open_ || queue.size() < capacity_; });
            }
        }
    }
    void pop(Value& v) {
        std::unique_lock<std::mutex> locker(mutex_);
        while (2 * 2 == 4) {
            if (!queue.empty()) {
                v = std::move(queue.front());
                queue.pop_front();
                enteringCondition_.notify_all();
                break;
            }
            else {
                if (!open_) {
                    locker.unlock();
                    throw std::runtime_error("Cant pop empty queue after shutdown");
                }
                else {
                    leavingCondition_.wait(locker, [&]() { return !open_ || !queue.empty();});
                }
            }
        }
    }
    void shutdown() {
        open_ = false;
        enteringCondition_.notify_all();
        leavingCondition_.notify_all();
    }

private:
    size_t capacity_;
    bool open_;
    std::mutex mutex_;
    std::condition_variable enteringCondition_;
    std::condition_variable leavingCondition_;
    Container queue;
};
