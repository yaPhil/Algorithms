#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

int main() {
    int n;
    std::cin >> n;
    std::string str;
    std::cin >> str;
    std::vector<int> suffArray(n);
    for(size_t i = 0; i < n; ++i) {
        std::cin >> suffArray[i];
        suffArray[i]--;
    }
    std::vector<int> lcp(n);
    std::vector<int> pos(n);
    for(size_t i = 0; i < n; ++i) {
        pos[suffArray[i]] = i;
    }
    int k = 0;
    for(size_t i = 0; i < n; ++i) {
        if (k > 0) {
            k--;
        }
        if (pos[i] == n - 1) {
            lcp[n - 1] = -1;
            k = 0;
        } else {
            int j = suffArray[pos[i] + 1];
            while(std::max((int)i + k, j + k) < n && str[i + k] == str[j + k]) {
                k++;
            }
            lcp[pos[i]] = k;
        }
    }
    for(size_t i = 0; i < n - 1; ++i) {
        std::cout << lcp[i] << ' ';
    }
    std::cout << std::endl;
    return 0;
}