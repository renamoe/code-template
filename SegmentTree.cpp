template<class Info,
    class Merge = std::plus<Info>>
struct SegmentTree {
private:
    const Merge merge;
    int n;
    std::vector<Info> f;

private:
    void modify(int u, int l, int r, int p, const Info &k) {
        if (r - l == 1) {
            f[u] = k;
            return;
        }
        int mid = (l + r) / 2;
        if (p < mid) {
            modify(u * 2, l, mid, p, k);
        } else {
            modify(u * 2 + 1, mid, r, p, k);
        }
        f[u] = merge(f[u * 2], f[u * 2 + 1]);
    }
    Info query(int u, int l, int r, int L, int R) {
        if (L <= l && r <= R) {
            return f[u];
        }
        int mid = (l + r) / 2;
        if (L >= mid) {
            return query(u * 2 + 1, mid, r, L, R);
        }
        if (R <= mid) {
            return query(u * 2, l, mid, L, R);
        }
        return merge(query(u * 2, l, mid, L, R), query(u * 2 + 1, mid, r, L, R));
    }

public:
    SegmentTree(int n) : merge(Merge()), n(n), f(n * 4) {}
    void modify(int p, const Info &k) {
        if (p < 0 || p >= n) return;
        modify(1, 0, n, p, k);
    }
    Info query(int l, int r) {
        if (l >= r) return Info();
        return query(1, 0, n, l, r);
    }
};


template<class Info, class Tag,
    class Merge = std::plus<Info>>
class LazySegmentTree {
private:
    const Merge merge; 
    int n;
    std::vector<Info> f;
    std::vector<Tag> tag;

private:
    void update(int u) {
        f[u] = merge(f[u * 2], f[u * 2 + 1]);
    }
    void apply(int u, const Tag &k) {
        f[u].apply(k);
        tag[u].apply(k);
    }
    void downdate(int u) {
        apply(u * 2, tag[u]);
        apply(u * 2 + 1, tag[u]);
        tag[u] = Tag();
    }
    void modify(int u, int l, int r, int p, const Info &k) {
        if (r - l == 1) {
            f[u] = k;
            return;
        }
        downdate(u);
        int mid = (l + r) / 2;
        if (p < mid) {
            modify(u * 2, l, mid, p, k);
        } else {
            modify(u * 2 + 1, mid, r, p, k);
        }
        update(u);
    }
    void rangeApply(int u, int l, int r, int L, int R, const Tag &k) {
        if (L <= l && r <= R) {
            apply(u, k);
            return;
        }
        downdate(u);
        int mid = (l + r) / 2;
        if (L < mid) {
            rangeApply(u * 2, l, mid, L, R, k);
        }
        if (R > mid) {
            rangeApply(u * 2 + 1, mid, r, L, R, k);
        }
        update(u);
    }
    Info rangeQuery(int u, int l, int r, int L, int R) {
        if (L <= l && r <= R) {
            return f[u];
        }
        downdate(u);
        int mid = (l + r) / 2;
        if (L >= mid) {
            return rangeQuery(u * 2 + 1, mid, r, L, R);
        }
        if (R <= mid) {
            return rangeQuery(u * 2, l, mid, L, R);
        }
        return merge(rangeQuery(u * 2, l, mid, L, R), rangeQuery(u * 2 + 1, mid, r, L, R));
    }
public:
    LazySegmentTree(int n) : merge(Merge()), n(n), f(n * 4), tag(n * 4) {}
    void modify(int p, const Info &k) {
        if (p < 0 || p >= n) return;
        modify(1, 0, n, p, k);
    }
    void rangeApply(int l, int r, const Tag &k) {
        if (l >= r) return;
        rangeApply(1, 0, n, l, r, k);
    }
    Info rangeQuery(int l, int r) {
        if (l >= r) return Info();
        return rangeQuery(1, 0, n, l, r);
    }
};
