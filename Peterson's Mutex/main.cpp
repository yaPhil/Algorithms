#include<iostream>
#include"tree_mutex.h"

int main()
{
	tree_mutex mtx = tree_mutex(10);
	mtx.lock(0);
	mtx.unlock(0);
	return 0;
}