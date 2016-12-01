#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<int> zFunction(std::string &str) {
    std::vector<int> zFunction(str.size());
    int left = 0;
    int right = 0;
    zFunction[0] = str.size();
    for (size_t i = 1; i < str.size(); ++i) {
        zFunction[i] = std::max(0, std::min(right - (int)i, zFunction[(int)i - left]));
        while (i + zFunction[i] < str.size() && str[zFunction[i]] == str[i + zFunction[i]]) {
            ++zFunction[i];
        }
        if (i + zFunction[i] >= right) {
            left = i;
            right = i + zFunction[i];
        }
    }
    return zFunction;
}

std::vector<int> prefFunction(std::string &str) {
    std::vector<int> piFunction(str.size());
    for (size_t i = 1; i < str.size(); ++i) {
        int j = piFunction[i - 1];
        while (j > 0 && str[i] != str[j]) {
            j = piFunction[j - 1];
        }
        if (str[i] == str[j]) {
            ++j;
        }
        piFunction[i] = j;
    }
    return piFunction;
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::string number;
    std::cin >> number;
    if(number[0] == '1') {
        std::cout << "1.(0)";
        return 0;
    }
    std::string reverseString = number.substr(2, number.size() - 2);
    std::reverse(reverseString.begin(), reverseString.end());
    std::vector<int> piFunction = prefFunction(reverseString);
    std::vector<int> zedFunction = zFunction(reverseString);
    int length = reverseString.size();
    int start = 0;
    for(size_t i = 1; i < reverseString.size(); ++i) {
        size_t j = i;
        if(i < reverseString.size() - 1) {
            j += zedFunction[i + 1];
        }
        size_t k = i + 1 - piFunction[i];
        if((i + 1) % k == 0) {
            if(k + reverseString.size() - (j + 1) < length) {
                length = k + reverseString.size() - (j + 1);
                start = j;
            }
        } else {
            if(i + 1 + reverseString.size() - (j + 1) < length) {
                length = i + 1 + reverseString.size() - (j + 1);
                start = j;
            }
        }
    }
    std::cout << number[0] << number[1];
    start = number.size() - 1 - start;
    length += 2;
    for(size_t i = 2; i < start; ++i) {
        std::cout << number[i];
    }
    std::cout << '(';
    for(size_t i = start; i < length; ++i) {
        std::cout << number[i];
    }
    std::cout << ')';
    return 0;
}