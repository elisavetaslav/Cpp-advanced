#pragma once

#include <stdint.h>

constexpr int64_t pow_mod(int64_t a, int64_t b, int64_t m) {
return b == 0 ? 1 : (a * pow_mod(a, b - 1, m)) % m;
}
constexpr int hash_ind(const char *s, int p, int mod, uint64_t ind) {
    return *s == '\0' ? 0 : ((*s * pow_mod(p, ind, mod)) % mod + hash_ind(s + 1, p, mod, ind + 1) % mod) % mod;
}

constexpr int hash(const char *s, int p, int mod) {
    return hash_ind(s, p, mod, 0);
}