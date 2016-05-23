#include"test.h"
#include"garbage_collector.h"

Test::Test(const int& i)
{
	val_ = i;
}

int& Test::get()
{
	return val_;
}

std::vector<SmartObject*> Test::pointers() const
{
	return smartptr;
}
