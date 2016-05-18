#include<iostream>
#include<vector>
#include<exception>
#include<stdexcept>
#include"smart_object.h"
#include"garbage_collector.h"
#include"test.h"

const int NUM_TEST = 500;

void test()
{
	for (int i = 0; i < NUM_TEST; ++i)
	{
		Test* a = new Test(i);
	}

	for (int i = 0; i < NUM_TEST; ++i)
	{
		Test*a = new Test(i);
		Test* b = new Test(i);
		Test* c = new Test(i);
		a->smartptr.push_back(b);
		b->smartptr.push_back(c);
		c->smartptr.push_back(a);
	}
}

int main()
{
	GarbageCollector::Instance(5000);
	Test a(5);
	Test* b = new Test(10);
	Test* d = new Test(10);
	Test* e = new Test(1);
	std::cout << "Mem use: " << GarbageCollector::memoryUse() << std::endl;
	Test* c = new Test(15);
	std::cout << "Mem use: " << GarbageCollector::memoryUse() << std::endl;
	test();
	GarbageCollector::DeleteInstance();
	return 0;
}