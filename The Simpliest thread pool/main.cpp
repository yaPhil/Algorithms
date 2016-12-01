#include <iostream>
#include "thread_pool.h"

int main() {
    thread_pool<int> thr(1);
    auto ftr1 = thr.submit([](){return 42;});
    auto ftr2 = thr.submit([](){return 111;});
    thr.shutdown();
    std::cout << ftr1.get() << ' ' << ftr2.get();
    return 0;
}