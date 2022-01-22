
// 输入：整形
// 输出：整形，字符

class FastIO {
private:
    static constexpr int N = 1 << 19 | 1;
    char ibuf[N];
    char *p, *q;
    char obuf[N];
    int o, sta[50];
    char next_char() {
        if (p == q) {
            p = ibuf;
            q = ibuf + std::fread(ibuf, 1, sizeof ibuf, stdin);
        }
        return p == q ? EOF : *p++;
    }
    void write_char(char c) {
        obuf[o++] = c;
        if (o == N) flush();
    }
    void flush() {
        std::fwrite(obuf, 1, o, stdout);
        o = 0;
    }
public:
    FastIO() : p(ibuf), q(ibuf), o(0) {}
    ~FastIO() { flush(); }
    template<typename T>
    FastIO &operator >>(T &x) {
        x = 0;
        char c = next_char();
        bool sign = false;
        while (!std::isdigit(c)) {
            sign |= c == '-';
            c = next_char();
        }
        while (std::isdigit(c)) {
            x = x * 10 + (c - '0');
            c = next_char();
        }
        if (sign) x = -x;
        return *this;
    }
    FastIO &operator <<(char c) {
        write_char(c);
        return *this;
    }
    template<typename T>
    FastIO &operator <<(T x) {
        if (x < 0) {
            write_char('-');
            x = -x;
        }
        int top = 0;
        do {
            sta[++top] = x % 10;
            x /= 10;
        } while (x);
        while (top) {
            write_char(sta[top] + '0');
            --top;
        }
        return *this;
    }
} io;
