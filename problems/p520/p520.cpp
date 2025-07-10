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

int n, m;
vector<vector<int>> adj;

void read() {
    cin >> n >> m;
    adj.assign(n, vector<int>());
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

pair<vector<int>, vector<vector<int>>> create_bfs_dag() {
    vector<int> dist(n, -1);
    vector<vector<int>> bfs(n);
    queue<int> q;
    q.push(0);
    dist[0] = 0;
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(int v: adj[u]) {
            if(dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    for(int u = 0; u < n; u++) {
        for(int v: adj[u]) {
            if(dist[v] == dist[u] + 1) {
                bfs[u].push_back(v);
            }
        }
    }

    return {dist, bfs};
}

int rec(int u, vector<int>& dp, vector<int>& dist, vector<vector<int>>& bfs) {
    if(dp[u] != -1) {
        return dp[u];
    }

    dp[u] = 0;
    for(int v: bfs[u]) {
        if(rec(v, dp, dist, bfs) == 0) {
            dp[u] = 1;
            break;
        }
    }

    return dp[u];
}

void solve() {
    auto [dist, bfs] = create_bfs_dag();
    vector<int> dp(n, -1);
    if(rec(0, dp, dist, bfs) == 1) {
        cout << "Vladimir" << '\n';
    } else {
        cout << "Nikolay" << '\n';
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
