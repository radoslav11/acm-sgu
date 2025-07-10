#include <bits/stdc++.h>
#include <coding_library/graph/hungarian_algorithm.hpp>

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

template<class T>
class HungarianAlgorithm {
  private:
    const T INF = numeric_limits<T>::max() / 2;
    vector<vector<T>> cost;

  public:
    vector<int> assignment;

    HungarianAlgorithm(const vector<vector<T>>& a) {
        int n = a.size(), m = a[0].size();
        cost.assign(n + 1, vector<T>(m + 1));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                cost[i][j] = a[i][j];
            }
        }

        vector<T> u(n + 1), v(m + 1);
        vector<int> p(m + 1, n), way(m + 1, n);
        for(int i = 0; i < n; i++) {
            p[m] = i;
            int j0 = m;
            vector<T> minv(m + 1, INF);
            vector<bool> used(m + 1, false);
            do {
                used[j0] = true;
                int i0 = p[j0], j1;
                T delta = INF;
                for(int j = 0; j < m; j++) {
                    if(!used[j]) {
                        T cur = cost[i0][j] - u[i0] - v[j];
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
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while(p[j0] != n);

            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while(j0 != m);
        }

        assignment = vector<int>(begin(p), end(p) - 1);
    }

    T get_cost() {
        T ans = 0;
        for(int i = 0; i < (int)assignment.size(); i++) {
            ans += cost[assignment[i]][i];
        }
        return ans;
    }
};

int n;
vector<int> a;
vector<vector<int>> adj;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
    adj.assign(n, {});
    for(int i = 0; i < n; i++) {
        int cnt;
        cin >> cnt;
        while(cnt--) {
            int x;
            cin >> x;
            adj[i].push_back(x - 1);
        }
    }
}

void solve() {
    vector<vector<int64_t>> cost(n, vector<int64_t>(n, 0));
    for(int i = 0; i < n; i++) {
        for(int j: adj[i]) {
            cost[j][i] = -a[i] * a[i];
        }
    }

    // for(int i = 0; i < n; i++) {
    //     for(int j = 0; j < n; j++) {
    //         cout << cost[i][j] << ' ';
    //     }
    //     cout << '\n';
    // }

    HungarianAlgorithm<int64_t> ha(cost);
    for(int i = 0; i < n; i++) {
        if(find(adj[i].begin(), adj[i].end(), ha.assignment[i]) ==
           adj[i].end()) {
            cout << 0 << ' ';
        } else {
            cout << ha.assignment[i] + 1 << ' ';
        }
    }
    cout << '\n';
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
