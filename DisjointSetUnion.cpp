#include <vector>

// vector 版

struct DSU {
    std::vector<int> fa;

    DSU(int n) : fa(n, -1) {}
    int find(int x) {
        if (fa[x] == -1) return x;
        return fa[x] = find(fa[x]);
    }
    bool join(int x, int y) {
        x = find(x);
        y = find(y);
        if (x != y) {
            fa[x] = y;
            return true;
        }
        return false;
    }
};

// 数组版

template<int N> struct DSU {
    int fa[N];

    void init(int n) {
        std::fill_n(fa, n, -1);
    }
    int find(int x) {
        if (fa[x] == -1) return x;
        return fa[x] = find(fa[x]);
    }
    bool join(int x, int y) {
        x = find(x);
        y = find(y);
        if (x != y) {
            fa[x] = y;
            return true;
        }
        return false;
    }
};
