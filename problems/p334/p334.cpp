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

int n, m;
vector<string> tbl;

void read() {
    cin >> n >> m;
    tbl.resize(n);
    cin >> tbl;
}

void solve() {
    // The main observation to make is that there are exactly 9 cells that are
    // an X, and 9! = 362880 is actually fairly small. This means we can try all
    // possible mappings to the 3x3 grid. Afterwards, we simply want to figure
    // out what's the least number of letters, such that the compositions would
    // be connected in both the original table and have the same "shapes". For a
    // given mapping, each cell has a delta (the shift from its original
    // position to its 3x3 position). Cells in the same part must share the same
    // delta (since parts are only shifted, not rotated). So we group cells by
    // delta, and within each group it's enough to count the connected
    // components (say using a DSU). The total number of components across
    // all groups is the number of parts for that permutation. We take the
    // minimum over all 9! permutations, where the total number of operations
    // will just be a few million and would fit in the constraints very
    // comfortably.

    vector<pair<int, int>> cells;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(tbl[i][j] == 'X') {
                cells.push_back({i, j});
            }
        }
    }

    vector<vector<int>> adj(9);
    for(int i = 0; i < 9; i++) {
        for(int j = i + 1; j < 9; j++) {
            if(abs(cells[i].first - cells[j].first) +
                   abs(cells[i].second - cells[j].second) ==
               1) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }

    auto build_dsu = [&](const vector<int>& p) {
        DSU dsu(9);
        vector<int> dr(9), dc(9);
        for(int i = 0; i < 9; i++) {
            dr[i] = p[i] / 3 - cells[i].first;
            dc[i] = p[i] % 3 - cells[i].second;
        }
        for(int i = 0; i < 9; i++) {
            for(int j: adj[i]) {
                if(j > i && dr[i] == dr[j] && dc[i] == dc[j]) {
                    dsu.unite(i, j);
                }
            }
        }
        return dsu;
    };

    vector<int> perm(9);
    iota(perm.begin(), perm.end(), 0);

    int best_parts = 10;
    vector<int> best_perm;

    do {
        auto dsu = build_dsu(perm);

        int cnt = 0;
        for(int i = 0; i < 9; i++) {
            if(dsu.root(i) == i) {
                cnt++;
            }
        }

        if(cnt < best_parts) {
            best_parts = cnt;
            best_perm = perm;
            if(best_parts == 1) {
                break;
            }
        }
    } while(next_permutation(perm.begin(), perm.end()));

    auto dsu = build_dsu(best_perm);

    vector<int> label(9, -1);
    int cur = 0;
    for(int i = 0; i < 9; i++) {
        int r = dsu.root(i);
        if(label[r] == -1) {
            label[r] = cur++;
        }
    }

    cout << best_parts << "\n";
    auto grid = tbl;
    for(int i = 0; i < 9; i++) {
        grid[cells[i].first][cells[i].second] = 'A' + label[dsu.root(i)];
    }
    for(auto& row: grid) {
        cout << row << "\n";
    }
    cout << "\n";

    vector<string> small(3, string(3, '.'));
    for(int i = 0; i < 9; i++) {
        small[best_perm[i] / 3][best_perm[i] % 3] = 'A' + label[dsu.root(i)];
    }
    for(auto& row: small) {
        cout << row << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
