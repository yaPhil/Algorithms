#include"smart_object.h"
#include"garbage_collector.h"
#include<iostream>
#include<string>
#include<cstdio>
#include<cstdlib>

SmartObject::SmartObject()
{
	if (GarbageCollector::Instance()->heapObject_.count(GarbageCollector::Element{ false, this }) == 0)
	{
		this->size_ = 0;
	}
    //std::cout << "Smart Obj const " << this->size_ << std::endl;

    GarbageCollector::registration(this, true);
}

void * SmartObject::operator new(size_t size)
{
    //std::cout << "SMO new " << size << std::endl;
	size_t curentMemory = GarbageCollector::memoryUse();
	size_t maxMemory = GarbageCollector::maxMemory();
	if (curentMemory + size > maxMemory)
	{
		GarbageCollector::collect();
		curentMemory = GarbageCollector::memoryUse();
		if (curentMemory + size > maxMemory)
		{
            throw(std::bad_alloc());
		}
	}
	SmartObject* obj = static_cast<SmartObject *>(malloc(size));
	obj->size_ = size;
    GarbageCollector::registration(obj, false);
	return obj;
}

void SmartObject::operator delete(void* obj)
{
    //std::cout << "SMO delete " << ((SmartObject*)obj)->size_ << std::endl;
	free(obj);
}

SmartObject::~SmartObject()
{
    //std::cout << "Smart Obj destruct " << this->size_ << std::endl;
    GarbageCollector::deRegistration(this);
}
