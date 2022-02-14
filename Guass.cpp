
int guass_det(Matrix a) {
    const int n = a.size();
    int det = 1;
    for (int i = 0; i < n; ++i) {
        if (a[i][i] == 0) {
            for (int j = i + 1; j < n; ++j) {
                if (a[j][i]) {
                    std::swap(a[i], a[j]);
                    det = norm(P - det);
                    break;
                }
            }
            if (a[i][i] == 0) return 0;
        }
        det = 1ll * det * a[i][i] % P;
        int inv = inverse(a[i][i]);
        for (int j = i + 1; j < n; ++j) {
            int d = 1ll * a[j][i] * inv % P;
            for (int k = i; k < n; ++k) {
                a[j][k] = norm(a[j][k] - 1ll * a[i][k] * d % P + P);
            }
        }
    }
    return det;
}
