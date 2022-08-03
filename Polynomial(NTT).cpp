// NTT

constexpr int P = 998244353, R = 3;

int norm(int x) {
    return x >= P ? (x - P) : x;
}

void inc(int &x, int y) {
    x += y;
    if (x >= P) x -= P;
}

int power(int a, int b) {
    int r = 1;
    while (b) {
        if (b & 1) r = 1ll * r * a % P;
        a = 1ll * a * a % P;
        b >>= 1;
    }
    return r;
}

void dft(std::vector<int> &a) {
    static std::vector<int> rev, roots{0, 1};
    int n = a.size();
    if (int(rev.size()) != n) {
        int k = __builtin_ctz(n) - 1;
        rev.resize(n);
        for (int i = 0; i < n; ++i)
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << k);
    }
    for (int i = 0; i < n; ++i)
        if (rev[i] < i) std::swap(a[rev[i]], a[i]);
    if (int(roots.size()) < n) {
        int k = __builtin_ctz(roots.size());
        roots.resize(n);
        while ((1 << k) < n) {
            int wn = power(R, (P - 1) >> (k + 1));
            for (int i = 1 << (k - 1); i < (1 << k); ++i) {
                roots[i * 2] = roots[i];
                roots[i * 2 + 1] = 1ll * roots[i] * wn % P;
            }
            ++k;
        }
    }
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += k * 2) {
            for (int j = 0; j < k; ++j) {
                int x = a[i + j];
                int y = 1ll * a[i + k + j] * roots[k + j] % P;
                a[i + j] = norm(x + y);
                a[i + k + j] = norm(x - y + P);
            }
        }
    }
}

void idft(std::vector<int> &a) {
    int n = a.size();
    std::reverse(a.begin() + 1, a.end());
    dft(a);
    int invn = power(n, P - 2);
    for (int i = 0; i < n; ++i) a[i] = 1ll * a[i] * invn % P;
}


std::vector<int> operator *(std::vector<int> a, std::vector<int> b) {
    int tot = a.size() + b.size() - 1;
    int n = 1;
    while (n < tot) n *= 2;
    std::vector<int> res(n);
    a.resize(n);
    b.resize(n);
    dft(a);
    dft(b);
    for (int i = 0; i < n; ++i) res[i] = 1ll * a[i] * b[i] % P;
    idft(res);
    res.resize(tot);
    return res;
}


struct Poly {
    std::vector<int> a;
    Poly() {}
    Poly(int a0) {
        if (a0) a = {a0};
    }
    Poly(const std::vector<int> &a1) : a(a1) {
        while (!a.empty() && !a.back()) a.pop_back();
    }
    int size() const { return a.size(); }
    int operator [](const int k) const {
        if (k < 0 || k >= size()) return 0;
        return a[k];
    }
    Poly modxk(int k) const {
        return Poly(std::vector<int>(a.begin(), a.begin() + std::min(k, size())));
    }
    friend Poly operator +(const Poly &a, const Poly &b) {
        std::vector<int> res(std::max(a.size(), b.size()));
        for (int i = 0; i < int(res.size()); ++i) res[i] = norm(a[i] + b[i]);
        return Poly(res);
    }
    friend Poly operator -(const Poly &a, const Poly &b) {
        std::vector<int> res(std::max(a.size(), b.size()));
        for (int i = 0; i < int(res.size()); ++i) res[i] = norm(a[i] - b[i] + P);
        return Poly(res);
    }
    friend Poly operator *(Poly a, Poly b) {
        int tot = a.size() + b.size() - 1;
        int n = 1;
        while (n < tot) n *= 2;
        std::vector<int> res(n);
        a.a.resize(n);
        b.a.resize(n);
        dft(a.a);
        dft(b.a);
        for (int i = 0; i < n; ++i) res[i] = 1ll * a[i] * b[i] % P;
        idft(res);
        return Poly(res);
    }
    Poly inv(int m) const {
        Poly f(power(a[0], P - 2));
        int k = 1;
        while (k < m) {
            k *= 2;
            f = (f * (2 - modxk(k) * f)).modxk(k);
        }
        return f.modxk(m);
    }
    Poly deriv() const {
        if (!size()) return Poly();
        std::vector<int> res(size() - 1);
        for (int i = 0; i < size() - 1; ++i)
            res[i] = 1ll * (i + 1) * a[i + 1] % P;
        return Poly(res);
    }
    Poly integr() const {
        if (!size()) return Poly();
        std::vector<int> res(size() + 1);
        for (int i = 0; i < size(); ++i)
            res[i + 1] = 1ll * power(i + 1, P - 2) * a[i] % P;
        return Poly(res);
    }
    Poly log(int m) const {
        return (deriv() * inv(m)).integr().modxk(m);
    }
    Poly exp(int m) const {
        Poly f(1);
        int k = 1;
        while (k < m) {
            k *= 2;
            f = (f * (1 - f.log(k) + modxk(k))).modxk(k);
        }
        return f.modxk(m);
    }
    Poly rev(int m = -1) const {
        auto b = a;
        if (~m) b.resize(m);
        std::reverse(b.begin(), b.end());
        return Poly(b);
    }
    Poly mod(Poly g) const {
        int n = size();
        int m = g.size();
        if (n < m) return *this;
        Poly f(a);
        int lim = n - m + 1;
        auto q = (f.rev().modxk(lim) * g.rev().modxk(lim).inv(lim)).rev(lim);
        return (f - q * g).modxk(m);
    }
};
