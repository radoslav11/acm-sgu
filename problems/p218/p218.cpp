#include <bits/stdc++.h>
// #include <coding_library/graph/maxflow.hpp>

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

class HopcroftKarp {
  private:
    int n, m;
    vector<int> dist;

    bool bfs() {
        queue<int> q;
        dist.assign(n, -1);
        for(int u = 0; u < n; ++u) {
            if(inv_match[u] == -1) {
                dist[u] = 0;
                q.push(u);
            }
        }

        bool found = false;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v: adj[u]) {
                int m = match[v];
                if(m == -1) {
                    found = true;
                } else if(dist[m] == -1) {
                    dist[m] = dist[u] + 1;
                    q.push(m);
                }
            }
        }

        return found;
    }

    bool dfs(int u) {
        for(int v: adj[u]) {
            int m = match[v];
            if(m == -1 || (dist[m] == dist[u] + 1 && dfs(m))) {
                inv_match[u] = v;
                match[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

  public:
    vector<int> match, inv_match;
    vector<vector<int>> adj;

    HopcroftKarp(int _n, int _m = -1) : n(_n), m(_m == -1 ? _n : _m) {
        adj.assign(n, vector<int>());
        clear(false);
    }

    void clear(bool clear_adj = true) {
        match.assign(m, -1);
        inv_match.assign(n, -1);
        if(clear_adj) {
            adj.assign(n, vector<int>());
        }
    }

    void add_edge(int u, int v) { adj[u].push_back(v); }

    int max_matching(bool shuffle_edges = false) {
        if(shuffle_edges) {
            for(int i = 0; i < n; i++) {
                shuffle(
                    adj[i].begin(), adj[i].end(),
                    mt19937(
                        chrono::steady_clock::now().time_since_epoch().count()
                    )
                );
            }
        }

        int ans = 0;
        while(bfs()) {
            for(int u = 0; u < n; ++u) {
                if(inv_match[u] == -1 && dfs(u)) {
                    ans++;
                }
            }
        }
        return ans;
    }

    vector<pair<int, int>> get_matching() {
        vector<pair<int, int>> matches;
        for(int u = 0; u < n; ++u) {
            if(inv_match[u] != -1) {
                matches.emplace_back(u, inv_match[u]);
            }
        }
        return matches;
    }

    pair<vector<int>, vector<int>> minimum_vertex_cover() {
        vector<int> left_cover, right_cover;
        bfs();

        for(int u = 0; u < n; ++u) {
            if(dist[u] == -1) {
                left_cover.push_back(u);
            }
        }

        for(int v = 0; v < m; ++v) {
            if(match[v] != -1 && dist[match[v]] != -1) {
                right_cover.push_back(v);
            }
        }

        return {left_cover, right_cover};
    }
};

using BipartiteMatching = HopcroftKarp;

int n;
vector<vector<int>> a;

void read() {
    cin >> n;
    a.assign(n, vector<int>(n));
    cin >> a;
}

BipartiteMatching eval(int mid, const vector<array<int, 3>> &edges) {
    vector<array<int, 3>> edges2;
    for(auto [w, u, v]: edges) {
        if(w <= edges[mid][0]) {
            edges2.push_back({w, u, v});
        }
    }

    int m = edges2.size();
    BipartiteMatching bm(n);
    for(auto [w, u, v]: edges2) {
        bm.add_edge(u, v);
    }

    return bm;
}

void solve() {
    vector<array<int, 3>> edges;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            edges.push_back({a[i][j], i, j});
        }
    }
    sort(edges.begin(), edges.end());

    int low = 0, high = n * n - 1, ans = -1;
    while(low <= high) {
        int mid = (low + high) / 2;
        if(eval(mid, edges).max_matching(true) == n) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    BipartiteMatching bm = eval(ans, edges);
    bm.max_matching();
    cout << edges[ans][0] << '\n';
    auto matching = bm.get_matching();
    for(auto [u, v]: matching) {
        cout << u + 1 << ' ' << v + 1 << '\n';
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
