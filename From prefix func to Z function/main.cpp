#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    std::vector<int> prefix(n);
    for(size_t i = 0; i < n; ++i) {
        std::cin >> prefix[i];
    }
    std::vector<int> Z(n);
    for(size_t i = 1; i < n; ++i) {
        if (prefix[i] > 0) {
            Z[i - prefix[i] + 1] = prefix[i];
        }
    }
    Z[0] = n;
    int i = 1;
    while(i < n) {
        int t = i;
        if (Z[i] > 0) {
            for(int j = 1; j < Z[i]; ++j) {
                if(Z[i + j] > Z[j]) {
                    break;
                }
                Z[i + j] = std::min(Z[j], Z[i] - j);
                t = i + j;
            }
        }
        i = t + 1;
    }
    for(size_t i = 0; i < n; ++i) {
        std::cout << Z[i] << ' ';
    }
    std::cout << std::endl;
    return 0;
}