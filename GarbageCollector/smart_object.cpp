#include"smart_object.h"
#include"garbage_collector.h"
#include<iostream>

SmartObject::SmartObject()
{
	std::vector<GarbageCollector::Element> ptr = GarbageCollector::pointers();
	size_t i = 0;
	while(i < ptr.size())
	{
		if (this == ptr[i].ptr)
		{
			break;
		}
		++i;
	}
	if (i == ptr.size())
	{
		this->size_ = 0;
		std::cout << "Stack obj!!!" << std::endl;
	}
	std::cout << "SmartObj const " << this->size_ << std::endl;

	GarbageCollector::Instance()->registration(this, 1);
}

void * SmartObject::operator new(size_t size)
{
	std::cout << "Operator New " << size << std::endl;
	size_t curentMemory = GarbageCollector::memoryUse();
	size_t maxMemory = GarbageCollector::maxMemory();
	if (curentMemory + size > maxMemory)
	{
		GarbageCollector::collect();
		curentMemory = GarbageCollector::memoryUse();
		if (curentMemory + size > maxMemory)
		{
			throw std::bad_alloc();
			return nullptr;
		}
	}
	SmartObject* obj = static_cast<SmartObject *>(malloc(size));
	obj->size_ = size;
	GarbageCollector::Instance()->registration(obj, false);
	return obj;
}

void SmartObject::operator delete(void* obj)
{
	std::cout << "SmartObj delete " << ((SmartObject*)obj)->size_ << std::endl;
	free(obj);
}

SmartObject::~SmartObject()
{
	std::cout << "SmartObj destruct " << this->size_ << std::endl;
	GarbageCollector::Instance()->deRegistration(this);
}