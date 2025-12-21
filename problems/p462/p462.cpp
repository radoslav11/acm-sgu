#include <bits/stdc++.h>
// #include <coding_library/graph/dsu.hpp>

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

class DSU {
  public:
    int n;
    vector<int> par;
    vector<int> sz;

    DSU(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        par.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        for(int i = 0; i <= n; i++) {
            par[i] = i;
            sz[i] = 1;
        }
    }

    int root(int u) { return par[u] = ((u == par[u]) ? u : root(par[u])); }
    bool connected(int x, int y) { return root(x) == root(y); }

    int unite(int x, int y) {
        x = root(x), y = root(y);
        if(x == y) {
            return x;
        }
        if(sz[x] > sz[y]) {
            swap(x, y);
        }
        par[x] = y;
        sz[y] += sz[x];
        return y;
    }

    vector<vector<int>> components() {
        vector<vector<int>> comp(n + 1);
        for(int i = 0; i <= n; i++) {
            comp[root(i)].push_back(i);
        }
        return comp;
    }
};

int n;
vector<tuple<int, int, int, int>> wires;

void read() {
    vector<int> nodes;

    cin >> n;
    wires.resize(n);
    for(auto& [u, v, r, p]: wires) {
        cin >> u >> v >> r >> p;
        nodes.push_back(u);
        nodes.push_back(v);
    }

    sort(nodes.begin(), nodes.end());
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

    for(auto& [u, v, r, p]: wires) {
        u = lower_bound(nodes.begin(), nodes.end(), u) - nodes.begin();
        v = lower_bound(nodes.begin(), nodes.end(), v) - nodes.begin();
    }
}

void solve() {
    // We can notice that we always will maintain an MST based on the
    // reliability. This means that no matter what order we choose the MST will
    // be almost the same (we can get convinced by correctness of Kruskal) - the
    // only difference comes in how we order the edges of the same reliability.
    // This is also not as hard - we simply want to keep the most expensive ones
    // last (so that previous cheaper wires get deleted). In other words it's
    // enough to sort the wires by "r" first, and then break ties by "p". This
    // is the order we will do and we could simulate the process with some
    // advanced data structures like link-cut trees, but there is an easier way
    // to recover the final configuration and price - we can simply do Kruskal
    // from the back.

    DSU dsu(2 * n);

    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);

    sort(perm.begin(), perm.end(), [&](int i, int j) {
        auto& [u1, v1, r1, p1] = wires[i];
        auto& [u2, v2, r2, p2] = wires[j];
        if(r1 != r2) {
            return r1 < r2;
        }
        return p1 < p2;
    });

    int64_t total_cost = 0;
    for(int i = n - 1; i >= 0; i--) {
        int idx = perm[i];
        auto& [u, v, r, p] = wires[idx];
        if(!dsu.connected(u, v)) {
            dsu.unite(u, v);
            total_cost += p;
        }
    }

    cout << total_cost << "\n";
    for(int i = 0; i < n; i++) {
        if(i > 0) {
            cout << " ";
        }
        cout << perm[i] + 1;
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
