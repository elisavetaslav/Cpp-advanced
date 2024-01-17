#pragma once

template<unsigned int a, unsigned int b>
struct Pow {
    static const int value = a * Pow<a, b - 1>::value;
};

template<unsigned int a>
struct Pow<a, 0> {
    static const int value = 1;
};