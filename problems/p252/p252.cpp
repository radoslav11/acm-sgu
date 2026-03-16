#include <bits/stdc++.h>
// #include <coding_library/graph/mincost_flow.hpp>

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

template<typename T>
class MinCostFlow {
  private:
    const static T inf = numeric_limits<T>::max();

    struct Edge {
        int to, rev;
        T capacity, cost, flow;
        Edge(int _to, int _rev, T _capacity, T _cost, T _flow)
            : to(_to),
              rev(_rev),
              capacity(_capacity),
              cost(_cost),
              flow(_flow) {}
    };

    int n;
    vector<vector<Edge>> g;
    vector<T> potential;
    vector<pair<int, int>> parent;
    vector<pair<T, T>> dist;

    void build_potential(int source) {
        fill(potential.begin(), potential.end(), inf);
        potential[source] = 0;

        while(true) {
            bool any = false;
            for(int v = 0; v < n; v++) {
                for(const auto& e: g[v]) {
                    if(e.capacity != 0 && potential[v] != inf &&
                       potential[v] + e.cost < potential[e.to]) {
                        potential[e.to] = potential[v] + e.cost;
                        any = true;
                    }
                }
            }
            if(!any) {
                break;
            }
        }
    }

    bool dijkstra(int source, int sink, T flow_limit, T flow) {
        fill(dist.begin(), dist.end(), make_pair(inf, flow_limit - flow));
        dist[source].first = 0;

        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> q;
        q.push({0, source});

        while(!q.empty()) {
            auto [cur_dist, v] = q.top();
            q.pop();
            if(cur_dist > dist[v].first) {
                continue;
            }

            for(const auto& e: g[v]) {
                if(potential[e.to] != inf &&
                   cur_dist + e.cost - potential[e.to] + potential[v] <
                       dist[e.to].first &&
                   e.flow < e.capacity) {
                    parent[e.to] = {v, e.rev};
                    dist[e.to] = {
                        cur_dist + e.cost - potential[e.to] + potential[v],
                        min(dist[v].second, e.capacity - e.flow)
                    };
                    q.push({dist[e.to].first, e.to});
                }
            }
        }
        return dist[sink].first != inf;
    }

  public:
    MinCostFlow(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        g.assign(n, {});
        potential.resize(n);
        parent.resize(n);
        dist.resize(n);
    }

    int size() const { return n; }

    T edge_flow(int from, int id) const { return g[from][id].flow; }

    int add_edge(int from, int to, T capacity, T cost) {
        int id = g[from].size();
        g[from].push_back(
            Edge(to, g[to].size() + (from == to), capacity, cost, 0)
        );
        g[to].push_back(Edge(from, id, 0, -cost, 0));
        return id;
    }

    pair<T, T> flow(int source, int sink, T flow_limit = inf) {
        for(int v = 0; v < n; v++) {
            for(auto& e: g[v]) {
                e.flow = 0;
            }
        }

        T cost = 0, flow = 0;
        build_potential(source);

        while(flow < flow_limit && dijkstra(source, sink, flow_limit, flow)) {
            T delta = dist[sink].second;
            flow += delta;

            for(int v = sink; v != source; v = parent[v].first) {
                auto& e = g[parent[v].first][g[v][parent[v].second].rev];
                cost += e.cost * delta;
                e.flow += delta;
                g[v][parent[v].second].flow -= delta;
            }

            for(int i = 0; i < n; i++) {
                if(dist[i].first != inf) {
                    potential[i] += dist[i].first;
                }
            }
        }
        return {cost, flow};
    }
};

int n, m;
vector<tuple<int, int, int>> edges;
MinCostFlow<int> mcf;

void read() {
    cin >> n >> m;
    edges.resize(m);
    for(auto& [a, b, c]: edges) {
        cin >> a >> b >> c;
    }
}

void solve() {
    // The problem statement is a bit confusing, but we are essentially looking
    // for the minimum cost of a schedule that goes through every vertex exactly
    // once. However, the roads can be a single vertex, so we also want to
    // prioritize having as few paths as possible. This is a somewhat standard
    // problem - we need to select at most one road going out of a city, and at
    // most one coming in. We can create a bipartite graph for this, with a copy
    // of each vertex in each side. The constraint of having as few paths as
    // possible implies that we want a maximal matching in this graph
    // (otherwise, we can reduce the number of roads). In particular, for each
    // path there is exactly 1 node with 0 out degree, meaning that the number
    // of paths is n-selected_edges, or we want to maximize selected edges
    // (which happens in the maximum matching). Out of these maximum matchings,
    // we want to have the lowest sum of weights, which we can do with mincost
    // maxflow.

    int source = 0, sink = 2 * n + 1;
    mcf.init(2 * n + 2);

    for(int i = 1; i <= n; i++) {
        mcf.add_edge(source, i, 1, 0);
        mcf.add_edge(n + i, sink, 1, 0);
    }

    vector<int> edge_ids(m);
    for(int i = 0; i < m; i++) {
        auto [a, b, c] = edges[i];
        edge_ids[i] = mcf.add_edge(a, n + b, 1, c);
    }

    auto [cost, matching] = mcf.flow(source, sink);
    int num_paths = n - matching;

    vector<int> nxt(n + 1, 0);
    vector<bool> has_prev(n + 1, false);
    for(int i = 0; i < m; i++) {
        if(mcf.edge_flow(get<0>(edges[i]), edge_ids[i]) > 0) {
            auto [a, b, c] = edges[i];
            nxt[a] = b;
            has_prev[b] = true;
        }
    }

    cout << num_paths << ' ' << cost << '\n';
    for(int i = 1; i <= n; i++) {
        if(has_prev[i]) {
            continue;
        }
        vector<int> path;
        for(int v = i; v != 0; v = nxt[v]) {
            path.push_back(v);
        }
        cout << path.size();
        for(int v: path) {
            cout << ' ' << v;
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
        solve();
    }

    return 0;
}
