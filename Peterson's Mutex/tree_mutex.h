#pragma once
#include<iostream>
#include<cstdlib>
#include<vector>
#include<atomic>
#include<thread>
#include<array>

class tree_mutex
{
public:
	tree_mutex(size_t num_threads)
	{
		numThreads_ = num_threads;
		size_t leaves = 1;
		while (leaves < numThreads_)
		{
			leaves *= 2;
		}
		adder_ = leaves;
		tournamentTree_.resize(leaves);
		for (size_t i = 1; i < leaves; ++i) {
			tournamentTree_[i] = new PetersonMutex();
		}
	}
	void lock(size_t thread_index)
	{
		size_t leaveNum = thread_index + adder_;
		size_t mutexNumber = leaveNum / 2;
		while (mutexNumber >= 1) {
			tournamentTree_[mutexNumber]->lock(leaveNum % 2);
			leaveNum = mutexNumber;
			mutexNumber = mutexNumber / 2;
		}
	}
	void unlock(size_t thread_index)
	{
		size_t mutexNumber = 1;
		size_t target = thread_index + adder_;
		while (mutexNumber < target) {
			int boss = tournamentTree_[mutexNumber]->getBoss();
			if (boss != 3)
				tournamentTree_[mutexNumber]->unlock(boss);
			if (boss == 0)
				mutexNumber *= 2;
			else
				mutexNumber = mutexNumber * 2 + 1;
		}
	}
private:
	class PetersonMutex
	{
	public:
		PetersonMutex()
		{
			want_[0].store(false);
			want_[1].store(false);
			victim_.store(0);
			boss_.store(3);
		}

		void lock(int t)
		{
			want_[t].store(true);
			victim_.store(t);
			while (want_[1 - t].load() && victim_.load() == t) {
				std::this_thread::yield();
			}
			boss_.store(t);
		}

		void unlock(int t) 
		{
			want_[t].store(false);
			boss_.store(3);
		}

		int getBoss()
		{
			return boss_.load();
		}

	private:
		std::array<std::atomic<bool>, 2> want_;
		std::atomic<int> victim_;
		std::atomic<int> boss_;
	};
	size_t numThreads_;
	size_t adder_;
	std::vector<PetersonMutex*> tournamentTree_;
};