void radix_sort(std::vector<int> &a) {
    constexpr int mask = (1 << 8) - 1;
    int n = a.size();
    std::vector<int> b(n);
    int cnt[1 << 8];
    for (int d = 0; d < 32; d += 8) {
        std::memset(cnt, 0, sizeof cnt);
        for (int x : a) {
            ++cnt[(x >> d) & mask];
        }
        for (int i = 1; i < (1 << 8); ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int i = n - 1; ~i; --i) {
            b[--cnt[(a[i] >> d) & mask]] = a[i];
        }
        std::swap(a, b);
    }
}
