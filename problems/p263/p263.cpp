#include <bits/stdc++.h>
// #include <coding_library/data_structures/treap.hpp>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

template<class KeyT, class T, T (*merge_func)(T, T), uint64_t (*rng)()>
struct TreapNode {
    KeyT key;
    T data, subtree;
    uint64_t prior;
    size_t size;
    TreapNode *left, *right;

    TreapNode(KeyT key, T data)
        : key(key), data(data), left(nullptr), right(nullptr), size(1) {
        prior = rng();
    }

    void pull() {
        subtree = data;
        size = 1;
        if(left) {
            subtree = merge_func(left->subtree, subtree);
            size += left->size;
        }
        if(right) {
            subtree = merge_func(subtree, right->subtree);
            size += right->size;
        }
    }

    friend pair<TreapNode*, TreapNode*> split(TreapNode* t, KeyT key) {
        if(!t) {
            return {nullptr, nullptr};
        }
        if(key < t->key) {
            auto [left, t_left] = split(t->left, key);
            t->left = t_left;
            t->pull();
            return {left, t};
        } else {
            auto [t_right, right] = split(t->right, key);
            t->right = t_right;
            t->pull();
            return {t, right};
        }
    }

    friend pair<TreapNode*, TreapNode*> split_by_size(
        TreapNode* t, size_t size
    ) {
        if(!t) {
            return {nullptr, nullptr};
        }

        size_t left_size = t->left ? t->left->size : 0;
        if(left_size >= size) {
            auto [left, t_left] = split_by_size(t->left, size);
            t->left = t_left;
            t->pull();
            return {left, t};
        } else {
            auto [t_right, right] =
                split_by_size(t->right, size - 1 - left_size);
            t->right = t_right;
            t->pull();
            return {t, right};
        }
    }

    friend TreapNode* merge(TreapNode* l, TreapNode* r) {
        if(!l || !r) {
            return l ? l : r;
        } else if(l->prior > r->prior) {
            l->right = merge(l->right, r);
            l->pull();
            return l;
        } else {
            r->left = merge(l, r->left);
            r->pull();
            return r;
        }
    }

    friend TreapNode* unordered_merge(TreapNode* l, TreapNode* r) {
        if(!l) {
            return r;
        }
        if(!r) {
            return l;
        }
        if(l->prior < r->prior) {
            swap(l, r);
        }
        auto [t1, t2] = split(r, l->key);
        l->left = unordered_merge(l->left, t1);
        l->right = unordered_merge(l->right, t2);
        l->pull();
        return l;
    }

    friend void insert_in(TreapNode*& t, TreapNode* it) {
        if(!t) {
            t = it;
        } else if(it->prior > t->prior) {
            auto [t1, t2] = split(t, it->key);
            it->left = t1;
            it->right = t2;
            t = it;
        } else {
            insert_in(it->key < t->key ? t->left : t->right, it);
        }
        t->pull();
    }

    friend TreapNode* erase_from(
        TreapNode*& t, KeyT key, bool delete_node = false
    ) {
        T return_data;
        if(t->key == key) {
            auto tmp = t;
            t = merge(t->left, t->right);

            return_data = tmp->data;
            if(delete_node) {
                delete tmp;
                return_data = nullptr;
            }
        } else {
            return_data =
                erase_from(key < t->key ? t->left : t->right, key, delete_node);
        }
        if(t) {
            t->pull();
        }
        return return_data;
    }
};

template<class KeyT, class T, T (*merge_func)(T, T)>
class Treap {
  public:
    static uint64_t rng() {
        static mt19937_64 static_rng(random_device{}());
        return static_rng();
    }

    using Node = TreapNode<KeyT, T, merge_func, Treap::rng>;

    void _pull_all(Node* t) {
        if(t) {
            _pull_all(t->left);
            _pull_all(t->right);
            t->pull();
        }
    }

    Node* root;

    Treap() { root = nullptr; }
    Treap(const vector<pair<KeyT, T>>& a) { build_cartesian_tree(a); }

