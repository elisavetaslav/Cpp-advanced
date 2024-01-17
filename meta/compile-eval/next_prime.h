
#pragma once

constexpr bool is_prime(int n) {
    int del = 3;
    while (del * del <= n && n % del != 0) {
        del += 2;
    }
    int ans = n % 2 == 0 ? n == 2 : del * del > n;
    return n == 1 ? false : ans;
}

constexpr int next_prime(int x) {
    int y = x;
    while (!is_prime(y)) {
        ++y;
    }
    return y;
}