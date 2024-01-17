#pragma once

#include <array>
#include <stdint.h>

template<uint64_t N>
constexpr int determinant(const std::array<std::array<int, N>, N>& a) {
    int ans = 0;
    for (uint64_t i = 0; i < N; ++i) {
        std::array<std::array<int, N - 1>, N - 1> b{};
        for (std::size_t j = 1; j < N; ++j) {

            for (uint64_t k = 0; k < i; ++k) {
                (&std::get<0>((&std::get<0>(b))[j-1]))[k] = a[j][k];
            }
            for (uint64_t k = i + 1; k < N; ++k) {
                (&std::get<0>((&std::get<0>(b))[j-1]))[k - 1] = a[j][k];
            }
        }
        ans += (i % 2 == 1 ? -1 : 1) * a[0][i] * determinant<N - 1>(b);
    }
    return ans;
}

template<>
constexpr int determinant<1>(const std::array<std::array<int, 1>, 1>& a) {
    return a[0][0];
}