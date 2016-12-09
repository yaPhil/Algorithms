//
// Created by philipp on 09.12.16.
//
#include <mutex>
#include <condition_variable>
#include <atomic>

class bathroom
{
public:
    bathroom()
    {
        manFlag_.store(false);
        girlFlag_.store(false);
        counter_ = 0;
    }
    void enter_male()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while(girlFlag_.load()) {
            waiter_.wait(lock);
        }
        manFlag_.store(true);
        counter_++;
        lock.unlock();
    }
    void leave_male()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        --counter_;
        if (counter_ == 0){
            manFlag_.store(false);
        }
        waiter_.notify_all();
    }
    void enter_female()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (manFlag_.load()) {
            waiter_.wait(lock);
        }
        girlFlag_.store(true);
        counter_++;
        lock.unlock();
    }
    void leave_female()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        --counter_;
        if (counter_ == 0){
            girlFlag_.store(false);
        }
        waiter_.notify_all();
    }
private:
    std::atomic<bool> manFlag_;
    std::atomic<bool> girlFlag_;
    std::condition_variable waiter_;
    std::mutex mutex_;
    std::size_t counter_;
};
