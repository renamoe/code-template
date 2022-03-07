template<class T> struct SegmentTree {
    std::vector<T> s;

    SegmentTree(int n) : s(n * 4) {}
    void modify(int u, int l, int r, int p, const T &k) {
        if (r - l == 1) {
            s[u] = k;
            return;
        }
        int mid = (l + r) / 2;
        if (p < mid) {
            modify(u * 2, l, mid, p, k);
        } else {
            modify(u * 2 + 1, mid, r, p, k);
        }
        s[u] = s[u * 2] + s[u * 2 + 1];
    }
    T query(int u, int l, int r, int L, int R) {
        if (L <= l && r <= R) {
            return s[u];
        }
        int mid = (l + r) / 2;
        if (L >= mid) return query(u * 2 + 1, mid, r, L, R);
        if (R <= mid) return query(u * 2, l, mid, L, R);
        return query(u * 2, l, mid, L, R) + query(u * 2 + 1, mid, r, L, R);
    }
};
