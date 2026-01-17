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

int m, n;
vector<int> a, b;

void read() {
    cin >> m >> n;
    a.resize(n);
    b.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> a[i] >> b[i];
    }
}

vector<vector<int>> combos;
vector<int> combo_index;

vector<vector<vector<int>>> available;
vector<vector<vector<pair<int, int>>>> adj;

int encode_state(const vector<int>& rest, const vector<int>& positions) {
    int v = 0;
    for(int i = 0; i < (int)positions.size(); i++) {
        v |= (rest[positions[i]] - 1) << (2 * i);
    }
    return v;
}

void decode_state(int v, const vector<int>& positions, vector<int>& rest) {
    fill(rest.begin(), rest.end(), 0);
    for(int i = 0; i < (int)positions.size(); i++) {
        rest[positions[i]] = ((v >> (2 * i)) & 3) + 1;
    }
}

void build_graph() {
    combo_index.assign(1 << n, -1);
    for(int mask = 0; mask < (1 << n); mask++) {
        if(__builtin_popcount(mask) <= 4) {
            combo_index[mask] = combos.size();
            vector<int> positions;
            for(int i = 0; i < n; i++) {
                if(mask & (1 << i)) {
                    positions.push_back(i);
                }
            }
            combos.push_back(positions);
        }
    }

    int num_combos = combos.size();
    available.assign(num_combos, vector<vector<int>>(256));
    adj.assign(
        num_combos,
        vector<vector<pair<int, int>>>(256, vector<pair<int, int>>(n + 1))
    );

    for(int ci = 0; ci < num_combos; ci++) {
        int sz = combos[ci].size();
        int max_v = 1 << (2 * sz);

        for(int v = 0; v < max_v; v++) {
            vector<int> rest(n, 0);
            decode_state(v, combos[ci], rest);

            for(int i = 0; i < n; i++) {
                if(rest[i] == 0 && a[i] > 0) {
                    available[ci][v].push_back(i);
                }
            }

            for(int used = -1; used < n; used++) {
                vector<int> new_rest(n);
                for(int i = 0; i < n; i++) {
                    if(i == used) {
                        new_rest[i] = b[i];
                    } else {
                        new_rest[i] = max(0, rest[i] - 1);
                    }
                }

                int new_mask = 0;
                for(int i = 0; i < n; i++) {
                    if(new_rest[i] > 0) {
                        new_mask |= (1 << i);
                    }
                }
                int new_ci = combo_index[new_mask];
                int new_v = encode_state(new_rest, combos[new_ci]);

                adj[ci][v][used + 1] = {new_ci, new_v};
            }
        }
    }
}

void solve() {
    // The solution is fairly simple - we can simply do a dynamic programming
    // dp[balloons left][state of time left]. The second dimension represents
    // the time left for every volunteer until it can fill a balloon again. On
    // first glance, this would be too slow as we might have O(MAX_B^N) states,
    // but in reality we can have at most 4 people with non-zero time left.
    // We have C(10, 4) = 210 (although we need to account for smaller sets
    // too), and very naively there are 4^4 = 256 combinations for their wait
    // times, meaning we are around a few million states.
    //
    // We might have to be a bit careful with time limits, where a good way is
    // to notice that the transitions don't depend on "balloons left", so we can
    // precompute the graph based on the other two dimensions. Then we can do a
    // BFS on this graph.

    build_graph();

    int num_combos = combos.size();
    vector<vector<vector<int>>> dist(
        m + 1, vector<vector<int>>(num_combos, vector<int>(256, -1))
    );

    queue<tuple<int, int, int>> q;
    dist[m][0][0] = 0;
    q.push({m, 0, 0});

    while(!q.empty()) {
        auto [bal, ci, v] = q.front();
        q.pop();

        if(bal == 0) {
            cout << dist[bal][ci][v] << '\n';
            return;
        }

        int d = dist[bal][ci][v];
        const auto& avail = available[ci][v];

        if(avail.empty()) {
            auto [new_ci, new_v] = adj[ci][v][0];
            if(dist[bal][new_ci][new_v] == -1) {
                dist[bal][new_ci][new_v] = d + 1;
                q.push({bal, new_ci, new_v});
            }
        } else {
            for(int i: avail) {
                auto [new_ci, new_v] = adj[ci][v][i + 1];
                int new_bal = max(0, bal - a[i]);
                if(dist[new_bal][new_ci][new_v] == -1) {
                    dist[new_bal][new_ci][new_v] = d + 1;
                    q.push({new_bal, new_ci, new_v});
                }
            }
        }
    }

    cout << -1 << '\n';
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
