
struct DisjointSetUnion {
    DisjointSetUnion(int n) : fa(n) {
        std::iota(fa.begin(), fa.end(), 0);
    }
    int find(int x) {
        return fa[x] == x ? x : (fa[x] = find(fa[x]));
    }
    std::vector<int> fa;
};