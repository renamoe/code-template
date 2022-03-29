// from https://loj.ac/s/777808

template <class T> struct push_relabel {
    struct arc {
        int to, rev;
        T cap;
    };
    const T inf = std::numeric_limits<T>::max();
    const int n;
    std::vector<std::tuple<int, int, T, T>> arc_pool;
    std::vector<arc> g;
    std::vector<int> head, ptr, d;
    std::vector<T> ex;
    std::vector<std::vector<int>> active;
    std::vector<std::set<int>> vs;
    int highest, works;
    push_relabel(int _n) :
        n(_n), head(n + 1), ptr(n), d(n), ex(n), active(n), vs(n) {}
    void add(int from, int to, T cap, bool directed = true) {
        assert(cap >= 0);
        if (from == to or cap == 0) return;
        arc_pool.emplace_back(from, to, cap, directed ? 0 : cap);
        ++head[from], ++head[to];
    }
    void build() {
        for (int v = 0; v < n; ++v) head[v + 1] += head[v];
        g.resize(head[n]);
        for (auto&& e : arc_pool) {
            int i = --head[std::get<0>(e)], j = --head[std::get<1>(e)];
            g[i] = {std::get<1>(e), j, std::get<2>(e)}, g[j] = {std::get<0>(e), i, std::get<3>(e)};
        }
    }
    void push(int v, arc& a) {
        if (ex[a.to] == 0) active[d[a.to]].push_back(a.to);
        T delta = std::min(ex[v], a.cap);
        ex[v] -= delta, ex[a.to] += delta;
        a.cap -= delta, g[a.rev].cap += delta;
    }
    void relabel(int v) {
        ++works;
        int mn = 2 * n;
        for (int i = head[v]; i < head[v + 1]; ++i)
            if (g[i].cap) mn = std::min(mn, d[g[i].to]);
        if (vs[d[v]].size() == 1) {
            for (int i = d[v]; i < n; ++i) {
                for (int u : vs[i]) d[u] = n;
                active[i].clear(), vs[i].clear();
            }
            return;
        }
        vs[d[v]].erase(v);
        if ((d[v] = mn + 1) < n) vs[highest = d[v]].insert(v);
    }
    void discharge(int v) {
        int i = ptr[v];
        while (ex[v]) {
            if (i == head[v + 1]) {
                relabel(v), i = head[v];
                if (d[v] >= n) break;
            } else {
                if (g[i].cap and d[v] > d[g[i].to]) push(v, g[i]);
                else ++i;
            }
        }
        ptr[v] = i;
    }
    void global_relabel(int t) {
        copy(begin(head), begin(head) + n, begin(ptr));
        fill(begin(d), end(d), n);
        for (int i = 0; i < n; ++i) active[i].clear(), vs[i].clear();
        highest = -1, works = 0;
        std::queue<int> que;
        d[t] = 0, que.push(t);
        while (not que.empty()) {
            int v = que.front();
            que.pop();
            vs[d[v]].insert(v);
            for (int i = head[v]; i < head[v + 1]; ++i)
                if (g[g[i].rev].cap and d[g[i].to] == n) {
                    d[g[i].to] = d[v] + 1, que.push(g[i].to);
                    if (ex[g[i].to]) active[highest = d[g[i].to]].push_back(g[i].to);
                }
        }
    }
    T max_flow(int s, int t) {
        build();
        ex[s] = inf, ex[t] = -inf;
        for (int i = head[s]; i < head[s + 1]; ++i) push(s, g[i]);
        global_relabel(t);
        for (; highest >= 0; --highest) while (not active[highest].empty()) {
            int v = active[highest].back();
            active[highest].pop_back();
            discharge(v);
            if (works > 4 * n) global_relabel(t);
        }
        return inf + ex[t];
    }
};
