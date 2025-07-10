#include <bits/stdc++.h>
// #include <coding_library/graph/maxflow.hpp>

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
class MaxFlow {
  private:
    const static T INF = numeric_limits<T>::max();

    struct Edge {
        T flow, cap;
        int idx, rev, to;
        Edge(int _to, int _rev, T _flow, T _cap, int _idx)
            : to(_to), rev(_rev), flow(_flow), cap(_cap), idx(_idx) {}
    };

    vector<int> dist, po;
    int n;

    bool bfs(int s, int t) {
        fill(dist.begin(), dist.end(), -1);
        fill(po.begin(), po.end(), 0);

        queue<int> q;
        q.push(s);
        dist[s] = 0;

        while(!q.empty()) {
            int u = q.front();
            q.pop();

            for(Edge e: adj[u]) {
                if(dist[e.to] == -1 && e.flow < e.cap) {
                    dist[e.to] = dist[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return dist[t] != -1;
    }

    T dfs(int u, int t, T fl = INF) {
        if(u == t) {
            return fl;
        }

        for(; po[u] < (int)adj[u].size(); po[u]++) {
            auto& e = adj[u][po[u]];
            if(dist[e.to] == dist[u] + 1 && e.flow < e.cap) {
                T f = dfs(e.to, t, min(fl, e.cap - e.flow));
                e.flow += f;
                adj[e.to][e.rev].flow -= f;
                if(f > 0) {
                    return f;
                }
            }
        }

        return 0;
    }

  public:
    MaxFlow(int n = 0) { init(n); }

    vector<vector<Edge>> adj;

    void init(int _n) {
        n = _n;
        adj.assign(n + 1, {});
        dist.resize(n + 1);
        po.resize(n + 1);
    }

    void add_edge(int u, int v, T w, int idx = -1) {
        adj[u].push_back(Edge(v, adj[v].size(), 0, w, idx));
        adj[v].push_back(Edge(u, adj[u].size() - 1, 0, 0, -1));
    }

    T flow(int s, int t) {
        assert(s != t);

        T ret = 0, to_add;
        while(bfs(s, t)) {
            while((to_add = dfs(s, t))) {
                ret += to_add;
            }
        }

        return ret;
    }
};

int n, m;
vector<tuple<int, int, int, int>> edges;

void read() {
    cin >> n >> m;
    edges.resize(m);
    for(int i = 0; i < m; i++) {
        int u, v, cap, flag;
        cin >> u >> v >> cap >> flag;
        u--, v--;
        edges[i] = {u, v, cap, flag ? cap : 0};
    }
}

pair<vector<int>, MaxFlow<int>> build_circulation(int x) {
    MaxFlow<int> mf(n + 2);

    vector<int> deg(n);
    for(int i = 0; i < m; i++) {
        auto [u, v, cap, lb] = edges[i];
        deg[v] += lb;
        deg[u] -= lb;
        mf.add_edge(u, v, cap - lb, i);
    }

    for(int i = 0; i < n; i++) {
        if(deg[i] > 0) {
            mf.add_edge(n, i, deg[i]);
        } else if(deg[i] < 0) {
            mf.add_edge(i, n + 1, -deg[i]);
        }
    }

    mf.add_edge(n - 1, 0, x);
    return {deg, mf};
}

bool check_circulation(int x) {
    auto [deg, mf] = build_circulation(x);
    int need_flow = 0;
    for(int i = 0; i < n; i++) {
        if(deg[i] > 0) {
            need_flow += deg[i];
        }
    }

    return mf.flow(n, n + 1) == need_flow;
}

void solve() {
    int low = 0, high = (int)1e8, mid, ans = -1;
    while(low <= high) {
        mid = (low + high) / 2;
        if(check_circulation(mid)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if(ans == -1) {
        cout << "Impossible\n";
    } else {
        MaxFlow<int> mf = build_circulation(ans).second;
        mf.flow(n, n + 1);

        cout << ans << '\n';
        vector<int> flow(m);
        for(int u = 0; u < n; u++) {
            for(auto e: mf.adj[u]) {
                if(e.idx != -1) {
                    flow[e.idx] = e.flow + get<3>(edges[e.idx]);
                }
            }
        }

        cout << flow << '\n';
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
