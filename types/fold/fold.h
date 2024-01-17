#pragma once

#include <vector>

struct Sum {
    template <class T>
    T operator()(T init, T second) {
        return init + second;
    }
};

struct Prod {
    template <class T>
    T operator()(T init, T second) {
        return init * second;
    }
};

struct Concat {
    template <class T>
    std::vector<T> operator()(std::vector<T>& init, std::vector<T>& second) {
        for (std::size_t i = 0; i < second.size(); ++i) {
            init.emplace_back(second[i]);
        }
        return init;
    }
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    for (Iterator it = first; it != last; ++it) {
        init = func(init, *it);
    }
    return init;
}

class Length {
public:
    Length(int* a) {
        a_ = a;
    }

    template <class T>
    int operator()(T, T) {
        return ++*a_;
    }

private:
    int* a_;
};