template<class T = int>
struct Dinic {
    static constexpr int InfF = std::numeric_limits<int>::max();
    static constexpr T InfC = std::numeric_limits<T>::max();

    struct Edge {
        int to;
        int cap;
        T cost;
    };

    int n;
    std::vector<std::vector<int>> g;
    std::vector<Edge> e;
    std::vector<T> d;
    std::vector<int> cur;
    std::vector<int> vis;
    T sumC;

    Dinic(int n) : n(n), g(n) {}
    void add(int u, int v, int f, T c) {
        g[u].push_back(e.size());
        e.push_back({v, f, c});
        g[v].push_back(e.size());
        e.push_back({u, 0, -c});
    }
    bool spfa(int s, int t) {
        d.assign(n, InfC);
        vis.assign(n, false);
        std::queue<int> q;
        q.push(s);
        d[s] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            vis[u] = false;
            for (int i : g[u]) {
                int v = e[i].to;
                if (e[i].cap && d[v] > d[u] + e[i].cost) {
                    d[v] = d[u] + e[i].cost;
                    if (!vis[v]) {
                        vis[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        return d[t] < InfC;
    }
    int dfs(int u, int t, int f) {
        if (u == t || !f) return f;
        vis[u] = true;
        int r = f;
        for (int &j = cur[u]; j < int(g[u].size()); ++j) {
            int i = g[u][j];
            int v = e[i].to;
            if (e[i].cap && d[v] == d[u] + e[i].cost && !vis[v]) {
                int a = dfs(v, t, std::min(f, e[i].cap));
                if (!a) d[v] = InfC;
                e[i].cap -= a;
                e[i ^ 1].cap += a;
                r -= a;
                sumC += a * e[i].cost;
                if (!r) break;
            }
        }
        vis[u] = false;
        return f - r;
    }
    std::pair<int, T> mcmf(int s, int t) {
        int sumF = 0;
        sumC = 0;
        while (spfa(s, t)) {
            cur.assign(n, 0);
            sumF += dfs(s, t, InfF);
        }
        return {sumF, sumC};
    }
};
