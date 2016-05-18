#include<iostream>
#include"garbage_collector.h"
#include"test.h"

GarbageCollector* GarbageCollector::Instance(size_t m)
{
	if (!self_)
	{
		self_ = new GarbageCollector();
		self_->memory_ = 0;
		self_->maxMemory_ = m;
	}
	return self_;
}

bool GarbageCollector::DeleteInstance()
{
	if (self_)
	{
		size_t i = 0;
		while (i < self_->pointers_.size())
		{
			if (self_->pointers_[i].type == false)
				delete self_->pointers_[i].ptr;
			else
				++i;
		}
		delete self_;
		self_ = 0;
		return true;
	}
	return false;
}

void GarbageCollector::registration(SmartObject* obj, bool place)
{
	for (size_t i = 0; i < self_->pointers_.size(); ++i)
	{
		if (self_->pointers_[i].ptr == obj)
			return;
	}
	std::cout << "registration " << obj->size_ << std::endl;
	self_->pointers_.push_back(GarbageCollector::Element{ place, false, obj });
	self_->memory_ += obj->size_;
}

void GarbageCollector::deRegistration(SmartObject* obj)
{
	for (size_t i = 0; i < self_->pointers_.size(); ++i)
	{
		if (self_->pointers_[i].ptr == obj)
		{
			std::cout << "deRegistration " << self_->pointers_[i].ptr->size_ << std::endl;
			self_->memory_ -= self_->pointers_[i].ptr->size_;
			self_->pointers_.erase(self_->pointers_.begin() + i);
			break;
		}
	}
}

void GarbageCollector::collect()
{
	std::cout << "collect" << std::endl;
	for (size_t i = 0; i < self_->pointers_.size(); ++i)
	{
		if (self_->pointers_[i].type == true)
		{
			dfs(self_->pointers_[i].ptr);
		}
	}
	size_t i = 0;
	while (i < self_->pointers_.size())
	{
		if (self_->pointers_[i].color == false)
		{
			delete self_->pointers_[i].ptr;
		}
		else
		{
			self_->pointers_[i].color = false;
			++i;
		}
	}
}

size_t GarbageCollector::memoryUse()
{
	return self_->memory_;
}

size_t GarbageCollector::maxMemory()
{
	return self_->maxMemory_;
}

const std::vector<GarbageCollector::Element>& GarbageCollector::pointers()
{
	return self_->pointers_;
}

void GarbageCollector::dfs(SmartObject* v)
{
	std::vector<SmartObject*> sons = v->pointers();
	for (size_t i = 0; i < self_->pointers_.size(); ++i)
	{
		if (self_->pointers_[i].ptr == v)
		{
			if (self_->pointers_[i].color)
				return;
			self_->pointers_[i].color = true;
			break;
		}
	}
	for (size_t i = 0; i < sons.size(); ++i)
	{
		dfs(sons[i]);
	}
}

GarbageCollector* GarbageCollector::self_ = 0;