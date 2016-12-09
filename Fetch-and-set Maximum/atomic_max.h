//
// Created by philipp on 09.12.16.
//
#include <atomic>
#include <cmath>

int fetch_and_set_max(std::atomic<int>& x, int y) {
    int ans = x.load();
    if(ans < y) {
        x.store(y);
    }
    return ans;
}