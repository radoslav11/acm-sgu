#include <bits/stdc++.h>
// #include <coding_library/graph/two_sat.hpp>

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

class TwoSat {
  private:
    vector<bool> visited;

    void dfs1(int u) {
        visited[u] = true;
        for(int v: adj[u]) {
            if(!visited[v]) {
                dfs1(v);
            }
        }

        top_sort.push_back(u);
    }

    void dfs2(int u) {
        for(int v: radj[u]) {
            if(comp[v] == -1) {
                comp[v] = comp[u];
                dfs2(v);
            }
        }
    }

  public:
    int n;
    vector<vector<int>> adj, radj;
    vector<int> comp, comp_ids, top_sort;

    TwoSat() {}
    TwoSat(int _n) { init(_n); }

    void init(int _n) {
        n = _n;
        comp_ids.clear();
        top_sort.clear();
        adj.assign(2 * n, {});
        radj.assign(2 * n, {});
    }

    void add_implication(int u, int v, bool neg_u = false, bool neg_v = false) {
        adj[u << 1 | neg_u].push_back(v << 1 | neg_v);
        radj[v << 1 | neg_v].push_back(u << 1 | neg_u);
    }

    pair<bool, vector<bool>> solve() {
        comp.assign(2 * n, -1);
        visited.assign(2 * n, false);

        for(int i = 0; i < 2 * n; i++) {
            if(!visited[i]) {
                dfs1(i);
            }
        }

        reverse(top_sort.begin(), top_sort.end());
        for(int u: top_sort) {
            if(comp[u] == -1) {
                comp[u] = (int)comp_ids.size();
                comp_ids.push_back(comp[u]);
                dfs2(u);
            }
        }

        vector<bool> assignment(n);
        for(int i = 0; i < n; i++) {
            if(comp[i << 1] == comp[i << 1 | 1]) {
                return {false, {}};
            }

            assignment[i] = comp[i << 1] > comp[i << 1 | 1];
        }

        return {true, assignment};
    }
};

int n, m;
vector<vector<int>> tbl;

void read() {
    cin >> n >> m;
    tbl.assign(n - 1, vector<int>(m - 1));
    for(int i = 0; i < n - 1; i++) {
        string s;
        cin >> s;
        for(int j = 0; j < m - 1; j++) {
            tbl[i][j] = s[j] - '0';
        }
    }
}

void solve() {
    for(int value00 = 0; value00 < 2; value00++) {
        vector<vector<int>> delta(n, vector<int>(m, 0));
        delta[0][0] = value00;

        TwoSat ts(n + m - 2);
        for(int i = 1; i < n; i++) {
            for(int j = 1; j < m; j++) {
                delta[i][j] = tbl[i - 1][j - 1] - delta[i - 1][j] -
                              delta[i][j - 1] - delta[i - 1][j - 1];

                for(int value_row = 0; value_row < 2; value_row++) {
                    for(int value_col = 0; value_col < 2; value_col++) {
                        int real_delta = delta[i][j];
                        if(j % 2 == 0) {
                            real_delta += value_row;
                        } else {
                            real_delta -= value_row;
                        }
                        if(i % 2 == 0) {
                            real_delta += value_col;
                        } else {
                            real_delta -= value_col;
                        }

                        if(real_delta == 0 || real_delta == 1) {
                            continue;
                        }

                        int u = i - 1;
                        int v = j + n - 2;
                        // (u == value_row) -> not (v == value_col)
                        ts.add_implication(
                            u, v, value_row == 0, value_col == 1
                        );
                        // (v == value_col) -> not (u == value_row)
                        ts.add_implication(
                            v, u, value_col == 0, value_row == 1
                        );
                    }
                }
            }
        }

        auto [is_possible, assignment] = ts.solve();
        if(!is_possible) {
            continue;
        }

        vector<vector<int>> ans(n, vector<int>(m, 0));
        ans[0][0] = value00;
        for(int i = 1; i < n; i++) {
            ans[i][0] = assignment[i - 1];
        }
        for(int j = 1; j < m; j++) {
            ans[0][j] = assignment[n + j - 2];
        }
        for(int i = 1; i < n; i++) {
            for(int j = 1; j < m; j++) {
                ans[i][j] = tbl[i - 1][j - 1] - ans[i - 1][j] - ans[i][j - 1] -
                            ans[i - 1][j - 1];
                assert(ans[i][j] == 0 || ans[i][j] == 1);
            }
        }
        
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                cout << ans[i][j];
            }
            cout << '\n';
        }

        return;
    }

    cout << "CORRUPT\n";
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
