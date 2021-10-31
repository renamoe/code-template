#include <queue>
#include <vector>

struct AhoCorasick {
    static constexpr int SIG = 26;
    struct Node {
        Node *ch[SIG];
        Node *fail;
        // some infomation
        Node() : ch{}, fail{} {}
    };
    std::vector<Node> pool;
    int cnt_pool;
    Node *root;
    AhoCorasick(int size) : pool(size + 1), cnt_pool(1), root(pool.data()) {}
    Node *new_node() {
        return pool.data() + (cnt_pool++);
    }
    Node *insert(const std::string &s) {
        Node *p = root;
        for (char _c : s) {
            int c = _c - 'a';
            if (!p->ch[c]) p->ch[c] = new_node();
            p = p->ch[c];
        }
        return p;
    }
    void build_fail() {
        std::queue<Node *> q;
        for (int i = 0; i < SIG; ++i) {
            if (root->ch[i]) {
                root->ch[i]->fail = root;
                q.push(root->ch[i]);
            } else {
                root->ch[i] = root;
            }
        }
        while (!q.empty()) {
            Node *u = q.front();
            q.pop();
            for (int i = 0; i < SIG; ++i) {
                if (u->ch[i]) {
                    u->ch[i]->fail = u->fail->ch[i];
                    q.push(u->ch[i]);
                } else {
                    u->ch[i] = u->fail->ch[i];
                }
            }
        }
    }
};