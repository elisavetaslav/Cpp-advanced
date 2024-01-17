#pragma once

#include <cmath>
#include <algorithm>
#include <thread>
#include <sstream>
#include <iostream>
#include <atomic>
#include <vector>
#include <mutex>

template <class RandomAccessIterator, class T, class Func>
T Reduce(RandomAccessIterator first, RandomAccessIterator last, const T& initial_value, Func func) {
    auto count = std::thread::hardware_concurrency();
    std::vector<std::thread> v;
    std::vector<T> ans(count, initial_value);
    std::mutex ans_mutex;
    auto d = std::max<long>((std::distance(first, last) + count - 2) / count, 1);
    for (size_t i = 0; i < count; ++i) {
        if (first + i * d == last) {
            break;
        }
        v.emplace_back(std::thread([&, i, d] {
            T cur_value(initial_value);
            RandomAccessIterator f = first + i * d, l = std::min(first + (i + 1) * d, last);
            while (f != l) {
                cur_value = func(cur_value, *f++);
            }
            ans_mutex.lock();
            ans[i] = cur_value;
            ans_mutex.unlock();
        }));
    }
    for (int i = 0; i < v.size(); ++i) {
        v[i].join();
    }
    T answer = initial_value;
    for (int i = 0; i < v.size(); ++i) {
        T cur_value(answer);
        answer = func(cur_value, ans[i]);
    }
    return answer;
}