using u64 = unsigned long long;

int ctz(u64 x) {
    return __builtin_ctzll(x);
}
int clz(u64 x) {
    return 63 - __builtin_clzll(x);
}

// 64^3 = 262144
struct PressedTrie {
    u64 f0;
    u64 f1[64];
    u64 f2[64 * 64];

    void insert(int k) {
        f2[k >> 6] |= 1ull << (k & 63);
        k >>= 6;
        f1[k >> 6] |= 1ull << (k & 63);
        k >>= 6;
        f0 |= 1ull << (k & 63);
    }
    void erase(int k) {
        if (f2[k >> 6] ^= 1ull << (k & 63)) return;
        k >>= 6;
        if (f1[k >> 6] ^= 1ull << (k & 63)) return;
        k >>= 6;
        f0 ^= 1ull << (k & 63);
    }
    int min() const {
        int k = ctz(f0);
        k = k << 6 | ctz(f1[k]);
        k = k << 6 | ctz(f2[k]);
        return k;
    }
    int max() const {
        int k = clz(f0);
        k = k << 6 | clz(f1[k]);
        k = k << 6 | clz(f2[k]);
        return k;
    }
    bool empty() const {
        return !f0;
    }
    void clear() {
        while (f0) erase(min());
    }
    void reset() {
        f0 = 0;
        std::memset(f1, 0, sizeof(f1));
        std::memset(f2, 0, sizeof(f2));
    }
};
