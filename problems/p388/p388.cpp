#include <bits/stdc++.h>
// #include <coding_library/graph/mincost_circulation.hpp>

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
class MinCostCirculation {
  private:
    struct Edge {
        int from, to;
        T capacity, cost, flow;
        Edge(int _from, int _to, T _capacity, T _cost, T _flow = 0)
            : from(_from),
              to(_to),
              capacity(_capacity),
              cost(_cost),
              flow(_flow) {}
    };

    int n;
    vector<Edge> edges;
    vector<int> pei, depth;
    vector<T> dual;
    vector<set<int>> tree;

    void dfs(int node) {
        for(auto ei: tree[node]) {
            if(ei == pei[node]) {
                continue;
            }
            int vec = edges[ei].to;
            dual[vec] = dual[node] + edges[ei].cost;
            pei[vec] = (ei ^ 1);
            depth[vec] = 1 + depth[node];
            dfs(vec);
        }
    }

    template<typename CB>
    void walk(int ei, CB&& cb) {
        cb(ei);
        int a = edges[ei].from, b = edges[ei].to;
        while(a != b) {
            if(depth[a] > depth[b]) {
                cb(pei[a] ^ 1), a = edges[pei[a]].to;
            } else {
                cb(pei[b]), b = edges[pei[b]].to;
            }
        }
    }

  public:
    MinCostCirculation(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        edges.clear();
        pei.assign(n + 1, -1);
        depth.assign(n + 1, 0);
        dual.assign(n + 1, 0);
        tree.assign(n + 1, set<int>());
    }

    int size() const { return n; }

    int add_edge(int from, int to, T capacity, T cost) {
        int id = edges.size();
        edges.push_back(Edge(from, to, capacity, cost, 0));
        edges.push_back(Edge(to, from, 0, -cost, 0));
        return id;
    }

    T min_circulation() {
        for(int i = 0; i < n; i++) {
            int ei = add_edge(n, i, 0, 0);
            tree[n].insert(ei);
            tree[i].insert(ei ^ 1);
        }

        T answer = 0;
        T flow;
        int cost, ein, eout, ptr = 0;
        const int B = 3 * n;
        for(int z = 0; z < (int)edges.size() / B + 1; z++) {
            if(!z) {
                dfs(n);
            }

            pair<T, int> pin = {0, -1};
            for(int t = 0; t < B; t++, (++ptr) %= (int)edges.size()) {
                auto& e = edges[ptr];
                if(e.flow < e.capacity) {
                    pin =
                        min(pin,
                            make_pair(dual[e.from] + e.cost - dual[e.to], ptr));
                }
            }

            tie(cost, ein) = pin;
            if(cost == 0) {
                continue;
            }

            pair<T, int> pout = {edges[ein].capacity - edges[ein].flow, ein};
            walk(ein, [&](int ei) {
                pout =
                    min(pout,
                        make_pair(edges[ei].capacity - edges[ei].flow, ei));
            });

            tie(flow, eout) = pout;
            walk(ein, [&](int ei) {
                edges[ei].flow += flow, edges[ei ^ 1].flow -= flow;
            });

            tree[edges[ein].from].insert(ein);
            tree[edges[ein].to].insert(ein ^ 1);
            tree[edges[eout].from].erase(eout);
            tree[edges[eout].to].erase(eout ^ 1);

            answer += flow * cost;
            z = -1;
        }
        return answer;
    }

    const Edge& get_edge(int id) const { return edges[id]; }
};

int n, m1, m2;
vector<vector<int>> G;

void read() {
    cin >> n >> m1 >> m2;
    G.assign(n, vector<int>(n, 0));
    for(int i = 0; i < m1; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        G[u][v] |= 1;
        G[v][u] |= 1;
    }
    for(int i = 0; i < m2; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        G[u][v] |= 2;
        G[v][u] |= 2;
    }
}

