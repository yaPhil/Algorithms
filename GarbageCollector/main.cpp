#include<iostream>
#include<vector>
#include<exception>
#include<stdexcept>
#include"smart_object.h"
#include"garbage_collector.h"
#include"test.h"

void test(unsigned long long NUM_TEST)
{
    std::cout << "Start testing.." << std::endl << "Number of tests is " << NUM_TEST << std::endl;
    for (unsigned long long i = 0; i < NUM_TEST; ++i)
	{
		Test* a = new Test(i);
    }

    for (unsigned long long i = 0; i < NUM_TEST; ++i)
	{
        Test* a = new Test(i);
		Test* b = new Test(i);
		Test* c = new Test(i);
        if(!GarbageCollector::isCollect_)
        {
            a->smartptr.push_back((SmartObject*)b);
            b->smartptr.push_back((SmartObject*)c);
            c->smartptr.push_back((SmartObject*)a);
        }
        else
        {
            GarbageCollector::isCollect_ = false;
        }
    }
    std::cout << "All tests have done" << std::endl;
}

int main()
{
    unsigned long long numTest = 0;
    unsigned long long heapSize = 0;
    std::cout << "Inpput number of tests and max Heap size" << std::endl;
    std::cin >> numTest >> heapSize;
    GarbageCollector::Instance(heapSize);
    /*Test* b = new Test(10);
    Test* d = new Test(10);
    if(!GarbageCollector::isCollect_)
    {
        b->smartptr.push_back(d);
        b->smartptr.push_back(b);
        b->smartptr.push_back(b);
        b->smartptr.push_back(b);
        d->smartptr.push_back(b);
    }
    else
        GarbageCollector::isCollect_ = false;
    Test* e = new Test(1);
    if(!GarbageCollector::isCollect_)
        b->smartptr.push_back(e);
    else
        GarbageCollector::isCollect_ = false;
	std::cout << "Mem use: " << GarbageCollector::memoryUse() << std::endl;
	Test* c = new Test(15);
    std::cout << "Mem use: " << GarbageCollector::memoryUse() << std::endl;
    */
    test(numTest);
	GarbageCollector::DeleteInstance();
	return 0;
}
