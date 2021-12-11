#include <vector>
#include <algorithm>

using Graph = std::vector<std::vector<int>>;

class Blossom {
private:
    int n;
    const Graph &G;
    std::vector<int> col, pre, bel, dfn;
    int ql, qr;
    std::vector<int> q;
public:
    std::vector<int> h;

private:
    void flip(int u) {
        if (~u && ~pre[u]) {
            flip(h[pre[u]]);
            h[u] = pre[u];
            h[pre[u]] = u;
        }
    }
    int find(int u) {
        return bel[u] == -1 ? u : (bel[u] = find(bel[u]));
    }
    int lca(int u, int v) {
        u = find(u);
        v = find(v);
        while (u != v) {
            if (dfn[u] < dfn[v]) std::swap(u, v);
            u = find(pre[h[u]]);
        }
        return u;
    }
    void push(int x) {
        dfn[x] = qr;
        q[qr++] = x;
    }
    void blossom(int u, int v, int p) {
        while (find(u) != p) {
            pre[u] = v;
            v = h[u];
            bel[u] = bel[v] = p;
            if (col[v] == 1) {
                col[v] = 0;
                push(v);
            }
            u = pre[v];
        }
    }
    bool match(int s) {
        ql = qr = 0;
        std::fill_n(col.begin(), n, -1);
        std::fill_n(pre.begin(), n, -1);
        std::fill_n(dfn.begin(), n, -1);
        std::fill_n(bel.begin(), n, -1);

        push(s);
        col[s] = 0;
        while (ql < qr) {
            int u = q[ql++];
            for (int v : G[u]) {
                if (col[v] == -1) {
                    pre[v] = u;
                    col[v] = 1;
                    if (h[v] == -1) {
                        flip(v);
                        return true;
                    }
                    col[h[v]] = 0;
                    push(h[v]);
                } else if (col[v] == 0) {
                    int p = lca(u, v);
                    blossom(u, v, p);
                    blossom(v, u, p);
                }
            }
        }
        return false;
    }
public:
    Blossom(int n, const Graph &G)
       : n(n), G(G), col(n), pre(n), bel(n), dfn(n), q(n), h(n, -1) {}
    int max_match() {
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            if (h[i] == -1 && match(i)) ++ans;
        }
        return ans;
    }
};