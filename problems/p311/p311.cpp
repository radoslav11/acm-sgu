#include <bits/stdc++.h>
#include <functional>

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
        if(t->left && t->left->size >= size) {
            auto [left, t_left] = split_by_size(t->left, size);
            t->left = t_left;
            t->pull();
            return {left, t};
        } else {
            auto [t_right, right] = split_by_size(
                t->right, size - 1 - (t->left ? t->left->size : 0)
            );
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
    Treap(const vector<KeyT, T>& a) { build_cartesian_tree(a); }

    void build_cartesian_tree(const vector<pair<KeyT, T>>& a) {
        root = nullptr;
        vector<Node*> st;
        for(const auto& [key, val]: a) {
            Node* new_node = new Node(key, val);
            Node* last = nullptr;
            while(!st.empty() && st.back()->prior < new_node->prior) {
                last = st.back();
                st.pop_back();
            }

            if(last) {
                new_node->left = last->right;
            }

            if(st.empty()) {
                root = new_node;
            } else {
                if(new_node->val < st.back()->val) {
                    st.back()->left = new_node;
                } else {
                    st.back()->right = new_node;
                }
            }
            st.push_back(new_node);
        }

        _pull_all(root);
    }

    void insert(KeyT key, T data) {
        Node* new_node = new Node(key, data);
        insert_in(root, new_node);
    }

    void erase(KeyT key) { return erase_from(root, key); }

    friend Treap<KeyT, T, merge_func> merge_treaps(
        Treap<KeyT, T, merge_func> l, Treap<KeyT, T, merge_func> r
    ) {
        Treap<KeyT, T, merge_func> res;
        res.root = unordered_merge(l.root, r.root);
        return res;
    }
};

pair<int64_t, int64_t> plus_func(
    pair<int64_t, int64_t> a, pair<int64_t, int64_t> b
) {
    return {a.first + b.first, a.second + b.second};
}

using TreapWithCount = Treap<int64_t, pair<int64_t, int64_t>, plus_func>;
using Node = TreapWithCount::Node;

pair<Node*, Node*> split_by_count(Node* t, int64_t k) {
    if(!t) {
        return {nullptr, nullptr};
    }
    if(t->left && t->left->subtree.first >= k) {
        auto [left, t_left] = split_by_count(t->left, k);
        t->left = t_left;
        t->pull();
        return {left, t};
    } else {
        k -= (t->left ? t->left->subtree.first : 0);
        if(k < t->data.first) {
            Node* new_left = new Node(t->key, {k, k * t->key});
            t->data.first -= k;
            t->data.second = t->data.first * t->key;

            insert_in(t->left, new_left);
            new_left = t->left;
            t->left = nullptr;
            t->pull();
            return {new_left, t};
        }

        auto [t_right, new_right] = split_by_count(t->right, k - t->data.first);
        t->right = t_right;
        t->pull();
        return {t, new_right};
    }
}

void solve() {
    TreapWithCount t;

    string type;
    int64_t arg1, arg2;
    while(cin >> type >> arg1 >> arg2) {
        if(type == "ARRIVE") {
            t.insert(arg2, {arg1, arg1 * arg2});
        } else {
            auto [t1, t2] = split_by_count(t.root, arg1);
            if(!t1 || t1->subtree.second > arg2 || t1->subtree.first != arg1) {
                t.root = merge(t1, t2);
                cout << "UNHAPPY\n";
            } else {
                t.root = t2;
                cout << "HAPPY\n";
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
