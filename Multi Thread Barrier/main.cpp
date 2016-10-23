#include"barrier.h"

barrier b(2);

int main()
{
	std::thread t([&]() {	std::cout << "Son entering\n";
							b.enter(); 
							std::cout << "Son entering\n";
							b.enter(); 
							std::cout << "Son entering\n";
							b.enter();	
							std::cout << "Son entering\n";
							b.enter(); 
							std::cout << "Son entering\n";
							b.enter(); 
							std::cout << "Son entering\n";
							b.enter(); 
							std::cout << "Son entering\n";
							b.enter(); 
							std::cout << "Son entering\n";
							b.enter(); 
							std::cout << "Son entering\n";
							b.enter(); 
							std::cout << "Son entering\n";
							b.enter(); 
							std::cout << "Son entering\n";
							b.enter(); 
							return; });

	std::cout << "Father entering\n";
	b.enter();
	std::cout << "Father entering\n";
	b.enter();
	std::cout << "Father entering\n";
	b.enter();
	std::cout << "Father entering\n";
	b.enter();
	std::cout << "Father entering\n";
	b.enter();
	std::cout << "Father entering\n";
	b.enter();
	t.join();
	return 0;
}