#include <bits/stdc++.h>
// #include <coding_library/graph/hungarian_algorithm.hpp>

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

// http://e-maxx.ru/bookz/files/ahuja_inverse_mst_faster.pdf
// Can be improved to O(n sqrt n log n) using the below paper:
// https://hochbaum.ieor.berkeley.edu/html/pub/Hoch-inverse-treeOR03.pdf
// -> It's essentially the Hungarian algorithm, but translated to
//    only having vertex weights.

template<class T>
class HungarianAlgorithm {
  private:
    const T INF = numeric_limits<T>::max() / 2;
    vector<int> way;

  public:
    int n, m;
    vector<vector<T>> cost;
    vector<int> assignment;
    vector<T> pot_left, pot_right;

    HungarianAlgorithm(const vector<vector<T>>& a) {
        n = a.size();
        m = a[0].size();
        assert(n <= m);

        cost.assign(n + 1, vector<T>(m + 1));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                cost[i][j] = a[i][j];
            }
        }

        pot_left.assign(n + 1, 0);
        pot_right.assign(m + 1, 0);
        assignment.assign(m + 1, n);
        way.assign(m + 1, m);

        for(int i = 0; i < n; i++) {
            assignment[m] = i;
            int j0 = m;
            vector<T> minv(m + 1, INF);
            vector<bool> used(m + 1, false);
            do {
                used[j0] = true;
                int i0 = assignment[j0], j1 = m;
                T delta = INF;
                for(int j = 0; j < m; j++) {
                    if(!used[j]) {
                        T cur = cost[i0][j] - pot_left[i0] - pot_right[j];
                        if(cur < minv[j]) {
                            minv[j] = cur;
                            way[j] = j0;
                        }
                        if(minv[j] < delta) {
                            delta = minv[j];
                            j1 = j;
                        }
                    }
                }
                for(int j = 0; j <= m; j++) {
                    if(used[j]) {
                        pot_left[assignment[j]] += delta;
                        pot_right[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while(assignment[j0] != n);

            do {
                int j1 = way[j0];
                assignment[j0] = assignment[j1];
                j0 = j1;
            } while(j0 != m);
        }
    }

    T get_cost() {
        T ans = 0;
        for(int j = 0; j < m; j++) {
            ans += cost[assignment[j]][j];
        }
        return ans;
    }
};

int n, m;
vector<vector<pair<int, int>>> tree_adj;
vector<tuple<int, int, int>> edges;

void read() {
    cin >> n >> m;
    edges.resize(m);
    for(auto& [u, v, w]: edges) {
        cin >> u >> v >> w;
        u--, v--;
    }
}

bool add_on_path(
    int u, int par, int tgt, int edge_tgt, vector<vector<int64_t>>& matrix
) {
    if(u == tgt) {
        return true;
    }

    for(auto [v, idx]: tree_adj[u]) {
        if(v != par && add_on_path(v, u, tgt, edge_tgt, matrix)) {
            matrix[idx][edge_tgt] = min<int64_t>(
                matrix[idx][edge_tgt],
                get<2>(edges[edge_tgt]) - get<2>(edges[idx])
            );
            return true;
        }
    }

    return false;
}

void solve() {
    vector<vector<int64_t>> matrix(n - 1, vector<int64_t>(m, 0));

    tree_adj.assign(n, {});
    for(int i = 0; i < n - 1; i++) {
        auto [u, v, _] = edges[i];
        tree_adj[u].push_back({v, i});
        tree_adj[v].push_back({u, i});
    }

    for(int i = n - 1; i < m; i++) {
        auto [u, v, w] = edges[i];
        add_on_path(u, u, v, i, matrix);
    }

    auto hungarian = HungarianAlgorithm<int64_t>(matrix);
    for(int i = 0; i < n - 1; i++) {
        cout << get<2>(edges[i]) + hungarian.pot_left[i] << '\n';
    }
    for(int i = n - 1; i < m; i++) {
        cout << get<2>(edges[i]) - hungarian.pot_right[i] << '\n';
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
