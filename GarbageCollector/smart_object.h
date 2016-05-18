#pragma once
#include<vector>

class SmartObject
{
	friend class GarbageCollector;
public:
	SmartObject();

	virtual std::vector<SmartObject*> pointers() const = 0;
	void* operator new(size_t size);
	void operator delete(void* obj);
	~SmartObject();
private:
	size_t size_;
};