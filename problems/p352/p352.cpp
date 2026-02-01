#include <bits/stdc++.h>
// #include <coding_library/tree/lca_sparse_table.hpp>
// #include <coding_library/data_structures/sparse_table.hpp>

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

template<class T, T (*merge)(T, T)>
class SparseTable {
  private:
    int n;
    vector<vector<T>> dp;
    vector<int> prec_lg2;

  public:
    SparseTable() {
        n = 0;
        dp.clear();
        prec_lg2.clear();
    }

    void init(const vector<T>& a) {
        n = a.size();
        prec_lg2.resize(n + 1);
        for(int i = 2; i <= n; i++) {
            prec_lg2[i] = prec_lg2[i >> 1] + 1;
        }

        dp.assign(prec_lg2[n] + 1, vector<T>(n));
        dp[0] = a;

        for(int j = 1; (1 << j) <= n; j++) {
            for(int i = 0; i + (1 << j) <= n; i++) {
                dp[j][i] = merge(dp[j - 1][i], dp[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    T query(int l, int r) {
        int k = prec_lg2[r - l + 1];
        return merge(dp[k][l], dp[k][r - (1 << k) + 1]);
    }
};

class LCAUtilsRMQ {
  private:
    static pair<int, int> _min_custom(pair<int, int> a, pair<int, int> b) {
        return min(a, b);
    }

    SparseTable<pair<int, int>, _min_custom> rmq;
    vector<int> pos, dep;
    vector<pair<int, int>> order;

    void pre_dfs(int u, int pr = -1, int d = 0) {
        pos[u] = order.size();
        dep[u] = d;
        order.push_back({d, u});

        for(int v: adj[u]) {
            if(v != pr) {
                pre_dfs(v, u, d + 1);
                order.push_back({d, u});
            }
        }
    }

  public:
    int n;
    vector<vector<int>> adj;

    LCAUtilsRMQ() { n = 0; }
    LCAUtilsRMQ(int _n) { init(_n); }
    LCAUtilsRMQ(int _n, const vector<vector<int>>& _adj) {
        init(_n);
        adj = _adj;
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void init(int _n) {
        n = _n;
        order.clear();
        adj.assign(n, {});
    }

    void prepare(int root = 0) {
        order.clear();
        pos.resize(n);
        dep.resize(n);
        pre_dfs(root);
        rmq.init(order);
    }

    int lca(int u, int v) {
        if(pos[u] > pos[v]) {
            swap(u, v);
        }
        return rmq.query(pos[u], pos[v]).second;
    }

    int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }
};

using LcaUtils = LCAUtilsRMQ;

int n, m;
LcaUtils lca;
vector<vector<pair<int, int>>> tree, nontree;
vector<int> parent;
vector<int64_t> dist;
vector<vector<int64_t>> addv, remv;
vector<int64_t> ans;

void dfs0(int u, int p) {
    for(auto [v, w]: tree[u]) {
        if(v == p) {
            continue;
        }
        parent[v] = u;
        dist[v] = dist[u] + w;
        dfs0(v, u);
    }
}

multiset<int64_t>* dfs(int u) {
    multiset<int64_t>* cur = new multiset<int64_t>();

    for(auto [v, w]: tree[u]) {
        if(v == parent[u]) {
            continue;
        }
        multiset<int64_t>* child = dfs(v);
        if(child->size() > cur->size()) {
            swap(cur, child);
        }
        cur->insert(child->begin(), child->end());
        delete child;
    }

    for(int64_t x: addv[u]) {
        cur->insert(x);
    }

    for(int64_t x: remv[u]) {
        auto it = cur->find(x);
        if(it != cur->end()) {
            cur->erase(it);
        }
    }

    if(u != 0) {
        if(cur->empty()) {
            ans[u] = -1;
        } else {
            ans[u] = *cur->begin() - dist[u];
        }
    }

    return cur;
}

void read() {
    cin >> n >> m;
    tree.assign(n, {});
    nontree.assign(n, {});
    addv.assign(n, {});
    remv.assign(n, {});
    dist.assign(n, 0);
    parent.assign(n, -1);
    ans.assign(n, -1);
    lca.init(n);

    for(int i = 0; i < m; i++) {
        int a, b, w, t;
        cin >> a >> b >> w >> t;
        a--;
        b--;
        if(t) {
            tree[a].push_back({b, w});
            tree[b].push_back({a, w});
            lca.add_edge(a, b);
        } else {
            nontree[a].push_back({b, w});
            nontree[b].push_back({a, w});
        }
    }
}

void solve() {
    // The core observation in this problem is that due to the fact that this is
    // a Dijkstra tree, after we burn some edge, we need to use at most one
    // non-special edge. This can be shown with a simple contradiction (assume
    // we use 2 non-special edges, consider the latest one in the path, and
    // then pick the simpler path going along the original tree edges which is
    // at least as quick). This means that if we burn the edge (par[u], u), we
    // want to choose some non-special edge (x, y, w) that minimizes dist(u, x)
    // + dist(1, y) + w, with x in the subtree of u and y outside of the tree.
    //
    // The naive way to do this is in O(N*M) by passing through all edges, but
    // this will be too slow. Instead we can do something smarter with small to
    // large trick. Let's have events - for edge (x, y, w), we will add this
    // event at x, add it at y, and remove it twice at lca(x, y). Each event is
    // represented by a single value equal to w + dist[x] + dist[y] where
    // dist[.] is the distance from the root. Then when we are finding the
    // answer for some u, we want to get min(events_subtree) - dist[u]. This can
    // easily be done with small to large, and the complexity will be somewhat
    // lower at O((N+M) log^2 (N+M)).

    dfs0(0, -1);
    lca.prepare(0);

    for(int u = 0; u < n; u++) {
        for(auto [v, w]: nontree[u]) {
            if(u < v) {
                int64_t val = dist[u] + dist[v] + w;
                int c = lca.lca(u, v);
                addv[u].push_back(val);
                addv[v].push_back(val);
                remv[c].push_back(val);
                remv[c].push_back(val);
            }
        }
    }

    dfs(0);

    for(int i = 1; i < n; i++) {
        cout << ans[i];
        if(i < n - 1) {
            cout << " ";
        }
    }
    cout << "\n";
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
