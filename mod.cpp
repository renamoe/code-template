constexpr int P = 998244353;

int norm(int x) {
    return x >= P ? (x - P) : x;
}
void inc(int &x, int y) {
    x += y;
    if (x >= P) x -= P;
}
void dec(int &x, int y) {
    x -= y;
    if (x < 0) x += P;
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

// Barrett

using u64 = unsigned long long;
using u128 = __uint128_t;

struct FastMod {
    u64 b, m;

    FastMod() {}
    FastMod(u64 b) : b(b), m(u64((u128(1) << 64) / b)) {}

    u64 operator()(u64 a) const {
        u64 q = (u64) ((u128(m) * a) >> 64);
        u64 r = a - q * b;
        return r >= b ? r - b : r;
    }
} mod;
