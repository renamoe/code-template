#include <queue>

template<class T> class DeleteHeap {
private:
    std::priority_queue<T> add, del;
    void clean() {
        while (!del.empty() && add.top() == del.top()) {
            add.pop();
            del.pop();
        }
    }
public:
    void push(const T &x) {
        add.push(x);
    }
    bool empty() {
        clean();
        return add.empty();
    }
    T top() {
        clean();
        return add.top();
    }
    void pop() {
        clean();
        add.pop();
    }
    void erase(const T &x) {
        del.push(x);
    }
};
