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

int n, m, k;
vector<tuple<int, int, int>> edges;
vector<vector<pair<int, int>>> adj;
vector<int> important;

void read() {
    cin >> n >> m;
    edges.resize(m);
    adj.assign(n, {});
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
        edges[i] = {u, v, w};
    }

    cin >> k;
    important.resize(k);
    cin >> important;
    for(int &v: important) {
        v--;
    }
}

vector<int> dijkstra(int src) {
    vector<int> dist(n, 1e9);
    dist[src] = 0;
    priority_queue<
        pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
        pq;
    pq.push({0, src});
    while(!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if(dist[u] < d) {
            continue;
        }
        for(auto [v, i]: adj[u]) {
            auto [_, __, w] = edges[i];
            if(dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

vector<vector<pair<int, int>>> build_shortest_path_dag(const vector<int>& dist
) {
    vector<vector<pair<int, int>>> dag(n);
    for(int u = 0; u < n; u++) {
        for(auto [v, i]: adj[u]) {
            auto [_, __, w] = edges[i];
            if(dist[v] == dist[u] + w) {
                dag[u].push_back({v, i});
            }
        }
    }
    return dag;
}

int get_furthest(const vector<int>& dist) {
    int endpoint = important[0];
    for(int v: important) {
        if(dist[v] > dist[endpoint]) {
            endpoint = v;
        }
    }
    return endpoint;
}

void solve() {
    vector<int> dist = dijkstra(important[0]);
    int endpoint = get_furthest(dist);

    dist = dijkstra(endpoint);
    vector<int> dp(n, -1);
    vector<bool> visited(n, false);
    for(int v: important) {
        visited[v] = true;
    }

    vector<vector<pair<int, int>>> dag = build_shortest_path_dag(dist);
    function<int(int)> dfs = [&](int u) {
        if(dp[u] != -1) {
            return dp[u];
        }
        dp[u] = visited[u];
        for(auto [v, i]: dag[u]) {
            dp[u] = max(dp[u], dfs(v) + visited[u]);
        }
        return dp[u];
    };

    int other_endpoint = get_furthest(dist);
    int start = endpoint;
    
    vector<int> ans;
    while(start != other_endpoint) {
        for(auto [v, i]: dag[start]) {
            if(dfs(start) == dfs(v) + visited[start]) {
                ans.push_back(i + 1);
                start = v;
                break;
            }
        }
    }

    cout << ans.size() << '\n';
    cout << ans << '\n';
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
