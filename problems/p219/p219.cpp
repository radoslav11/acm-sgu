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
vector<vector<int>> all;
vector<int> alive;

StronglyConnectedComponents scc;
vector<bool> has_zero_self_loop;

void read() {
    cin >> n >> m;
    all.resize(n);
    scc.init(n);
    has_zero_self_loop.resize(n);
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        all[u].push_back(v);
        if(w == 0) {
            if(u == v) {
                has_zero_self_loop[u] = true;
            } else {
                scc.add_edge(u, v);
            }
        }
    }
}

void dfs(int u) {
    for(int v: all[u]) {
        if(alive[v] == 1) {
            alive[v] = 0;
            dfs(v);
        }
    }
}

void solve() {
    // The problem is fairly simple, we just need to make a few observations:
    //     1) If a node is not alive, all nodes reachable from it are also not
    //        alive. This is trivial to prove.
    //     2) We don't actually care about the concrete w values, only whether
    //        they are 1 or 0.
    //     3) We might have self loops. If there is a w=0 self loop, then it's
    //        certainly impossible. If it's a w=1 self loop, it can be ignored.
    //     4) Arguably the main observation - if we have a w=0 cycle of > 1
    //        nodes, then all nodes in it can't be alive. This is generalized to
    //        the nodes in a strongly connected component. This is easy to show
    //        as there is always at least one incoming edge that can't fire.
    //     5) All other nodes are alive. In particular, a good intuition is to
    //        think about nodes that have in_degree = 0. They effectively let us
    //        generate tokens for free, which we can later propagate through the
    //        structure. If there are no such nodes, then we have cycles that
    //        have all incoming edges "full", or there is w=0 cycle. We can make
    //        an inductive argument by considering the "highest" nodes /
    //        strongly connected component.
    //
    // Then the solution is fairly simple - we keep two graphs G with all edges,
    // and H with only the 0 edges. We create the strongly connected components
    // in H, and if some of them has size > 1, all nodes are marked as not
    // alive. Finally we propagate down from every non alive node over the edges
    // of G. The time complexity is O(N+M) if we use Tarjan or Kosaraju for the
    // SCC.

    alive.assign(n, 1);
    scc.find_components();

    vector<int> comp_size(scc.comp_ids.size());
    for(int i = 0; i < n; i++) {
        comp_size[scc.comp[i]]++;
    }
    for(int i = 0; i < n; i++) {
        if(has_zero_self_loop[i] || comp_size[scc.comp[i]] > 1) {
            alive[i] = 0;
        }
    }
    for(int i = 0; i < n; i++) {
        if(alive[i] == 0) {
            dfs(i);
        }
    }
    for(int i = 0; i < n; i++) {
        cout << alive[i] << "\n";
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
