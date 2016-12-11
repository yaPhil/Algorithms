#include <iostream>
#include "lock_free_queue.h"



int main() {
    lock_free_queue<int> que;
    int a;
    que.enqueue(10);
    que.enqueue(11);
    que.enqueue(12);
    que.enqueue(13);
    que.enqueue(14);
    que.enqueue(15);
    que.dequeue(a);
    std::cout << a << std::endl;
    que.dequeue(a);
    std::cout << a << std::endl;
    que.dequeue(a);
    std::cout << a << std::endl;
    return 0;
}