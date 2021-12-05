#include <vector>

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