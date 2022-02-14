
std::vector<int> lagrange(std::vector<int> x, std::vector<int> y) {
    const int n = x.size();
    std::vector<int> l(n + 1);
    l[0] = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = i; j >= 0; --j) {
            inc(l[j + 1], l[j]);
            l[j] = 1ll * l[j] * (P - x[i]) % P;
        }
    }
    std::vector<int> f(n);
    for (int i = 0; i < n; ++i) {
        int prod = 1;
        for (int j = 0; j < n; ++j) {
            if (j != i) prod = 1ll * prod * (x[i] - x[j] + P) % P;
        }
        int cof = 1ll * y[i] * inverse(prod) % P;
        int inv = inverse(P - x[i]);
        int last = 0;
        for (int j = 0; j < n; ++j) {
            int now = 1ll * (l[j] - last + P) * inv % P;
            f[j] = (f[j] + 1ll * now * cof) % P;
            last = now;
        }
    }
    return f;
}
