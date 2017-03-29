#include <iostream>
#include "geometry.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Vector a(1, 1, 1);
    Vector b(1, 1, 1);
    Vector c = a - b;
    return 0;
}