void solve() {
    // This problem is conceptually not that hard - we are give two sets of
    // edges E1 and E2, and we want to find the largest subset of actors, such
    // that there is a perfect matching among both E1 and E2. One simplification
    // is that the problem has a "traditional" view and assumes the graph is
    // bipartite.
    // To solve this, one of the core ideas is that we can first colour the
    // nodes, and then direct the edges from Juan and Rosa in opposite
    // direction. Then selecting a subset of people is equivalent to selecting a
    // set of node disjoint cycles with the largest number of edges. We can
    // think of this as a circulation problem, but the tricky bit is that we are
    // interested in maximum so we will have negative cycles (as we need to
    // invert all weights). Classical algorithms unfortunately fail, so we
    // either need to go for cost scaling push relabel, or network simplex (or
    // some slower variants like cycle elimination). Personally, the network
    // simplex is the most intuitive and it's easier to implement so here we opt
    // for that.
    // It's unclear by the problem statement, so one thing we should make sure
    // is that the cycles don't have common nodes. An easy way to do this is to
    // split each node into two parts "u" and "u+n", with capacity 1 between
    // than and cost 0. Then all incoming edges will go to "u", while all
    // outgoing will come out of "u+n".
    // It might be good to read up on min circulation, as before this problem
    // I had incorrectly believed that a good chunk of flow implementations
    // actually work for negative cycles:
    //     - https://codeforces.com/blog/entry/104075?#comment-925064
    //     - https://codeforces.com/blog/entry/57018
    //     - https://codeforces.com/blog/entry/94190 (+ regular simplex)

    vector<int> color(n, -1);
    function<bool(int, int)> check_bipartite_dfs = [&](int u, int c) -> bool {
        color[u] = c;
        for(int v = 0; v < n; v++) {
            if(!G[u][v]) {
                continue;
            }
            if(color[v] == -1) {
                if(!check_bipartite_dfs(v, 1 - c)) {
                    return false;
                }
            } else if(color[v] == color[u]) {
                return false;
            }
        }
        return true;
    };

    for(int i = 0; i < n; i++) {
        if(color[i] == -1) {
            assert(check_bipartite_dfs(i, 0));
        }
    }

    MinCostCirculation<int64_t> mcc(2 * n);
    vector<tuple<int, int, int, int>> edge_info;

    for(int i = 0; i < n; i++) {
        mcc.add_edge(i, i + n, 1, 0);
        for(int j = i + 1; j < n; j++) {
            if(!G[i][j]) {
                continue;
            }

            // Juan edges: always go 0->1
            // Rosa edges: always go 1->0
            if(G[i][j] == 1 || G[i][j] == 3) {
                if(color[i] == 0 && color[j] == 1) {
                    int ei = mcc.add_edge(i + n, j, 1, -1);
                    edge_info.push_back({ei, i, j, 1});
                } else {
                    int ei = mcc.add_edge(j + n, i, 1, -1);
                    edge_info.push_back({ei, i, j, 1});
                }
            }
            if(G[i][j] == 2 || G[i][j] == 3) {
                if(color[i] == 1 && color[j] == 0) {
                    int ei = mcc.add_edge(i + n, j, 1, -1);
                    edge_info.push_back({ei, i, j, 2});
                } else {
                    int ei = mcc.add_edge(j + n, i, 1, -1);
                    edge_info.push_back({ei, i, j, 2});
                }
            }
        }
    }

    int ans = -mcc.min_circulation() / 2;

    vector<pair<int, int>> juan, rosa;
    for(auto [ei, u, v, type]: edge_info) {
        if(mcc.get_edge(ei).flow > 0) {
            if(type == 1) {
                juan.push_back({u, v});
            } else {
                rosa.push_back({u, v});
            }
        }
    }

    cout << ans << '\n';
    for(auto [u, v]: juan) {
        cout << u + 1 << ' ' << v + 1 << '\n';
    }
    for(auto [u, v]: rosa) {
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
