// FFT

constexpr double Pi = std::acos(-1.0);

struct Complex {
    double real, imag;
    Complex(double x = 0, double y = 0) : real(x), imag(y) {}
    friend Complex operator +(const Complex &x, const Complex &y) {
        return Complex(x.real + y.real, x.imag + y.imag);
    }
    friend Complex operator -(const Complex &x, const Complex &y) {
        return Complex(x.real - y.real, x.imag - y.imag);
    }
    friend Complex operator *(const Complex &x, const Complex &y) {
        return Complex(x.real * y.real - x.imag * y.imag, x.real * y.imag + x.imag * y.real);
    }
    Complex conj() const {
        return Complex(real, -imag);
    }
};

void dft(std::vector<Complex> &a) {
    static std::vector<int> rev;
    static std::vector<Complex> roots{Complex(0, 0), Complex(1, 0)};
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
            auto d = Pi / (1 << k);
            Complex w(std::cos(d), std::sin(d));
            for (int i = 1 << (k - 1); i < (1 << k); ++i) {
                roots[i * 2] = roots[i];
                roots[i * 2 + 1] = roots[i] * w;
            }
            ++k;
        }
    }
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += k * 2) {
            for (int j = 0; j < k; ++j) {
                Complex x = a[i + j];
                Complex y = a[i + k + j] * roots[k + j];
                a[i + j] = x + y;
                a[i + k + j] = x - y;
            }
        }
    }
}

void idft(std::vector<Complex> &a) {
    int n = a.size();
    std::reverse(a.begin() + 1, a.end());
    dft(a);
    for (int i = 0; i < n; ++i) {
        a[i].real /= n;
        a[i].imag /= n;
    }
}

// 简单卷积

std::vector<Complex> operator *(std::vector<Complex> a, std::vector<Complex> b) {
    int tot = a.size() + b.size() - 1;
    int n = 1;
    while (n < tot) n *= 2;
    a.resize(n);
    b.resize(n);
    dft(a);
    dft(b);
    std::vector<Complex> res(n);
    for (int i = 0; i < n; ++i) res[i] = a[i] * b[i];
    idft(res);
    res.resize(tot);
    return res;
}


// MTT
// Complex 需要开 long double

std::vector<int> mtt(const std::vector<int> &f, const std::vector<int> &g, int p) {
    int tot = f.size() + g.size() - 1;
    int n = 1;
    while (n < tot) n *= 2;
    std::vector<Complex> a(n), b(n), c(n), d(n);
    for (int i = 0; i < int(f.size()); ++i) a[i] = Complex(f[i] >> 15, f[i] & 32767);
    for (int i = 0; i < int(g.size()); ++i) c[i] = Complex(g[i] >> 15, g[i] & 32767);
    dft(a);
    dft(c);
    for (int i = 0; i < n; ++i) b[i] = a[i].conj();
    std::reverse(b.begin() + 1, b.end());
    for (int i = 0; i < n; ++i) d[i] = c[i].conj();
    std::reverse(d.begin() + 1, d.end());
    for (int i = 0; i < n; ++i) {
        Complex aa = (a[i] + b[i]) * Complex(0.5, 0);
        Complex bb = (a[i] - b[i]) * Complex(0, -0.5);
        Complex cc = (c[i] + d[i]) * Complex(0.5, 0);
        Complex dd = (c[i] - d[i]) * Complex(0, -0.5);
        a[i] = aa * cc + Complex(0, 1) * (aa * dd + bb * cc);
        b[i] = bb * dd;
    }
    idft(a);
    idft(b);
    std::vector<int> res(tot);
    for (int i = 0; i < tot; ++i) {
        int t1 = (long long)(a[i].real + 0.5) % p;
        int t2 = (long long)(a[i].imag + 0.5) % p;
        int t3 = (long long)(b[i].real + 0.5) % p;
        res[i] = (((1ll << 30) * t1 + (1ll << 15) * t2 + t3) % p + p) % p;
    }
    res.resize(tot);
    return res;
}