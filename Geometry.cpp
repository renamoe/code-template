// 实数

using f64 = double;

constexpr f64 Eps = 1e-9;

int fcmp(f64 x) {
    if (std::abs(x) < Eps) return 0;
    return x > 0 ? 1 : -1;
}

struct Point {
    f64 x, y;

    Point() {}
    Point(f64 x, f64 y) : x(x), y(y) {}
    f64 norm2() const {
        return x * x + y * y;
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
    friend f64 operator *(const Point &a, const Point &b) {
        return a.x * b.x + a.y * b.y;
    }
    friend f64 operator ^(const Point &a, const Point &b) {
        return a.x * b.y - a.y * b.x;
    }
    friend bool operator <(const Point &a, const Point &b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};

struct Line {
    Point s, d;

    Line() {}
    Line(Point s, Point d) : s(s), d(d) {}
    f64 angle() const {
        return std::atan2(d.y, d.x);
    }
    friend bool operator <(const Line &a, const Line &b) {
        if (fcmp(a.angle() - b.angle()) == 0) {
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
        if (i > 0 && fcmp(a[i].angle() - a[i - 1].angle()) == 0) continue;
        while (r - l >= 2 && fcmp((p[r - 2] - a[i].s) ^ a[i].d) > 0) --r;
        while (r - l >= 2 && fcmp((p[l] - a[i].s) ^ a[i].d) > 0) ++l;
        if (r - l >= 1) p[r - 1] = inter(b[r - 1], a[i]);
        b[r++] = a[i];
    }
    while (r - l >= 2 && fcmp((p[r - 2] - b[l].s) ^ b[l].d) > 0) --r;
    if (r - l >= 2) p[r - 1] = inter(b[l], b[r - 1]);
    b = std::vector<Line>(b.begin() + l, b.begin() + r);
    for (int i = 0; i < r - l; ++i) {
        if (fcmp(b[i].d ^ b[(i + 1) % (r - l)].d) <= 0) return std::vector<Point>();
    }
    p = std::vector<Point>(p.begin() + l, p.begin() + r);
    return p;
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

