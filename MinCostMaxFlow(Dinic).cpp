#include <vector>
#include <numeric>
#include <algorithm>

template<typename FT, typename CT> struct Graph {
    static constexpr FT Inf = std::numeric_limits<FT>::max() / 3;
    std::vector<int> head;
    struct Edge {
        int nxt, to;
        FT flow;
        CT cost;
    };
    std::vector<Edge> e;
    Graph(int n) : head(n, -1) {}
    void add(int u, int v, FT f, CT c) {
        e.emplace_back(Edge{head[u], v, f, c});
        head[u] = e.size() - 1;
    }
    void add_flow(int u, int v, FT f, CT c) {
        add(u, v, f, c);
        add(v, u, 0, -c);
    }
};

template<typename FT, typename CT> class Dinic {
private:
    int n;
    Graph<FT, CT> &G;
    std::vector<CT> dep;
    std::vector<int> inq;
    std::vector<int> cur, ins;
    FT max_flow;
    CT min_cost;
    CT spfa(int S, int T) {
        std::fill_n(dep.begin(), n, -1);
        std::fill_n(inq.begin(), n, 0);
        static std::queue<int> q;
        dep[S] = 0;
        q.push(S);
        inq[S] = true;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = false;
            for (int i = G.head[u]; ~i; i = G.e[i].nxt) {
                int v = G.e[i].to;
                if (G.e[i].flow && (dep[v] == -1 || dep[v] > dep[u] + G.e[i].cost)) {
                    dep[v] = dep[u] + G.e[i].cost;
                    if (!inq[v]) {
                        q.push(v);
                        inq[v] = true;
                    }
                }
            }
        }
        return dep[T];
    }
    FT dfs(int u, int T, FT lim) {
        if (u == T || !lim) return lim;
        ins[u] = true;
        FT rest = lim;
        for (int &i = cur[u]; ~i; i = G.e[i].nxt) {
            int v = G.e[i].to;
            if (!ins[v] && G.e[i].flow && dep[v] == dep[u] + G.e[i].cost) {
                FT rlow = dfs(v, T, std::min(G.e[i].flow, rest));
                G.e[i].flow -= rlow;
                G.e[i ^ 1].flow += rlow;
                rest -= rlow;
                min_cost += rlow * G.e[i].cost;
                if (!rlow) dep[v] = -1;
            }
        }
        ins[u] = false;
        return lim - rest;
    }
public:
    Dinic(int n, Graph<FT, CT> &G)
       : n(n), G(G), dep(n), inq(n), cur(n), ins(n) {}
    std::pair<CT, FT> mcmf(int S, int T) {
        max_flow = 0;
        min_cost = 0;
        FT rlow;
        while (spfa(S, T) != -1) {
            std::copy_n(G.head.begin(), n, cur.begin());
            while (rlow = dfs(S, T, Graph<FT, CT>::Inf), rlow > 0) max_flow += rlow;
        }
        return {min_cost, max_flow};
    }
};