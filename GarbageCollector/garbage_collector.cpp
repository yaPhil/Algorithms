#include<iostream>
#include"garbage_collector.h"
#include"test.h"

bool operator<(const GarbageCollector::Element& a, const GarbageCollector::Element& b)
{
    return a.ptr < b.ptr;
}

bool operator==(const GarbageCollector::Element& a, const GarbageCollector::Element& b)
{
    return a.ptr == b.ptr;
}

GarbageCollector* GarbageCollector::Instance(size_t m)
{
    if (self_ == nullptr)
	{
		self_ = new GarbageCollector();
		self_->memory_ = 0;
		self_->maxMemory_ = m;
        self_->isCollect_ = false;
        self_->stackObject_.insert(Element{true, NULL});
        self_->heapObject_.insert(Element{true, NULL});
        self_->heapObject_.erase(Element{true, NULL});
        self_->stackObject_.erase(Element{true, NULL});
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
   // std::cout << "GC registration " << obj->size_ << std::endl;
	if (self_->heapObject_.count(Element{ false, obj }) || self_->stackObject_.count(Element{ false, obj }))
		return;
	if (place == true)
		self_->stackObject_.insert(Element{ false, obj });
	else
		self_->heapObject_.insert(Element{ false, obj });
	self_->memory_ += obj->size_;
    self_->collect(obj);
}

void GarbageCollector::deRegistration(SmartObject* obj)
{
    //std::cout << "GC deregistration " << obj->size_ << std::endl;
	if (self_->heapObject_.count(Element{ false, obj }))
	{
			self_->memory_ -= obj->size_;
			self_->heapObject_.erase(Element{ false, obj });
	}
	if (self_->stackObject_.count(Element{ false, obj }))
	{
		self_->memory_ -= obj->size_;
		self_->stackObject_.erase(Element{ false, obj });
	}
    //self_->collect();
}

void GarbageCollector::collect(SmartObject* ign)
{
   // std::cout << "GC Collect " << std::endl;
    GarbageCollector::isCollect_ = true;
    std::vector<SmartObject*> ptr;
    //std::cout << "IF" << std::endl;
    //std::cout << self_->stackObject_.size()  << ' ' << self_->heapObject_.size()<< std::endl;
    if(self_->stackObject_.size() > 0)
    {
        for (auto i = self_->stackObject_.begin(); i != self_->stackObject_.end(); ++i)
        {
            if(i->ptr != ign)
            {
      //          std::cout << "GC stack perebor " << std::endl;
                dfs(i->ptr);
            }
        }
    }
    if(self_->heapObject_.size() > 0)
    {
        //std::cout << "collect clean" << std::endl;
        for(auto i = self_->heapObject_.begin(); i != self_->heapObject_.end(); ++i)
        {
            if(i->ptr != ign)
            {
                if (i->color == false)
                {
                    ptr.push_back(i->ptr);
                }
                else
                {
                    Element tmp{ false, i->ptr };
                    self_->heapObject_.erase(i);
                    self_->heapObject_.insert(tmp);
                }
            }
        }
        for(size_t i = 0; i < ptr.size(); ++i)
        {
            if(ptr[i] != ign)
            {
          //      std::cout << "collect kill " << ptr[i]->size_ << std::endl;
                delete ptr[i];
            }
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
