#include <vector>

constexpr int P = 998244353;

int power(int a, int b) {
    int r = 1;
    while (b) {
        if (b & 1) r = 1ll * r * a % P;
        a = 1ll * a * a % P;
        b >>= 1;
    }
    return r;
}

struct Binom {
    std::vector<int> fac, ifac;
    Binom(int n) : fac(n), ifac(n) {
        fac[0] = ifac[0] = 1;
        for (int i = 1; i < n; ++i) fac[i] = 1ll * fac[i - 1] * i % P;
        ifac[n - 1] = power(fac[n - 1], P - 2);
        for (int i = n - 1; i > 1; --i) ifac[i - 1] = 1ll * ifac[i] * i % P;
    }
    int operator ()(int x, int y) const {
        return 1ll * fac[x] * ifac[y] % P * ifac[x - y] % P;
    }
};

