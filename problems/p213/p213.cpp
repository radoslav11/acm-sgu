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

int n, m, s, t;
vector<vector<pair<int, int>>> adj;

void read() {
    cin >> n >> m >> s >> t;
    s--, t--;
    adj.assign(n, {});
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
}

void solve() {
    vector<int> dist(n, -1);

    queue<int> q;
    q.push(s);
    dist[s] = 0;

    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(auto [v, _]: adj[u]) {
            if(dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    vector<vector<int>> answer(dist[t]);
    for(int u = 0; u < n; u++) {
        assert(dist[u] != -1);
        if(dist[u] >= dist[t]) {
            continue;
        }

        for(auto [v, id]: adj[u]) {
            if(dist[v] == dist[u] + 1) {
                answer[dist[u]].push_back(id + 1);
            }
        }
    }

    cout << dist[t] << '\n';
    for(int i = 0; i < dist[t]; i++) {
        cout << answer[i].size() << ' ' << answer[i] << '\n';
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
