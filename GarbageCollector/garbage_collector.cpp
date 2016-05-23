#include<iostream>
#include"garbage_collector.h"
#include"test.h"

GarbageCollector* GarbageCollector::Instance(size_t m)
{
    if (self_ == nullptr)
	{
		self_ = new GarbageCollector();
		self_->memory_ = 0;
		self_->maxMemory_ = m;
        self_->isCollect_ = false;
        std::cout << "Garbage collector initialized" << std::endl << "Total heap memory is " << m << " bytes" << std::endl;
	}
	return self_;
}

bool GarbageCollector::DeleteInstance()
{
    std::cout << "Deleting Garbage Collector" << std::endl;
    if (self_ != nullptr)
	{
		for(auto i = self_->heapObject_.begin(); i != self_->heapObject_.end(); ++i)
		{
			delete i->ptr;
		}
		std::cout << "Heap clear" << std::endl;
        /*std::cout << "Garbage collector is free!!" << std::endl;*/
		delete self_;
		return true;
	}
	return false;
}

void GarbageCollector::registration(SmartObject* obj, bool place)
{
	if (self_->heapObject_.count(Element{ false, obj }) || self_->stackObject_.count(Element{ false, obj }))
		return;
	if (place == true)
		self_->stackObject_.insert(Element{ false, obj });
	else
		self_->heapObject_.insert(Element{ false, obj });
	self_->memory_ += obj->size_;
}

void GarbageCollector::deRegistration(SmartObject* obj)
{
	if (self_->heapObject_.count(Element{ false, obj }))
	{
			self_->memory_ -= obj->size_;
			self_->heapObject_.erase(Element{ false, obj });
			return;
	}
	if (self_->stackObject_.count(Element{ false, obj }))
	{
		self_->memory_ -= obj->size_;
		self_->stackObject_.erase(Element{ false, obj });
		return;
	}
}

void GarbageCollector::collect()
{
    GarbageCollector::isCollect_ = true;
	for (auto i = self_->stackObject_.begin(); i != self_->stackObject_.end(); ++i)
	{
		dfs(i->ptr);
	}
	for(auto i = self_->heapObject_.begin(); i != self_->heapObject_.end(); ++i)
	{
		if (i->color == false)
		{
			delete i->ptr;
		}
		else
		{
			Element tmp{ false, i->ptr };
			self_->heapObject_.erase(i);
			self_->heapObject_.insert(tmp);
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

void GarbageCollector::dfs(SmartObject* v)
{
	std::vector<SmartObject*> sons = v->pointers();
	if (self_->stackObject_.count(Element{ true, v }) || self_->heapObject_.count(Element{ true, v }))
		return;
	if (self_->stackObject_.count(Element{ false, v }))
	{
		self_->stackObject_.erase(Element{ false, v });
		self_->stackObject_.insert(Element{ true, v });
	}

	if (self_->heapObject_.count(Element{ false, v }))
	{
		self_->heapObject_.erase(Element{ false, v });
		self_->heapObject_.insert(Element{ true, v });
	}
	
	for (size_t i = 0; i < sons.size(); ++i)
	{
		dfs(sons[i]);
	}
}

GarbageCollector* GarbageCollector::self_ = nullptr;
bool GarbageCollector::isCollect_ = false;
