#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

const int inf = (int)1e9;

int n, m;
vector<vector<pair<int, int>>> adj;

void read() {
    cin >> n >> m;
    adj.assign(n, {});
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--, w--;
        adj[u].push_back({v, w});
    }
}

void solve() {
    vector<vector<int>> dist(n, vector<int>(3, inf));
    dist[0] = {0, 0, 0};

    queue<pair<int, int>> q;
    q.push({0, 0});
    q.push({0, 1});
    q.push({0, 2});

    while(!q.empty()) {
        auto [u, w] = q.front();
        q.pop();
        for(auto [v, w2]: adj[u]) {
            if(w != w2 && dist[v][w2] > dist[u][w] + 1) {
                dist[v][w2] = dist[u][w] + 1;
                q.push({v, w2});
            }
        }
    }

    int ans = min({dist[n - 1][0], dist[n - 1][1], dist[n - 1][2]});
    if(ans == inf) {
        cout << -1 << '\n';
    } else {
        cout << ans << '\n';
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
