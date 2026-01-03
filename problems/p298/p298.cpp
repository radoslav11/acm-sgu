#include <bits/stdc++.h>
// #include <coding_library/graph/scc.hpp>

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

class StronglyConnectedComponents {
  private:
    vector<bool> visited;

    void dfs1(int u) {
        visited[u] = true;
        for(int v: adj[u]) {
            if(!visited[v]) {
                dfs1(v);
            }
        }

        top_sort.push_back(u);
    }

    void dfs2(int u) {
        for(int v: radj[u]) {
            if(comp[v] == -1) {
                comp[v] = comp[u];
                dfs2(v);
            }
        }
    }

  public:
    int n;
    vector<vector<int>> adj, radj;
    vector<int> comp, comp_ids, top_sort;

    StronglyConnectedComponents() {}
    StronglyConnectedComponents(int _n) { init(_n); }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    void init(int _n) {
        n = _n;
        comp_ids.clear();
        top_sort.clear();
        adj.assign(n, {});
        radj.assign(n, {});
    }

    void find_components() {
        comp.assign(n, -1);
        visited.assign(n, false);

        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                dfs1(i);
            }
        }

        reverse(top_sort.begin(), top_sort.end());
        for(int u: top_sort) {
            if(comp[u] == -1) {
                comp[u] = (int)comp_ids.size();
                comp_ids.push_back(comp[u]);
                dfs2(u);
            }
        }
    }
};

int n, m;
vector<tuple<int, int, int>> edges;

void read() {
    cin >> n >> m;
    edges.resize(m);
    for(int i = 0; i < m; i++) {
        int x, y, c;
        cin >> x >> y >> c;
        x--;
        y--;
        edges[i] = {x, y, c};
    }
}

pair<bool, vector<int>> check(vector<tuple<int, int, int>> extra_edges) {
    StronglyConnectedComponents scc(n);
    for(auto& [x, y, c]: edges) {
        scc.add_edge(y, x);
    }
    for(auto& [x, y, c]: extra_edges) {
        scc.add_edge(y, x);
    }
    scc.find_components();

    for(auto& [x, y, c]: edges) {
        if(scc.comp[x] == scc.comp[y] && c > 0) {
            return {false, {}};
        }
    }
    for(auto& [x, y, c]: extra_edges) {
        if(scc.comp[x] == scc.comp[y]) {
            return {false, {}};
        }
    }

    int num_comps = (int)scc.comp_ids.size();
    vector<vector<pair<int, int>>> comp_adj(num_comps);
    vector<int> indegree(num_comps, 0);

    for(auto& [x, y, c]: edges) {
        int cx = scc.comp[x], cy = scc.comp[y];
        if(cx != cy) {
            comp_adj[cy].push_back({cx, c});
            indegree[cx]++;
        }
    }
    for(auto& [x, y, c]: extra_edges) {
        int cx = scc.comp[x], cy = scc.comp[y];
        if(cx != cy) {
            comp_adj[cy].push_back({cx, c});
            indegree[cx]++;
        }
    }

    vector<int> comp_val(num_comps, -10000);
    vector<int> topo_order;
    queue<int> q;
    for(int i = 0; i < num_comps; i++) {
        if(indegree[i] == 0) {
            q.push(i);
        }
    }

    while(!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        for(auto [v, c]: comp_adj[u]) {
            comp_val[v] = max(comp_val[v], comp_val[u] + c);
            indegree[v]--;
            if(indegree[v] == 0) {
                q.push(v);
            }
        }
    }

    for(int i = num_comps - 1; i >= 0; i--) {
        int u = topo_order[i], val = 10001;
        for(auto [v, c]: comp_adj[u]) {
            val = min(val, comp_val[v] - c);
        }

        if(val < 10001) {
            comp_val[u] = val;
        }
    }

    vector<int> ans(n);
    for(int i = 0; i < n; i++) {
        ans[i] = comp_val[scc.comp[i]];
        if(ans[i] < -10000 || ans[i] > 10000) {
            return {false, {}};
        }
    }

    return {true, ans};
}

void solve() {
    // Let's start with verifying if a valid configuration is possible, rather
    // than trying to minimize the difference. We will have edges (u -> v, c),
    // representing a[u] >= a[v] + c. If we had a DAG, it would be fairly to
    // check if this is possible to do this with DP - the 0 in degree nodes
    // will be assigned with -10000, and we recursively / in topological order
    // try to set the lowest allowed value. When there is a cycle, we can notice
    // that all edges with endpoints in it should actually have c = 0.
    // Otherwise, it's impossible which is easy to show with contradiction if we
    // look at the inequalities in the cycle. Furthermore, all nodes u in these
    // 0-cycles should be assigned the same value a[u]. This means we can simply
    // compress the strongly connected components, run the DP, check that there
    // is no edge with c > 0 in a SCC, and finally check that all values are
    // within [-10000; 10000] after running the DP. This check runs in O(n + m).
    // Something we should be careful about and will come in the last paragraph
    // is that we should "tighten" the constraints over the DAG after we do the
    // initial setting.
    //
    // Now let's try to get the lowest difference of a[n] - a[1]. We have an
    // oracle that quickly verifies that there is a solution, so we should think
    // if we could potentially exploit this. The standard approach is to
    // consider if we could potentially binary search on the answer, and it
    // turns out this is the case. Particularly, we can add a constraint a[1] >=
    // a[n] + K and binary search for the largest K for which this is possible
    // (the answer is then -K). There is a duality argument we could use here
    // too, but it's not hard to get convinced this is equivalent. Note that the
    // K we binary search over could also be negative for the case a[1] <= a[n]
    // so the bounds should be -20000 to 20000.
    //
    // There is a special case when adding the edge n -> 1 creates a cycle. In
    // this case, we would need K to be negative, which is non trivial to
    // handle. Instead, we first check if a solution exists without any extra
    // edges. If not, we output -1. Otherwise, we binary search but return false
    // whenever a cycle is created. If the binary search finds no valid K, it
    // means the edge always creates a cycle, implying nodes 1 and n are already
    // in the same SCC. In this case, the initial check({}) already gives the
    // minimal difference solution since 1 and n must either have the same
    // value, or there is a direct dependence between the two and the DP always
    // tries to minimize the new value. We should be careful about the fact that
    // we start from -10000 though. Consider the case of N depending on some
    // vertex v with a large delta, and on 1 with a smaller delta. Then running
    // the DP the critical constraint would be the one between N and v, while 1
    // will be set to -10000 by the DP. In other words there will be slack in
    // the 1-N constraint. However, it makes more sense to just assign 1 a
    // higher value so that the constraint 1-N becomes tight. This can be done
    // by doing a second pass over the reverse topological order, and increasing
    // the values if we can.
    //
    // There is also a Bellman-Ford solution that is significantly simpler and
    // without binary search, but it runs slower (the one described here is
    // O((n+m) log MAX)).

    auto [base_ok, base_ans] = check({});
    if(!base_ok) {
        cout << -1 << '\n';
        return;
    }

    int low = -20000, high = 20000, mid;
    vector<int> ans;
    while(low <= high) {
        mid = (low + high) / 2;
        auto [ok, res] = check({{0, n - 1, mid}});
        if(ok) {
            ans = res;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if(ans.empty()) {
        ans = base_ans;
    }
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
