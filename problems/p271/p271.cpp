#include <bits/stdc++.h>
// #include <coding_library/data_structures/treap_lazy.hpp>

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

template<
    class KeyT, class T, T (*merge_func)(T, T), class LazyT, uint64_t (*rng)()>
struct TreapNode {
    KeyT key;
    T data, subtree;
    uint64_t prior;
    size_t size;
    TreapNode *left, *right;
    LazyT lazy;

    TreapNode(KeyT key, T data)
        : key(key), data(data), left(nullptr), right(nullptr), size(1) {
        prior = rng();
        lazy = LazyT();
    }

    void pull() {
        subtree = data;
        size = 1;
        if(left) {
            left->push();
            subtree = merge_func(left->subtree, subtree);
            size += left->size;
        }
        if(right) {
            right->push();
            subtree = merge_func(subtree, right->subtree);
            size += right->size;
        }
    }

    void push() { lazy.apply_lazy(this); }

    friend void push_lazy(TreapNode* t) {
        if(t) {
            t->push();
        }
    }

    friend pair<TreapNode*, TreapNode*> split(TreapNode* t, KeyT key) {
        if(!t) {
            return {nullptr, nullptr};
        }

        t->push();
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

        t->push();
        size_t left_size = t->left ? t->left->size : 0;
        if(left_size >= size) {
            auto [left, t_left] = split_by_size(t->left, size);
            t->left = t_left;
            t->pull();
            return {left, t};
        } else {
            auto [t_right, right] = split_by_size(
                t->right, size - 1 - left_size
            );
            t->right = t_right;
            t->pull();
            return {t, right};
        }
    }

    friend TreapNode* merge(TreapNode* l, TreapNode* r) {
        push_lazy(l);
        push_lazy(r);
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
        push_lazy(l);
        push_lazy(r);
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
        } else {
            t->push();
            if(it->prior > t->prior) {
                auto [t1, t2] = split(t, it->key);
                it->left = t1;
                it->right = t2;
                t = it;
            } else {
                insert_in(it->key < t->key ? t->left : t->right, it);
            }
        }
        t->pull();
    }

    friend TreapNode* erase_from(
        TreapNode*& t, KeyT key, bool delete_node = false
    ) {
        t->push();
        T return_data;
        if(t->key == key) {
            auto tmp = t;
            t = merge(t->left, t->right);

            return_data = tmp->data;
            if(delete_node) {
                delete tmp;
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

template<class KeyT, class T, T (*merge_func)(T, T), class LazyT>
class Treap {
  public:
    static uint64_t rng() {
        // static mt19937_64 static_rng(random_device{}());
        // FOR DEBUG:
        static mt19937_64 static_rng(42);
        return static_rng();
    }

    using Node = TreapNode<KeyT, T, merge_func, LazyT, Treap::rng>;

    void _pull_all(Node* t) {
        if(t) {
            t->push();
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

    void erase(KeyT key) { return erase_from(root, key); }

    friend Treap<KeyT, T, merge_func, LazyT> merge_treaps(
        Treap<KeyT, T, merge_func, LazyT> l, Treap<KeyT, T, merge_func, LazyT> r
    ) {
        Treap<KeyT, T, merge_func, LazyT> res;
        res.root = unordered_merge(l.root, r.root);
        return res;
    }
};

template<class T>
struct ReverseLazy {
    bool should_reverse;

    ReverseLazy() { should_reverse = false; }

    template<class G, uint64_t (*rng)(), T (*merge_func)(T, T)>
    void apply_lazy(TreapNode<G, T, merge_func, ReverseLazy, rng>* node) {
        if(!node || !should_reverse) {
            return;
        }

        swap(node->left, node->right);
        if(node->left) {
            node->left->lazy.should_reverse ^= true;
        }
        if(node->right) {
            node->right->lazy.should_reverse ^= true;
        }

        should_reverse = false;
    }
};

struct EmptyMonoid {
    static EmptyMonoid merge(EmptyMonoid a, EmptyMonoid b) {
        return EmptyMonoid();
    }
};

using TreapWithReverse =
    Treap<string, EmptyMonoid, EmptyMonoid::merge, ReverseLazy<EmptyMonoid>>;

int n, m, k;
vector<string> names;

void read() {
    cin >> n >> m >> k;
    names.resize(n);
    cin >> names;
}

void solve() {
    vector<pair<string, EmptyMonoid>> init_treap_data;
    for(auto name: names) {
        init_treap_data.emplace_back(name, EmptyMonoid());
    }

    TreapWithReverse treap(init_treap_data);

    while(m--) {
        string txt;
        cin >> txt;
        if(txt[0] == 'A') {
            string name;
            int state = 0;
            for(char c: txt) {
                if(c == '(') {
                    state++;
                } else if(c == ')') {
                    state++;
                } else if(state == 1) {
                    name.push_back(c);
                }
            }

            auto new_node = new TreapWithReverse::Node(name, EmptyMonoid());
            treap.root = merge(new_node, treap.root);
        } else {
            auto [t1, t2] = split_by_size(treap.root, k); 
            if(t1) {
                t1->lazy.should_reverse ^= true;
            }
            treap.root = merge(t1, t2);
        }
    }

    function<void(TreapWithReverse::Node*)> dfs =
        [&](TreapWithReverse::Node* node) {
            if(node) {
                node->push();
                dfs(node->left);
                cout << node->key << endl;
                dfs(node->right);
            }
        };

    dfs(treap.root);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
