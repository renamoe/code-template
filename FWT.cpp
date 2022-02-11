// FWT

// 自己写！


// ============================================

// 多项式复合集合幂级数

constexpr int P = 998244353;

void inc(int &x, int y) {
    x += y;
    if (x >= P) x -= P;
}

void dec(int &x, int y) {
    x -= y;
    if (x < 0) x += P;
}

constexpr int N = 21;

using Arr = int[N + 1];

void transform(Arr *f, void (*op)(int &, int), int d) {
    const int n = 1 << d;
    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += k << 1)
            for (int j = 0; j < k; ++j)
                for (int t = 0; t <= d; ++t)
                    op(f[i | k | j][t], f[i | j][t]);
}

std::vector<int> composite(const std::vector<int> &f, const std::vector<int> &g) {
    const int n = f.size() - 1;
    assert(g.size() == 1u << n);
    static Arr a[1 << N], b[1 << N], t[1 << N];

    for (int i = 0; i <= n; ++i) {
        std::fill_n(a[i], N + 1, 0);
        a[i][0] = f[i];
    }
    for (int k = 0; k < n; ++k) {
        for (int s = 0; s < (1 << k); ++s) {
            std::fill_n(t[s], k + 1, 0);
            t[s][__builtin_popcount(s)] = g[s | (1 << k)];
        }
        transform(t, inc, k);

        for (int i = 0; i < n - k; ++i) {
            std::copy_n(a[i << k], (N + 1) << k, b[i << (k + 1)]);
            std::copy_n(a[i << k], (N + 1) << k, b[(i << (k + 1)) | (1 << k)]);
        }
        for (int i = 1; i <= n - k; ++i) {
            for (int s = 0; s < (1 << k); ++s) {
                for (int j = 0; j <= k; ++j) {
                    uint64_t r = 0;
                    for (int l = 0; l <= j; ++l) {
                        r += 1ull * a[(i << k) | s][l] * t[s][j - l];
                    }
                    inc(b[((i - 1) << (k + 1)) | (1 << k) | s][j + 1], int(r % P));
                }
            }
        }
        std::copy_n(b[0], (n - k) * (N + 1) << (k + 1), a[0]);
    }
    transform(a, dec, n);
    std::vector<int> res(1 << n);
    for (int s = 0; s < (1 << n); ++s) res[s] = a[s][__builtin_popcount(s)];
    return res;
}
