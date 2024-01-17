#pragma once

#include <stack>
#include <tuple>

template <class It, class P, class F>
void TransformIf(It begin, It end, P p, F f) {
    using T = typename std::decay<decltype(*begin)>::type;
    std::stack<It> log_it;
    std::stack<T> log;
    try {
        for (It it = begin; it != end; ++it) {
            if (p(*it)) {
                try {
                    log.push(*it);
                    log_it.push(it);
                } catch (...) {
                }
                f(*it);
            }
        }
    } catch (...) {
        while (!log.empty()) {
            try {
                It p_it = log_it.top();
                log_it.pop();
                *p_it = log.top();
                log.pop();
            } catch (...) {
                throw;
            }
        }
        throw;
    }
}