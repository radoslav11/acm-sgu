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

const int64_t inf = 1e18;

int n, m, h, s, e;
vector<int> exits;

struct Edge {
    int to, t, r, p;
};

vector<vector<Edge>> adj;

void read() {
    cin >> n >> m >> h >> s >> e;
    adj.resize(n + 1);
    for(int i = 0; i < m; i++) {
        int a, b, t, r, p;
        cin >> a >> b >> t >> r >> p;
        adj[a].push_back({b, t, r, p});
        adj[b].push_back({a, t, r, p});
    }
    exits.resize(e);
    cin >> exits;
}

pair<bool, vector<int>> check(int64_t x) {
    vector<int64_t> dist(n + 1, inf);
    vector<int> parent(n + 1, -1);
    vector<bool> visited(n + 1, false);

    dist[s] = 0;

    for(int i = 0; i < n; i++) {
        int u = -1;
        for(int v = 1; v <= n; v++) {
            if(!visited[v] && (u == -1 || dist[v] < dist[u])) {
                u = v;
            }
        }

        if(u == -1 || dist[u] == inf) {
            break;
        }
        visited[u] = true;

        for(auto& [to, t, r, p]: adj[u]) {
            int64_t heat = r + (dist[u] + t) * (int64_t)p;
            if(heat <= x) {
                int64_t new_dist = dist[u] + t;
                if(new_dist < dist[to]) {
                    dist[to] = new_dist;
                    parent[to] = u;
                }
            }
        }
    }

    int best_exit = -1;
    for(int ex: exits) {
        if(dist[ex] < inf) {
            best_exit = ex;
            break;
        }
    }

    if(best_exit == -1) {
        return {false, {}};
    }

    vector<int> path;
    int cur = best_exit;
    while(cur != -1) {
        path.push_back(cur);
        cur = parent[cur];
    }
    reverse(path.begin(), path.end());

    return {true, path};
}

void solve() {
    // Let's binary search for the answer X. This means that we can now only
    // pass through a road (u, v) at time t, if r(u, v) + (t + time(u, v)) *
    // p(u, v) <= X. We want to figure out if there is a valid path from S to
    // any of the exists that satisfies this.
    //
    // The core observation is that after we have settled on X, we are
    // interested in the lowest time t we reach any vertex u. This can be done
    // with a Dijkstra, but by only allowing edges that don't violate the X
    // constraint. Note that the graph could be dense, so we opt for the
    // quadratic Dijkstra implementation.
    //
    // The time complexity is overall O(N^2 log MAX).

    int low = 0, high = h, mid, ret = -1;
    vector<int> best_path;

    while(low <= high) {
        mid = (low + high) / 2;
        auto [ok, path] = check(mid);
        if(ok) {
            ret = mid;
            best_path = path;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if(ret == -1) {
        cout << "NO\n";
    } else {
        cout << "YES\n";
        cout << ret << "\n";
        cout << best_path.size() << " " << best_path << "\n";
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
