#pragma once

#include <stdint.h>

constexpr int pow(int a, int b) {
    return b == 0 ? 1 : a * pow(a, b - 1);
}