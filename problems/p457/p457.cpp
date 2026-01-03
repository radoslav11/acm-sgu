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

    T dfs(int u, int t, T fl = inf) {
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
    constexpr static T inf = numeric_limits<T>::max();

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

int n, m, A, B;
vector<tuple<int, int, int, int>> edges;

void read() {
    cin >> n >> m >> A >> B;
    edges.resize(m);
    for(int i = 0; i < m; i++) {
        auto& [x, y, w, t] = edges[i];
        cin >> x >> y >> w >> t;
    }
}

void solve() {
    // The problem can be formulated as a maxflow with demands. Particularly, a
    // path from A to B can be through as pushing a single unit of flow through
    // edges that have capacities equal to the initial snow on the roads. Note
    // that we are allowed the path to be non-simple. We want to fully clear the
    // historical roads, so this can also be formulated as a minimum demand
    // equal to the capacity. We want to find the largest flow satisfying this
    // (maximum number of paths / days of work).
    //
    // Flow with demands is a classic problem, where we build a new graph that
    // has capacities equal to the difference between the original capacity and
    // the demand and create a fake source and sink that we connect to all nodes
    // in the graph based on their cumulative supply / demand of adjacent edges.
    // There is a full explanation with some intuition in:
    //
    //     https://cp-algorithms.com/graph/flow_with_demands.html
    //
    // The tricky bit in this problem is the cycles, and that we want to
    // reconstruct the answer. We should still first check that we can satisfy
    // the flow with demands as this is a prerequisite for having a solution
    // here.
    //
    // In terms of reconstruction, the first step is that we should always push
    // flow from the real source and sink (A and B in this problem), after we
    // push the flow from S' to T'. This is needed so that we actually have a
    // valid flow rather than a circulation. We now know that we can meet the
    // demands for all historical roads, so let's artificially push +snow flow
    // in their direction and -snow in the opposite. Now if we look at only the
    // positive flow roads, we should have a valid configuration that uses all
    // historical edges. The only challenge left is to figure out how to
    // partition these roads into #days paths.
    //
    // To do this, we can start by greedily with BFS / DFS constructing #days
    // simple paths from A to B. Let's remove these edges - we can notice that
    // we are left with some cycles (technically strongly connected components)
    // that have in and out flow being the same. There might be many of these
    // that are disconnected, but it's important to note that these certainly
    // have a directed Eulerian cycle, meaning that we can easily turn them into
    // a sequence of edges. This is what we will do, and afterwards we are left
    // with only attaching these cycles into the #days path we greedily created
    // - this can be done by finding an intersection, rotating the cycle, and
    // then inserting it into the path from A to B.

    int S = n + 1, T = n + 2;
    MaxFlow<int64_t> mf(n + 3);
    mf.add_edge(B, A, MaxFlow<int64_t>::inf);

    vector<tuple<int, int, int>> hist_edges;
    vector<int> V(n + 1, 0);
    for(auto& [x, y, w, t]: edges) {
        if(t == 0) {
            mf.add_edge(x, y, w);
        } else {
            hist_edges.push_back({x, y, w});
            V[x] -= w;
            V[y] += w;
        }
    }

    int64_t k = 0;
    for(int i = 1; i <= n; i++) {
        if(V[i] > 0) {
            mf.add_edge(S, i, V[i]);
            k += V[i];
        } else if(V[i] < 0) {
            mf.add_edge(i, T, -V[i]);
        }
    }

    int64_t circulation_flow = mf.flow(S, T);
    if(circulation_flow != k) {
        cout << 0 << endl;
        return;
    }

    mf.flow(A, B);
    for(auto& [x, y, w]: hist_edges) {
        mf.add_edge(x, y, w);
        int idx = mf.adj[x].size() - 1;
        mf.adj[x][idx].flow = w;
        mf.adj[y][mf.adj[x][idx].rev].flow = -w;
    }

    auto& adj_A = mf.adj[A];
    int64_t total_paths = 0;
    for(auto& e: adj_A) {
        if(e.to > n) {
            continue;
        }
        total_paths += e.flow;
    }

    if(total_paths == 0) {
        cout << 0 << endl;
        return;
    }

    vector<map<int, int>> res(n + 1);
    for(int u = 1; u <= n; u++) {
        for(int j = 0; j < (int)mf.adj[u].size(); j++) {
            auto& e = mf.adj[u][j];
            if(e.to >= 1 && e.to <= n && e.to != u) {
                if(e.flow > 0) {
                    res[u][e.to] += e.flow;
                }
            }
        }
    }

    vector<bool> in_paths(n + 1, false);

    vector<vector<int>> paths;
    for(int i = 0; i < total_paths; i++) {
        vector<int> path;
        vector<int> parent(n + 1, -1);
        queue<int> q;
        q.push(A);
        parent[A] = A;
        while(!q.empty() && parent[B] == -1) {
            int u = q.front();
            q.pop();
            for(auto& [v, cnt]: res[u]) {
                if(cnt > 0 && parent[v] == -1) {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        for(int v = B; v != A; v = parent[v]) {
            path.push_back(v);
            res[parent[v]][v]--;
        }
        path.push_back(A);
        reverse(path.begin(), path.end());
        paths.push_back(path);
        for(int v: path) {
            in_paths[v] = true;
        }
    }

    function<void(int, vector<int>&)> dfs_euler = [&](int v, vector<int>& path) {
        for(auto& [u, cnt]: res[v]) {
            while(cnt > 0) {
                cnt--;
                dfs_euler(u, path);
            }
        }
        path.push_back(v);
    };

    for(int i = 1; i <= n; i++) {
        for(auto& [j, cnt]: res[i]) {
            while(cnt > 0) {
                vector<int> cycle;
                dfs_euler(i, cycle);
                reverse(cycle.begin(), cycle.end());
                cycle.pop_back();

                set<int> in_cycle(cycle.begin(), cycle.end());

                bool inserted = false;
                for(auto& path: paths) {
                    for(int pos = 0; pos < (int)path.size() && !inserted;
                        pos++) {
                        if(in_cycle.count(path[pos])) {
                            int mid = -1;
                            for(int w = 0; w < (int)cycle.size(); w++) {
                                if(cycle[w] == path[pos]) {
                                    mid = w;
                                    break;
                                }
                            }
                            rotate(
                                cycle.begin(), cycle.begin() + mid, cycle.end()
                            );
                            path.insert(
                                path.begin() + pos, cycle.begin(), cycle.end()
                            );
                            inserted = true;
                        }
                    }
                    if(inserted) {
                        break;
                    }
                }
                assert(inserted);
            }
        }
    }

    cout << total_paths << endl;
    for(auto& path: paths) {
        for(int j = 0; j < (int)path.size(); j++) {
            if(j) {
                cout << " ";
            }
            cout << path[j];
        }
        cout << endl;
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
