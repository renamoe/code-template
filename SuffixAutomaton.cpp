struct SuffixAutomaton {
    struct Node {
        Node *fa;
        Node *ch[Sig];
        int len;
        int pos;
        Node() : fa{}, ch{}, len(0), pos(-1) {}
    };
    std::vector<Node> pool;
    int tot;
    Node *root;

    SuffixAutomaton(const std::string &s) {
        pool.resize(s.length() * 2 + 1);
        tot = 0;
        root = new_node();
        Node *last = root;
        for (int i = 0; i < int(s.length()); ++i) {
            last = extend(last, s[i] - '0', i);
        }
    }
    Node *new_node() {
        return pool.data() + (tot++);
    }
    int id(Node *p) {
        return p - pool.data();
    }
    Node *node(int k) {
        return pool.data() + k;
    }
    Node *extend(Node *p, int c, int pos) {
        Node *np = new_node();
        np->len = p->len + 1;
        np->pos = pos;
        while (p && !p->ch[c]) {
            p->ch[c] = np;
            p = p->fa;
        }
        if (!p) {
            np->fa = root;
        } else {
            Node *q = p->ch[c];
            if (q->len == p->len + 1) {
                np->fa = q;
            } else {
                Node *nq = new_node();
                nq->fa = q->fa;
                std::copy_n(q->ch, Sig, nq->ch);
                nq->len = p->len + 1;
                q->fa = np->fa = nq;
                while (p && p->ch[c] == q) {
                    p->ch[c] = nq;
                    p = p->fa;
                }
            }
        }
        return np;
    }
};
