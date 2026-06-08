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

struct edge {
    int to, cap, flow;
};

int n, m;
vector<vector<int>> adj;
vector<pair<int, int>> tunnels;
vector<int> a_set, b_set;

void read() {
    cin >> n >> m;
    adj.assign(n + 1, {});
    tunnels.resize(m);
    for(auto& [u, w]: tunnels) {
        cin >> u >> w;
        adj[u].push_back(w);
        adj[w].push_back(u);
    }
    int n1;
    cin >> n1;
    a_set.resize(n1);
    cin >> a_set;
    int n2;
    cin >> n2;
    b_set.resize(n2);
    cin >> b_set;
}

void solve() {
    // We need an unextendable family of vertex-disjoint shortest paths from the
    // A-labs to the B-labs, where every path has length K = min over a in A,
    // b in B of dist(a, b).
    //
    // A multi-source BFS from all B-labs gives dist[v] = distance to the
    // nearest exit, and K is the minimum dist over the A-labs. Only A-labs with
    // dist == K can host a path: a length-K shortest path visits vertices with
    // strictly decreasing distance K, K-1, ..., 0, so its start sits at
    // distance exactly K and its end at distance 0 (a B-lab). Consequently
    // A-labs only ever appear as starts and B-labs only as ends, so we never
    // have to treat them as interior vertices.
    //
    // The shortest-path DAG keeps an oriented edge u -> w whenever {u, w} is a
    // tunnel with dist[u] == dist[w] + 1 (one step closer to an exit). To
    // forbid sharing a lab we split every vertex into an in-node and an
    // out-node joined by a capacity-1 edge; a super source feeds the in-nodes
    // of the participating A-labs and the out-nodes of the B-labs feed a super
    // sink, all with capacity 1.
    //
    // A blocking flow in this layered unit-capacity network is exactly an
    // unextendable plan: it leaves no residual source-to-sink path along
    // forward edges, i.e. no further disjoint path can be added without
    // rerouting existing ones. We compute it with one greedy DFS phase using
    // current-arc pointers (a node that cannot reach the sink is skipped
    // permanently), then decompose the saturated edges into individual paths.
    //
    // The single blocking-flow phase is O(V + E). Every DFS step either fails
    // and advances a current-arc pointer, which happens at most once per edge
    // since pointers never move back, for O(E) total; or it succeeds and
    // retraces an augmenting path. Because all capacities are 1 and the splits
    // make the paths vertex-disjoint, every vertex lies on at most one path, so
    // the lengths of all augmenting paths sum to at most V. The BFS is also
    // O(V + E), so the whole solution is linear in the graph size.
    //
    // This is why the heavier MKM (Malhotra-Kumar-Maheshwari) blocking-flow
    // algorithm, O(V^2 + E), is not needed here: MKM pays off when capacities
    // are large and a single augmenting path can leave an edge partly used, so
    // current-arc DFS could revisit the same edge many times. With unit
    // capacities every traversed edge is immediately saturated, the naive DFS
    // phase is already O(V + E), and the extra machinery would only add cost.

    const int INF = INT_MAX;
    vector<int> dist(n + 1, INF);
    queue<int> bfs;
    for(int b: b_set) {
        if(dist[b] == INF) {
            dist[b] = 0;
            bfs.push(b);
        }
    }
    while(!bfs.empty()) {
        int u = bfs.front();
        bfs.pop();
        for(int w: adj[u]) {
            if(dist[w] == INF) {
                dist[w] = dist[u] + 1;
                bfs.push(w);
            }
        }
    }

    int k = INF;
    for(int a: a_set) {
        k = min(k, dist[a]);
    }

    int src = 0, snk = 2 * n + 1;
    auto in_node = [&](int v) { return v; };
    auto out_node = [&](int v) { return n + v; };

    vector<edge> edges;
    vector<vector<int>> g(2 * n + 2);
    auto add_edge = [&](int u, int v, int cap) {
        g[u].push_back(edges.size());
        edges.push_back({v, cap, 0});
    };

    for(int a: a_set) {
        if(dist[a] == k) {
            add_edge(src, in_node(a), 1);
        }
    }
    for(int v = 1; v <= n; v++) {
        add_edge(in_node(v), out_node(v), 1);
    }
    for(auto& [u, w]: tunnels) {
        if(dist[u] != INF && dist[u] == dist[w] + 1) {
            add_edge(out_node(u), in_node(w), 1);
        } else if(dist[w] != INF && dist[w] == dist[u] + 1) {
            add_edge(out_node(w), in_node(u), 1);
        }
    }
    for(int b: b_set) {
        add_edge(out_node(b), snk, 1);
    }

    vector<int> ptr(2 * n + 2, 0);
    function<bool(int)> dfs = [&](int u) -> bool {
        if(u == snk) {
            return true;
        }
        for(; ptr[u] < (int)g[u].size(); ptr[u]++) {
            edge& e = edges[g[u][ptr[u]]];
            if(e.cap - e.flow > 0 && dfs(e.to)) {
                e.flow++;
                return true;
            }
        }
        return false;
    };

    while(dfs(src)) {
    }

    vector<vector<int>> paths;
    for(int id: g[src]) {
        if(edges[id].flow == 0) {
            continue;
        }
        vector<int> path;
        int cur = edges[id].to;
        while(true) {
            path.push_back(cur);
            if(dist[cur] == 0) {
                break;
            }
            for(int eid: g[out_node(cur)]) {
                if(edges[eid].flow == 1 && edges[eid].to != snk) {
                    cur = edges[eid].to;
                    break;
                }
            }
        }
        paths.push_back(path);
    }

    cout << paths.size() << ' ' << k << '\n';
    for(auto& path: paths) {
        cout << path << '\n';
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
