// 实数

using f64 = double;

constexpr f64 Eps = 1e-9;

int sign(f64 x) {
    if (std::fabs(x) < Eps) return 0;
    return x > 0 ? 1 : -1;
}

struct Point {
    f64 x, y;

    Point() {}
    Point(f64 x, f64 y) : x(x), y(y) {}
    f64 norm2() const {
        return x * x + y * y;
    }
    f64 norm() const {
        return std::sqrt(norm2());
    }
    Point rotate() const {
        return Point(-y, x);
    }
    Point rotate(f64 a) const {
        f64 c = std::cos(a);
        f64 s = std::sin(a);
        return Point(x * c - y * s, x * s + y * c);
    }
    Point operator -() const {
        return Point(-x, -y);
    }
    friend Point operator +(const Point &a, const Point &b) {
        return Point(a.x + b.x, a.y + b.y);
    }
    friend Point operator -(const Point &a, const Point &b) {
        return Point(a.x - b.x, a.y - b.y);
    }
    friend Point operator *(const Point &a, f64 b) {
        return Point(a.x * b, a.y * b);
    }
    friend Point operator /(const Point &a, f64 b) {
        return Point(a.x / b, a.y / b);
    }
    friend f64 operator *(const Point &a, const Point &b) {
        return a.x * b.x + a.y * b.y;
    }
    friend f64 operator ^(const Point &a, const Point &b) {
        return a.x * b.y - a.y * b.x;
    }
    friend bool operator <(const Point &a, const Point &b) {
        if (sign(a.x - b.x) == 0) return sign(a.y - b.y) < 0;
        return a.x < b.x;
    }
    friend bool operator ==(const Point &a, const Point &b) {
        return sign(a.x - b.x) == 0 && sign(a.y - b.y) == 0;
    }
};

std::vector<Point> convexHull(std::vector<Point> p) {
    std::sort(p.begin(), p.end());
    p.erase(std::unique(p.begin(), p.end()), p.end());
    std::sort(p.begin() + 1, p.end(), [&](const Point &a, const Point &b) {
        f64 d = (a - p[0]) ^ (b - p[0]);
        if (sign(d) == 0) return (a - p[0]).norm2() < (b - p[0]).norm2();
        return d < 0;
    });
    std::vector<Point> res(p.size());
    auto top = res.begin();
    for (auto i : p) {
        while (top - res.begin() >= 2 && sign((i - top[-2]) ^ (top[-1] - top[-2])) <= 0) {
            --top;
        }
        *top++ = i;
    }
    res.erase(top, res.end());
    return res;
}

struct Line {
    Point s, d;

    Line() {}
    Line(Point s, Point d) : s(s), d(d) {}
    f64 angle() const {
        return std::atan2(d.y, d.x);
    }
    friend bool operator <(const Line &a, const Line &b) {
        if (sign(a.angle() - b.angle()) == 0) {
            return ((b.s - a.s) ^ a.d) > 0;
        }
        return a.angle() < b.angle();
    }
    friend Point inter(const Line &a, const Line &b) {
        f64 k = (b.d ^ (a.s - b.s)) / (a.d ^ b.d);
        return a.s + a.d * k;
    }
};

std::vector<Point> half_plane_inter(std::vector<Line> a) {
    std::sort(a.begin(), a.end());
    const int n = a.size();
    std::vector<Line> b(n);
    std::vector<Point> p(n);
    int l = 0, r = 0;
    for (int i = 0; i < n; ++i) {
        if (i > 0 && sign(a[i].angle() - a[i - 1].angle()) == 0) continue;
        while (r - l >= 2 && sign((p[r - 2] - a[i].s) ^ a[i].d) > 0) --r;
        while (r - l >= 2 && sign((p[l] - a[i].s) ^ a[i].d) > 0) ++l;
        if (r - l >= 1) p[r - 1] = inter(b[r - 1], a[i]);
        b[r++] = a[i];
    }
    while (r - l >= 2 && sign((p[r - 2] - b[l].s) ^ b[l].d) > 0) --r;
    if (r - l >= 2) p[r - 1] = inter(b[l], b[r - 1]);
    b = std::vector<Line>(b.begin() + l, b.begin() + r);
    for (int i = 0; i < r - l; ++i) {
        if (sign(b[i].d ^ b[(i + 1) % (r - l)].d) <= 0) return std::vector<Point>();
    }
    p = std::vector<Point>(p.begin() + l, p.begin() + r);
    return p;
}

std::vector<Point> minkowski(std::vector<Point> a, std::vector<Point> b) {
    int n = a.size();
    int m = b.size();
    std::vector<Point> res(n + m);
    res[0] = a[0] + b[0];
    Point t = a[0];
    for (int i = 0; i < n - 1; ++i) a[i] = a[i + 1] - a[i];
    a[n - 1] = t - a[n - 1];
    t = b[0];
    for (int i = 0; i < m - 1; ++i) b[i] = b[i + 1] - b[i];
    b[m - 1] = t - b[m - 1];
    int i = 0, j = 0;
    for (int k = 1; k < n + m; ++k) {
        if (i < n && (j == m || (a[i] ^ b[j]) <= 0)) {
            res[k] = res[k - 1] + a[i++];
        } else {
            res[k] = res[k - 1] + b[j++];
        }
    }
    return res;
}

// ----------------------------------------------------------------

// 整数

using i64 = long long;

struct Point {
    int x, y;
 
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    i64 norm2() const {
        return (i64) x * x + (i64) y * y;
    }
    Point operator -() const {
        return Point(-x, -y);
    }
    friend Point operator +(const Point &a, const Point &b) {
        return Point(a.x + b.x, a.y + b.y);
    }
    friend Point operator -(const Point &a, const Point &b) {
        return Point(a.x - b.x, a.y - b.y);
    }
    friend i64 operator *(const Point &a, const Point &b) {
        return (i64) a.x * b.x + (i64) a.y * b.y;
    }
    friend i64 operator ^(const Point &a, const Point &b) {
        return (i64) a.x * b.y - (i64) a.y * b.x;
    }
    friend bool operator <(const Point &a, const Point &b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};
 
std::vector<Point> convex_hull(std::vector<Point> p) {
    std::iter_swap(p.begin(), std::min_element(p.begin(), p.end()));
    std::sort(p.begin() + 1, p.end(), [&](const Point &a, const Point &b) {
        i64 d = (b - p[0]) ^ (a - p[0]);
        return d > 0 || (d == 0 && (a - p[0]).norm2() < (b - p[0]).norm2());
    });
    std::vector<Point> res(p.size());
    auto top = res.begin();
    for (const auto &x : p) {
        while (top - res.begin() >= 2 && ((x - *(top - 2)) ^ (*(top - 1) - *(top - 2))) <= 0) --top;
        *top++ = x;
    }
    res.erase(top, res.end());
    return res;
}

