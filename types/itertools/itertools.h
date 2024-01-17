#pragma once

#include <cmath>
#include <tuple>
#include <utility>

template <class T>
class IteratorRange {
public:
    using value_type = T;  // NOLINT
    IteratorRange(T now, T step) : now_(now), step_(step){};
    IteratorRange operator++() {
        now_ += step_;
        return *this;
    }
    bool operator!=(IteratorRange another) {
        return now_ != another.now_;
    }
    T operator*() {
        return now_;
    }

private:
    T now_;
    T step_;
};

template <class It>
class Iterator {
public:
    Iterator(It begin, It end) : begin_(begin), end_(end) {
    }
    It begin() const {  // NOLINT
        return begin_;
    }
    It end() const {  // NOLINT
        return end_;
    }

private:
    It begin_, end_;
};

template <class T>
auto Range(T last) {
    T step = static_cast<T>(1);
    T first = static_cast<T>(0);
    T end = first + (last - first - 1 + step) / step * step;
    return Iterator(IteratorRange(first, step), IteratorRange(end, step));
}

template <class T>
auto Range(T first, T last) {
    T step = static_cast<T>(1);
    T end = first + (last - first - 1 + step) / step * step;
    return Iterator(IteratorRange(first, step), IteratorRange(end, step));
}

template <class T>
auto Range(T first, T last, T step) {
    T end = first + (last - first - 1 + step) / step * step;
    return Iterator(IteratorRange(first, step), IteratorRange(end, step));
}

template <class T, class U, class TVType = typename T::value_type,
        class UVType = typename U::value_type>
class IteratorZip {
public:
    IteratorZip(T first, U second) : first_(first), second_(second){};

    IteratorZip operator++() {
        ++first_;
        ++second_;
        return *this;
    }

    bool operator!=(IteratorZip another) {
        return first_ != another.first_ && second_ != another.second_;
    }

    std::pair<TVType, UVType> operator*() {
        return std::make_pair(*first_, *second_);
    }

private:
    T first_;
    U second_;
};

template <class T, class U>
auto Zip(T& first, U& second) {
    return Iterator(IteratorZip(first.begin(), second.begin()),
                    IteratorZip(first.end(), second.end()));
}

template <class T, class U>
auto Zip(T& first, U&& second) {
    return Iterator(IteratorZip(first.begin(), second.begin()),
                    IteratorZip(first.end(), second.end()));
}

template <class T, class U>
auto Zip(T&& first, U& second) {
    return Iterator(IteratorZip(first.begin(), second.begin()),
                    IteratorZip(first.end(), second.end()));
}

template <class T, class U>
auto Zip(T&& first, U&& second) {
    return Iterator(IteratorZip(first.begin(), second.begin()),
                    IteratorZip(first.end(), second.end()));
}

template <class T>
class IteratorGroup {
public:
    IteratorGroup(T begin, T end) : begin_(begin), end_(end){};
    IteratorGroup operator++() {
        T curr = begin_;
        for (T it = begin_; it != end_; ++it) {
            if (*begin_ == *it) {
                ++curr;
            } else {
                break;
            }
        }
        begin_ = curr;
        return *this;
    }
    bool operator!=(IteratorGroup another) {
        return begin_ != another.begin_;
    }
    Iterator<T> operator*() {
        T curr = begin_;
        for (T it = begin_; it != end_; ++it) {
            if (*begin_ == *it) {
                ++curr;
            } else {
                break;
            }
        }
        return Iterator(begin_, curr);
    }
    using value_type = T;  // NOLINT

private:
    T begin_;
    T end_;
};

template <class T>
auto Group(T& value) {
    return Iterator(IteratorGroup(value.begin(), value.end()),
                    IteratorGroup(value.end(), value.end()));
}