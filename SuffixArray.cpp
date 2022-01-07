// SA

std::vector<int> get_sa(std::string s) {
    s = s + '#';
    int n = s.length();
    std::vector<int> sa(n), rk(n);
    std::iota(sa.begin(), sa.end(), 0);
    std::sort(sa.begin(), sa.end(), [&](int i, int j) { return s[i] < s[j]; });
    rk[sa[0]] = 0;
    for (int i = 1; i < n; ++i)
        rk[sa[i]] = rk[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);

    std::vector<int> tp(n), tax(n);
    int len = 1;
    while (rk[sa[n - 1]] < n - 1) {
        for (int i = 0; i < len; ++i) tp[i] = n - len + i;
        int cnt = len;
        for (int i = 0; i < n; ++i)
            if (sa[i] >= len) tp[cnt++] = sa[i] - len;

        tax.assign(n, 0);
        for (int i = 0; i < n; ++i) ++tax[rk[i]];
        for (int i = 1; i < n; ++i) tax[i] += tax[i - 1];
        for (int i = n - 1; ~i; --i) sa[--tax[rk[tp[i]]]] = tp[i];

        std::swap(tp, rk);
        rk[sa[0]] = 0;
        for (int i = 1; i < n; ++i)
            rk[sa[i]] = rk[sa[i - 1]] + (tp[sa[i]] != tp[sa[i - 1]] || tp[sa[i] + len] != tp[sa[i - 1] + len]);

        len *= 2;
    }
    sa.erase(std::find(sa.begin(), sa.end(), n - 1));
    return sa;
}


// SA & Height

std::pair<std::vector<int>, std::vector<int>> get_sa(std::string s) {
    s.push_back('#');
    int n = s.length();
    std::vector<int> sa(n);
    std::iota(sa.begin(), sa.end(), 0);
    std::sort(sa.begin(), sa.end(), [&](int i, int j) { return s[i] < s[j]; });
    std::vector<int> rk(n);
    rk[sa[0]] = 0;
    for (int i = 1; i < n; ++i) rk[sa[i]] = rk[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);
    std::vector<int> tp(n), tax;
    for (int len = 1; rk[sa[n - 1]] < n - 1; len *= 2) {
        int p = 0;
        for (int i = n - len; i < n; ++i) tp[p++] = i;
        for (int i = 0; i < n; ++i) {
            if (sa[i] >= len) tp[p++] = sa[i] - len;
        }
        tax.assign(n, 0);
        for (int i = 0; i < n; ++i) ++tax[rk[i]];
        for (int i = 1; i < n; ++i) tax[i] += tax[i - 1];
        for (int i = n - 1; ~i; --i) sa[--tax[rk[tp[i]]]] = tp[i];
        std::swap(rk, tp);
        rk[sa[0]] = 0;
        for (int i = 1; i < n; ++i) rk[sa[i]] = rk[sa[i - 1]] + (tp[sa[i - 1]] != tp[sa[i]] || tp[sa[i - 1] + len] != tp[sa[i] + len]);
    }
    sa.erase(std::find(sa.begin(), sa.end(), n - 1));

    --n;
    for (int i = 0; i < n; ++i) rk[sa[i]] = i;
    std::vector<int> ht(n);
    int k = 0;
    for (int i = 0; i < n; ++i) {
        if (rk[i] == 0) {
            k = 0;
        } else {
            if (k) --k;
            while (s[i + k] == s[sa[rk[i] - 1] + k]) ++k;
            ht[rk[i]] = k;
        }
    }
    return {sa, ht};
}

