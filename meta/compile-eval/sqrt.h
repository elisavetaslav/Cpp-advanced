#include <stdint.h>

template<bool f, int l, int r, int N, bool breakk>
struct BinSearch {
    static const int value = BinSearch<static_cast<uint64_t>(((l+ r) / 2 + r) / 2) * (((l + r) / 2 + r) / 2) < N,(l + r) / 2, r, N, (r > ((l + r) / 2 + 1))>::value;
};

template<int l, int r, int N>
struct BinSearch<false, l, r, N, true> {
    static const int value = BinSearch<static_cast<uint64_t>((l + (l + r) / 2) / 2) * ((l + (l + r) / 2) / 2) < N, l, (l + r) / 2, N, ((l + r) / 2 > (l + 1))>::value;
};

template<bool f, int l, int r, int N>
struct BinSearch<f, l, r, N, false> {
    static const int value = r;
};

template<int N>
struct Sqrt {
    static const int value = BinSearch<(static_cast<uint64_t>((N - 1) / 2) * ((N - 1) / 2)) < N, -1, N, N, true>::value;
};