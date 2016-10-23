#pragma once
#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<atomic>

class barrier
{
public:
	barrier(size_t numThreads) :
		numThreads_(numThreads),
		counterIn_(0),
		counterOut_(0)
	{}
	void enter()
	{
		std::unique_lock<std::mutex> locker(mutex_);
		while (counterOut_.load() > 0)
		{
			conditionIn_.wait(locker);
		}
		if (counterIn_.load() == numThreads_ - 1)
		{
			counterIn_.store(0);
			counterOut_.store(numThreads_);
			conditionOut_.notify_all();
		}
		else
		{
			counterIn_.fetch_add(1);
			while (counterOut_ == 0)
			{
				conditionOut_.wait(locker);
			}
		}
		counterOut_.fetch_sub(1);
		if (counterOut_.load() == 0)
		{
			conditionIn_.notify_all();
		}
		locker.unlock();
	}
private:
	size_t numThreads_;
	std::atomic<size_t> counterIn_;
	std::atomic<size_t> counterOut_;
	std::mutex mutex_;
	std::condition_variable conditionIn_;
	std::condition_variable conditionOut_;
};
