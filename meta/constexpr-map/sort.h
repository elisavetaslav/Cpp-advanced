#pragma once

#include <constexpr_map.h>

#include <type_traits>
#include <array>
#include <utility>
#include <tuple>
#include <exception>
#include <stdexcept>

template<class K, class V, int S>
constexpr auto Sort(ConstexprMap<K, V, S> map) {
    if constexpr (std::is_integral<V>()) {
        for (int i = map.Size() - 1; i > 0; --i) {
            for (int j = 0; j < i; ++j) {
                if (!(map.GetByIndex(j).first < map.GetByIndex(j + 1).first)) {
                    map.Swap(j, j + 1);
                }
            }
        }
        return map;
    }
    for (int i = map.Size() - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (map.GetByIndex(j).first < map.GetByIndex(j + 1).first) {
                map.Swap(j, j + 1);
            }
        }
    }
    return map;
}