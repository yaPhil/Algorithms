#include <iostream>
#include <string>
#include <vector>
const unsigned long long P = 101;
const unsigned long long Q = 1000000007;
int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::string s;
    std::cin >> s;
    std::vector<unsigned long long> hash(s.size() + 1);
    std::vector<unsigned long long> power(s.size() + 1);
    hash[0] = 0;
    power[0] = 1;
    for(size_t i = 0; i < s.size(); ++i) {
        hash[i + 1] = hash[i] * P + (unsigned long long) s[i];
        power[i + 1] = power[i] * P;
    }
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i)
    {
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;
        unsigned long long hash1 = hash[b] - hash[a - 1] * power[b - a + 1];
        unsigned long long hash2 = hash[d] - hash[c - 1] * power[d - c + 1];
        if(hash1 != hash2) {
            std::cout << "No\n";
        } else {
            std::cout << "Yes\n";
        }
    }
    return 0;
}