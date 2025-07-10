#include <bits/stdc++.h>

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

template<class T = int>
class MeldableHeap {
  private:
    static uint32_t rng() {
        static mt19937 static_rng(random_device{}());
        return static_rng();
    }

    struct Node {
        T key;
        Node *left, *right;

        Node(T _key) : key(_key), left(nullptr), right(nullptr) {}
    };

    Node* merge(Node* a, Node* b) {
        if(!a) {
            return b;
        }
        if(!b) {
            return a;
        }

        if(a->key > b->key) {
            swap(a, b);
        }

        Node* q = new Node(a->key);
        if(rng() & 1) {
            q->left = merge(a->left, b);
            q->right = a->right;
        } else {
            q->left = a->left;
            q->right = merge(a->right, b);
        }

        return q;
    }

    pair<Node*, Node*> pop(Node* a) {
        Node* head = new Node(a->key);
        Node* tail = merge(a->left, a->right);
        return {head, tail};
    }

  public:
    Node* root;

    MeldableHeap() : root(nullptr) {}
    MeldableHeap(Node* _root) : root(_root) {}

    MeldableHeap copy() const {
        MeldableHeap new_heap;
        new_heap.root = root;
        return new_heap;
    }

    MeldableHeap merge(const MeldableHeap<T>& other) {
        MeldableHeap new_heap;
        new_heap.root = merge(root, other.root);
        return new_heap;
    }

    friend MeldableHeap merge(
        const MeldableHeap<T>& a, const MeldableHeap<T>& b
    ) {
        return a.merge(b);
    }

    void push(T key) {
        Node* new_node = new Node(key);
        root = merge(root, new_node);
    }

    T pop() {
        assert(root);
        auto [head, tail] = pop(root);
        root = tail;
        return head->key;
    }

    T top() const { return root->key; }

    tuple<T, MeldableHeap<T>, MeldableHeap<T>> trio() const {
        return {
            root->key, MeldableHeap<T>{root->left}, MeldableHeap<T>{root->right}
        };
    }

    bool empty() const { return root == nullptr; }

    bool operator<(const MeldableHeap<T>& other) const {
        return top() < other.top();
    }
};

template<class T = int>
class EppsteinShortestPaths {
  private:
    const T inf = numeric_limits<T>::max() / 2;

    struct Edge {
        int u, v;
        T w;
        Edge(int _u = 0, int _v = 0, T _w = 0) : u(_u), v(_v), w(_w) {}
    };

    pair<vector<T>, vector<int>> build_dijkstra_tree(int t) {
        vector<T> dist(n, inf);

        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> pq;
        dist[t] = 0;
        pq.emplace(0, t);

        while(!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if(d != dist[u]) {
                continue;
            }

            for(auto [v, idx]: rev_adj[u]) {
                T nd = d + edges[idx].w;
                if(nd < dist[v]) {
                    dist[v] = nd;
                    pq.emplace(nd, v);
                }
            }
        }

        vector<int> tree(n, -1);
        for(int u = 0; u < n; u++) {
            for(auto [v, idx]: adj[u]) {
                if(dist[u] == dist[v] + edges[idx].w) {
                    tree[u] = idx;
                    break;
                }
            }
        }

        return {dist, tree};
    }

    vector<int> topsort(const vector<int>& tree) {
        vector<int> deg(n, 0);
        for(int u = 0; u < n; u++) {
            if(tree[u] != -1) {
                deg[edges[tree[u]].v]++;
            }
        }

        queue<int> q;
        for(int u = 0; u < n; u++) {
            if(deg[u] == 0) {
                q.push(u);
            }
        }

        vector<int> order;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            order.push_back(u);

            if(tree[u] != -1) {
                int v = edges[tree[u]].v;
                deg[v]--;
                if(deg[v] == 0) {
                    q.push(v);
                }
            }
        }

        return order;
    }

  public:
    int n;
    vector<vector<pair<int, int>>> adj;
    vector<vector<pair<int, int>>> rev_adj;
    vector<Edge> edges;

    void init(int _n) {
        n = _n;
        edges.clear();
        adj.assign(n, {});
        rev_adj.assign(n, {});
    }

    EppsteinShortestPaths(int n = 0) { init(n); }

    int add_edge(int u, int v, T w, bool directed = true) {
        int idx = edges.size();
        edges.emplace_back(u, v, w);
        adj[u].emplace_back(v, idx);
        rev_adj[v].emplace_back(u, idx);

        if(!directed) {
            edges.emplace_back(v, u, w);
            adj[v].emplace_back(u, idx + 1);
            rev_adj[u].emplace_back(v, idx + 1);
        }

        return idx;
    }

    vector<T> get_k_shortest_paths(int s, int t, int k) {
        auto dist_and_tree = build_dijkstra_tree(t);
        auto dist = dist_and_tree.first;
        auto tree = dist_and_tree.second;

        if(dist[s] == inf || k <= 0) {
            return vector<T>();
        }

        vector<MeldableHeap<pair<T, int>>> heaps(n);
        for(int u = 0; u < n; u++) {
            for(auto& [v, idx]: adj[u]) {
                if(tree[u] == idx) {
                    continue;
                }

                T cost = edges[idx].w + dist[v] - dist[u];
                heaps[u].push({cost, v});
            }
        }

        auto order = topsort(tree);
        reverse(order.begin(), order.end());
        for(int u: order) {
            if(tree[u] != -1) {
                int par = edges[tree[u]].v ^ edges[tree[u]].u ^ u;
                heaps[u] = heaps[u].merge(heaps[par]);
            }
        }

        vector<T> ans = {dist[s]};
        if(heaps[s].empty()) {
            return ans;
        }

        priority_queue<
            pair<T, MeldableHeap<pair<T, int>>>,
            vector<pair<T, MeldableHeap<pair<T, int>>>>, greater<>>
            pq;
        pq.emplace(dist[s] + heaps[s].top().first, heaps[s].copy());

        while(!pq.empty() && (int)ans.size() < k) {
            auto [d, meld_heap] = pq.top();
            pq.pop();
            ans.push_back(d);

            auto [head, left_heap, right_heap] = meld_heap.trio();
            if(!left_heap.empty()) {
                pq.emplace(d - head.first + left_heap.top().first, left_heap);
            }
            if(!right_heap.empty()) {
                pq.emplace(d - head.first + right_heap.top().first, right_heap);
            }

            int v = head.second;
            if(!heaps[v].empty()) {
                pq.emplace(d + heaps[v].top().first, heaps[v].copy());
            }
        }

        return ans;
    }
};

int n, m, k, s, t;
EppsteinShortestPaths<int64_t> ksp;

void read() {
    cin >> n >> m;
    cin >> k;
    cin >> s >> t;
    s--;
    t--;

    ksp.init(n);
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--;
        v--;
        ksp.add_edge(u, v, w);
    }
}

void solve() {
    auto ans = ksp.get_k_shortest_paths(s, t, k);
    for(int i = 0; i < k; i++) {
        if(i < (int)ans.size()) {
            cout << ans[i] << '\n';
        } else {
            cout << "NO\n";
        }
    }
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
