template<class T = int> struct Dinic {
    static constexpr T Inf = std::numeric_limits<T>::max();
    struct Edge {
        int to;
        T cap;
    };
    int n;
    std::vector<std::vector<int>> g;
    std::vector<Edge> e;
    std::vector<int> cur;
    std::vector<int> h;

    Dinic(int n) : n(n), g(n) {}
    void addFlow(int u, int v, T w) {
        g[u].push_back(e.size());
        e.push_back({v, w});
        g[v].push_back(e.size());
        e.push_back({u, 0});
    }
    bool bfs(int s, int t) {
        h.assign(n, -1);
        std::queue<int> q;
        h[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i : g[u]) {
                int v = e[i].to;
                if (e[i].cap > 0 && h[v] == -1) {
                    h[v] = h[u] + 1;
                    if (v == t) return true;
                    q.push(v);
                }
            }
        }
        return false;
    }
    T dfs(int u, int t, T f) {
        if (u == t || !f) {
            return f;
        }
        T r = f;
        for (int &i = cur[u]; i < int(g[u].size()); ++i) {
            int j = g[u][i];
            int v = e[j].to;
            if (e[j].cap > 0 && h[v] == h[u] + 1) {
                T l = dfs(v, t, std::min(r, e[j].cap));
                e[j].cap -= l;
                e[j ^ 1].cap += l;
                r -= l;
                if (!r) break;
            }
        }
        return f - r;
    }
    T maxFlow(int s, int t) {
        T res = 0;
        while (bfs(s, t)) {
            cur.assign(n, 0);
            res += dfs(s, t, Inf);
        }
        return res;
    }
};


// old

template<typename FT> struct Graph {
    static constexpr FT Inf = std::numeric_limits<FT>::max() / 3;
    std::vector<int> head;
    struct Edge {
        int nxt, to;
        FT flow;
    };
    std::vector<Edge> e;
    Graph(int n) : head(n, -1) {}
    void add(int u, int v, FT w) {
        e.emplace_back(Edge{head[u], v, w});
        head[u] = e.size() - 1;
    }
    void addFlow(int u, int v, FT w) {
        add(u, v, w);
        add(v, u, 0);
    }
};

template<typename FT> class Dinic {
private:
    int n;
    Graph<FT> &G;
    std::vector<int> dep, cur;
    bool bfs(int S, int T) {
        std::fill_n(dep.begin(), n, -1);
        static std::queue<int> q;
        dep[S] = 0;
        q.push(S);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = G.head[u]; ~i; i = G.e[i].nxt) {
                int v = G.e[i].to;
                if (G.e[i].flow && dep[v] == -1) {
                    dep[v] = dep[u] + 1;
                    q.push(v);
                }
            }
        }
        return dep[T] != -1;
    }
    FT dfs(int u, int T, FT lim) {
        if (u == T || !lim) return lim;
        FT rest = lim;
        for (int &i = cur[u]; ~i; i = G.e[i].nxt) {
            int v = G.e[i].to;
            if (G.e[i].flow && dep[v] == dep[u] + 1) {
                FT rlow = dfs(v, T, std::min(G.e[i].flow, rest));
                if (!rlow) dep[v] = -1;
                G.e[i].flow -= rlow;
                G.e[i ^ 1].flow += rlow;
                rest -= rlow;
                if (!rest) break;
            }
        }
        return lim - rest;
    }
public:
    Dinic(int n, Graph<FT> &G) : n(n), G(G), dep(n), cur(n) {}
    FT maxFlow(int S, int T) {
        FT res = 0;
        FT rlow;
        while (bfs(S, T)) {
            std::copy_n(G.head.begin(), n, cur.begin());
            while (rlow = dfs(S, T, Graph<FT>::Inf), rlow > 0) res += rlow;
        }
        return res;
    }
};