    void build_cartesian_tree(const vector<pair<KeyT, T>>& a) {
        vector<Node*> st;

        function<Node*(Node*)> recycle_stack = [&](Node* last) {
            Node* new_last = st.back();
            st.pop_back();
            new_last->right = last;
            return new_last;
        };

        for(const auto& [key, val]: a) {
            Node* new_node = new Node(key, val);
            Node* last = nullptr;
            while(!st.empty() && st.back()->prior < new_node->prior) {
                last = recycle_stack(last);
            }

            new_node->left = last;
            st.push_back(new_node);
        }

        root = nullptr;
        while(!st.empty()) {
            root = recycle_stack(root);
        }

        _pull_all(root);
    }

    void insert(KeyT key, T data) {
        Node* new_node = new Node(key, data);
        insert_in(root, new_node);
    }

    Node* erase(KeyT key) { return erase_from(root, key); }

    friend Treap<KeyT, T, merge_func> merge_treaps(
        Treap<KeyT, T, merge_func> l, Treap<KeyT, T, merge_func> r
    ) {
        Treap<KeyT, T, merge_func> res;
        res.root = unordered_merge(l.root, r.root);
        return res;
    }

    int count_leq(KeyT max_key) {
        int cnt = 0;
        Node* cur = root;
        while(cur) {
            if(cur->key <= max_key) {
                cnt += (cur->left ? cur->left->size : 0) + 1;
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }
        return cnt;
    }
};

struct Tower {
    int end;
    int64_t sum;
};

Tower merge_tower(Tower a, Tower b) { return a; }

using TowerTreap = Treap<int, Tower, merge_tower>;
using Node = TowerTreap::Node;

Node* kth_tower(Node* t, int k) {
    while(t) {
        int left_size = t->left ? (int)t->left->size : 0;
        if(k < left_size) {
            t = t->left;
        } else if(k == left_size) {
            return t;
        } else {
            k -= left_size + 1;
            t = t->right;
        }
    }
    return nullptr;
}

Node* find_node(Node* t, int key) {
    while(t) {
        if(t->key == key) {
            return t;
        }
        t = key < t->key ? t->left : t->right;
    }
    return nullptr;
}

Node* pred_node(Node* t, int x) {
    Node* res = nullptr;
    while(t) {
        if(t->key <= x) {
            res = t;
            t = t->right;
        } else {
            t = t->left;
        }
    }
    return res;
}

void erase_node(Node*& t, int key) {
    if(t->key == key) {
        Node* tmp = t;
        t = merge(t->left, t->right);
        delete tmp;
    } else {
        erase_node(key < t->key ? t->left : t->right, key);
    }
    if(t) {
        t->pull();
    }
}

namespace fastio {
constexpr int BUF_SIZE = 1 << 16;
char buf[BUF_SIZE];
int buf_pos = 0, buf_len = 0;

int get_char() {
    if(buf_pos == buf_len) {
        buf_len = (int)fread(buf, 1, BUF_SIZE, stdin);
        buf_pos = 0;
    }
    return buf_pos < buf_len ? (unsigned char)buf[buf_pos++] : EOF;
}

int skip_ws() {
    int c = get_char();
    while(c == ' ' || c == '\n' || c == '\r' || c == '\t') {
        c = get_char();
    }
    return c;
}

int read_int() {
    int c = skip_ws();
    int sign = 1;
    if(c == '-') {
        sign = -1;
        c = get_char();
    } else if(c == '+') {
        c = get_char();
    }
    int x = 0;
    while(c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = get_char();
    }
    return sign * x;
}

int read_word(char* s) {
    int c = skip_ws();
    int len = 0;
    while(c != EOF && c != ' ' && c != '\n' && c != '\r' && c != '\t') {
        s[len++] = (char)c;
        c = get_char();
    }
    s[len] = 0;
    return len;
}

constexpr int OUT_SIZE = 1 << 22;
char out_buf[OUT_SIZE];
int out_pos = 0;

void flush_out() {
    fwrite(out_buf, 1, out_pos, stdout);
    out_pos = 0;
}

void put_char(char c) {
    if(out_pos == OUT_SIZE) {
        flush_out();
    }
    out_buf[out_pos++] = c;
}

void put_str(const char* s) {
    while(*s) {
        put_char(*s++);
    }
}

void put_int(int64_t x) {
    if(x < 0) {
        put_char('-');
        x = -x;
    }
    char tmp[24];
    int n = 0;
    if(x == 0) {
        tmp[n++] = '0';
    }
    while(x) {
        tmp[n++] = (char)('0' + x % 10);
        x /= 10;
    }
    while(n) {
        put_char(tmp[--n]);
    }
}

void put_ordinal(int64_t x) {
    put_int(x);
    put_str("th");
}
}  // namespace fastio

int n;
int height[1000002];
TowerTreap towers;

void do_put(int x, int cubes) {
    Node*& root = towers.root;
    if(height[x] > 0) {
        height[x] += cubes;
        pred_node(root, x)->data.sum += cubes;
        return;
    }

    height[x] = cubes;
    bool left = height[x - 1] > 0;
    bool right = height[x + 1] > 0;
    if(!left && !right) {
        towers.insert(x, Tower{x, (int64_t)cubes});
    } else if(left && !right) {
        Node* lt = pred_node(root, x - 1);
        lt->data.end = x;
        lt->data.sum += cubes;
    } else if(!left && right) {
        Node* rt = find_node(root, x + 1);
        rt->key = x;
        rt->data.sum += cubes;
    } else {
        Node* lt = pred_node(root, x - 1);
        Node* rt = find_node(root, x + 1);
        lt->data.end = rt->data.end;
        lt->data.sum += (int64_t)cubes + rt->data.sum;
        erase_node(root, x + 1);
    }
}

void read() { n = fastio::read_int(); }

void solve() {
    // Cubes are only ever added, never removed, so a filled cell stays filled
    // and a tower can only grow or merge with a neighbour - it never splits.
    // That lets us keep the towers in a treap keyed by their starting cell,
    // one node per tower, while a flat height[] array (cells go up to 10^6)
    // holds the cubes standing on each individual column.
    //
    // Each tower node stores its end cell and its total cube count, so 'cubes'
    // and 'length' of the t-th tower are answered by an order-statistic lookup
    // (the k-th tower by subtree size) reading a single node, and 'towers' is
    // just the number of nodes in the treap.
    //
    // A 'put x c' on an already filled cell only bumps that cell's height and
    // the sum of the tower containing it. On an empty cell we inspect the two
    // neighbours x-1 and x+1: with neither filled we insert a fresh length-one
    // tower; with only the left filled we extend that tower's end to x; with
    // only the right filled we relabel that tower's start down to x by editing
    // its node key, which keeps the BST order because x-1 is empty; with both
    // filled we bridge them, folding the right tower's end and sum into the
    // left node and erasing the right one. 'tput' and 'tcubes' map column x of
    // tower t to cell start+x-1 and reuse the per-cell height.

    char cmd[8];
    for(int i = 0; i < n; i++) {
        fastio::read_word(cmd);
        Node* root = towers.root;
        if(cmd[0] == 'p') {
            int x = fastio::read_int();
            int c = fastio::read_int();
            do_put(x, c);
        } else if(cmd[0] == 'c') {
            int t = fastio::read_int();
            Node* nd = kth_tower(root, t - 1);
            fastio::put_int(nd->data.sum);
            fastio::put_str(" cubes in ");
            fastio::put_ordinal(t);
            fastio::put_str(" tower\n");
        } else if(cmd[0] == 'l') {
            int t = fastio::read_int();
            Node* nd = kth_tower(root, t - 1);
            fastio::put_str("length of ");
            fastio::put_ordinal(t);
            fastio::put_str(" tower is ");
            fastio::put_int(nd->data.end - nd->key + 1);
            fastio::put_char('\n');
        } else if(cmd[1] == 'o') {
            fastio::put_int(root ? (int64_t)root->size : 0);
            fastio::put_str(" towers\n");
        } else if(cmd[1] == 'p') {
            int t = fastio::read_int();
            int x = fastio::read_int();
            int c = fastio::read_int();
            Node* nd = kth_tower(root, t - 1);
            height[nd->key + x - 1] += c;
            nd->data.sum += c;
        } else {
            int t = fastio::read_int();
            int x = fastio::read_int();
            Node* nd = kth_tower(root, t - 1);
            fastio::put_int(height[nd->key + x - 1]);
            fastio::put_str(" cubes in ");
            fastio::put_ordinal(x);
            fastio::put_str(" column of ");
            fastio::put_ordinal(t);
            fastio::put_str(" tower\n");
        }
    }

    fastio::flush_out();
}

int main() {
    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
