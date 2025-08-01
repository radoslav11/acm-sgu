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
    const static T INF = numeric_limits<T>::max();

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

    T flow(int s, int t, int max_add = INF) {
        assert(s != t);

        T ret = 0, to_add;
        while(bfs(s, t)) {
            while((to_add = dfs(s, t))) {
                ret += to_add;
                if(ret >= max_add) {
                    // Early exit if we reached the desired
                    // max flow.
                    return ret;
                }
            }
        }

        return ret;
    }
};

struct Edge {
    int u, v, w;
    Edge() : u(0), v(0), w(0) {}
    Edge(int _u, int _v, int _w) : u(_u), v(_v), w(_w) {}
};

int n, m;
vector<Edge> edges;
vector<vector<pair<int, int>>> adj;

void read() {
    cin >> n >> m;
    edges.resize(m);
    adj.assign(n, {});
    for(int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
        edges[i].u--;
        edges[i].v--;
        adj[edges[i].u].emplace_back(edges[i].v, i);
        adj[edges[i].v].emplace_back(edges[i].u, i);
    }
}

vector<vector<pair<int, int>>> get_shortest_path_dag(int src) {
    vector<int> dist(n, numeric_limits<int>::max());
    priority_queue<
        pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
        pq;

    dist[src] = 0;
    pq.push({0, src});

    while(!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if(d > dist[u]) {
            continue;
        }

        for(auto [v, idx]: adj[u]) {
            if(dist[v] > d + edges[idx].w) {
                dist[v] = d + edges[idx].w;
                pq.push({dist[v], v});
            }
        }
    }

    vector<vector<pair<int, int>>> ret(n);
    for(int i = 0; i < m; i++) {
        if(dist[edges[i].u] + edges[i].w == dist[edges[i].v]) {
            ret[edges[i].u].emplace_back(edges[i].v, i);
        }
        if(dist[edges[i].v] + edges[i].w == dist[edges[i].u]) {
            ret[edges[i].v].emplace_back(edges[i].u, i);
        }
    }

    return ret;
}

void solve() {
    vector<vector<pair<int, int>>> dag = get_shortest_path_dag(0);
    MaxFlow<int> mf(n + m);

    for(int i = 0; i < n; i++) {
        for(auto [v, idx]: dag[i]) {
            mf.add_edge(i, n + idx, 1);
            mf.add_edge(n + idx, v, 1, idx);
        }
    }

    if(mf.flow(0, n - 1, 2) < 2) {
        cout << "No solution\n";
        return;
    }

    vector<vector<int>> final_graph(n);

    for(int i = n; i < n + m; i++) {
        for(auto& e: mf.adj[i]) {
            if(e.idx != -1 && e.flow > 0) {
                auto& edge = edges[e.idx];
                int u = edge.u, v = edge.v;
                if(u == e.to) {
                    swap(u, v);
                }
                final_graph[u].push_back(v);
            }
        }
    }

    for(int cnt = 0; cnt < 2; cnt++) {
        vector<int> path = {0};
        int u = 0;

        while(u != n - 1) {
            assert(!final_graph[u].empty());
            int v = final_graph[u].back();
            final_graph[u].pop_back();
            path.push_back(v);
            u = v;
        }

        for(int u: path) {
            cout << u + 1 << ' ';
        }
        cout << '\n';
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
