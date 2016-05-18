#pragma once
#include"smart_object.h"

class Test : public SmartObject
{
public:
	Test(const int& i);
	int& get();
	std::vector<SmartObject*> pointers() const;
	std::vector<SmartObject*> smartptr;
private:
	int val_;
};