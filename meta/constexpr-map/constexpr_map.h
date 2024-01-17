#pragma once

#include <type_traits>
#include <array>
#include <utility>
#include <tuple>
#include <exception>
#include <stdexcept>

template<class K, class V, int MaxSize = 8>
class ConstexprMap {
public:
    constexpr ConstexprMap() = default;

    constexpr V& operator[](const K& key) {
        if (size_ > MaxSize) {
            throw std::runtime_error("");
        }
        for (int i = 0; i < size_; ++i) {
            if (arr_[i].first == key) {
                return arr_[i].second;
            }
        }
        arr_[size_].first = key;
        ++size_;
        return arr_[size_ - 1].second;
    }

    constexpr const V& operator[](const K& key) const {
        for (int i = 0; i < size_; ++i) {
            if (arr_[i].first == key) {
                return arr_[i].second;
            }
        }
        throw std::runtime_error("");
    }

    constexpr void Swap(int a, int b) {
        std::pair<K, V> p = arr_[a];
        arr_[a] = arr_[b];
        arr_[b] = p;
    }

    constexpr bool Erase(const K& key) {
        if (Find(key)) {
            for (int i = 0; i < size_; ++i) {
                if (arr_[i].first == key) {
                    for (int l = i; l < size_ - 1; ++l) {
                        arr_[i] = arr_[i + 1];
                    }
                    --size_;
                    return true;
                }
            }
        }
        return false;
    }

    constexpr bool Find(const K& key) const {
        for (int i = 0; i < size_; ++i) {
            if (arr_[i].first == key) {
                return true;
            }
        }
        return false;
    }

    constexpr std::size_t Size() const {
        return size_;
    }

    constexpr std::pair<K, V>& GetByIndex(std::size_t pos) {
        if (pos >= size_) {
            throw std::runtime_error("");
        }
        return arr_[pos];
    }

    constexpr const std::pair<K, V>& GetByIndex(std::size_t pos) const {
        if (pos >= size_) {
            throw std::runtime_error("");
        }
        return arr_[pos];
    }

private:
    std::array<std::pair<K, V>, MaxSize> arr_;
    std::size_t size_ = 0;
};