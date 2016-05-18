#pragma once
#include"smart_object.h"

class GarbageCollector
{
public:
	static GarbageCollector* Instance(size_t m = 1024);
	static bool DeleteInstance();
	static void registration(SmartObject* obj, bool place);
	static void deRegistration(SmartObject* obj);
	static void collect();
	static size_t memoryUse();
	static size_t maxMemory();
	class Element
	{
	public:
		bool type;
		bool color;
		SmartObject* ptr;
	};
	static const std::vector<Element>& pointers();
private:
	static void dfs(SmartObject* v);
	GarbageCollector() {}
	virtual ~GarbageCollector() {}
	static GarbageCollector* self_;
	
	std::vector<Element> pointers_;
	size_t memory_;
	size_t maxMemory_;
};
