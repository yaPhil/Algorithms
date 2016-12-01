#pragma once
#include"smart_object.h"
#include<set>

class GarbageCollector
{
	friend class SmartObject;
public:
	static GarbageCollector* Instance(size_t m = 1024);
	static bool DeleteInstance();
	static void registration(SmartObject* obj, bool place);
	static void deRegistration(SmartObject* obj);
    static void collect(SmartObject* ign = nullptr);
	static size_t memoryUse();
	static size_t maxMemory();
    static bool isCollect_;

	class Element
	{
	public:
		bool color;
		SmartObject* ptr;
		friend bool operator<(const Element& a, const Element& b);
        friend bool operator==(const Element& a, const Element& b);
	};

private:
	static void dfs(SmartObject* v);
	GarbageCollector() {}
	virtual ~GarbageCollector() {}
	static GarbageCollector* self_;
	
	std::set<Element> stackObject_;
	std::set<Element> heapObject_;
	size_t memory_;
	size_t maxMemory_;
};


