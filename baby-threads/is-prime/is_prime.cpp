#include "is_prime.h"
#include <cmath>
#include <algorithm>
#include <thread>
#include <sstream>
#include <iostream>
#include <atomic>

std::atomic<bool> is_res = false;
std::atomic<bool> ans = false;

#include <exception>
#include <vector>

void Prime(uint64_t l, uint64_t r, uint64_t x) {
    if (x <= 1) {
        ans = false;
        is_res = true;
        return;
    }
    for (uint64_t i = std::max<uint64_t>(l, 2); i < r; ++i) {
        if (x % i == 0) {
            ans = false;
            is_res = true;
            return;
        }
    }
}

bool IsPrime(uint64_t x) {
    ans = true;
    int count = std::thread::hardware_concurrency();
    uint64_t root = sqrt(x);
    auto bound = std::min(root + 6, x);
    auto d = (bound + count - 2) / count;
    std::vector<std::thread> v(count);
    for (int i = 0; i < count; ++i) {
        v[i] = std::thread(Prime, i * d, std::min((i + 1) * d, bound), x);
    }
    for (int i = 0; i < count; ++i) {
        v[i].join();
    }
    return ans;
}