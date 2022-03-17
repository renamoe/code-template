#include <vector>

template<class T> struct Fenwick {
    int n;
    std::vector<T> s;

    Fenwick(int n) : n(n), s(n) {}
    void modify(int x, const T &k) {
        while (x < n) {
            s[x] += k;
            x += ~x & (x + 1);
        }
    }
    T query(int x) {
        T res = 0;
        while (~x) {
            res += s[x];
            x -= ~x & (x + 1);
        }
        return res;
    }
};

template<class T> struct FenwickCdq {
    int n;
    std::vector<std::vector<T>> s;

    FenwickCdq(int n) : n(n), s(n) {}
    void modify(int x, const T &k) {
        while (x < n) {
            s[x].push_back(k);
            x += ~x & (x + 1);
        }
    }
    void query(int x, const T &k) {
        while (~x) {
            s[x].push_back(k);
            x -= ~x & (x + 1);
        }
    }
};

// Old

struct Fenwick {
    int n;
    std::vector<int> s;
    Fenwick(int _n) : n(_n), s(_n) {}
    void add(int x, int k) {
        for (int i = x + 1; i <= n; i += i & -i) s[i - 1] += k;
    }
    int query(int x) {
        int res = 0;
        for (int i = x + 1; i; i -= i & -i) res += s[i - 1];
        return res;
    }
};

template<typename T> struct Fenwick {
    int n;
    std::vector<T> s;
    Fenwick(int _n) : n(_n), s(_n) {}
    void add(int x, T k) {
        for (int i = x + 1; i <= n; i += i & -i) s[i - 1] += k;
    }
    T query(int x) {
        T res = 0;
        for (int i = x + 1; i; i -= i & -i) res += s[i - 1];
        return res;
    }
};
