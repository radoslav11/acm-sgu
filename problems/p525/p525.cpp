#include <bits/stdc++.h>
using namespace std;

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
vector<pair<int, int>> edges;
StronglyConnectedComponents G;

void read() {
    cin >> n >> m;
    edges.resize(m);
    G.init(n);
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        edges[i] = {u, v};
        G.add_edge(u, v);
    }
}

void solve() {
    // Both N and M aren't huge, and the naive solution is O(M^2) with trying to
    // add every edge and then finding the largest strongly connected component.
    // However, to have something acceptable we would have to implement a O(NM)
    // approach. This is possible by a slight modification - we will only
    // calculate the strongly connected components decomposition in the
    // beginning in O(M), and then calculate the transitive closure between the
    // components in O(NM), or even O(NM/w) with bitset (word size w = 32 or
    // 64). We will then try every edge (u, v) and if u and v are in different
    // components, we will add an edge v -> u (note u -> v are already an edge),
    // we will merge comp[u], comp[v], and all w such that C[comp[u], w] and
    // C[w, comp[v]] (here C[x, y] means y is reachable from x or essentially
    // this is the transitive closure). This check can be done in O(N) by simply
    // iterating through all w, which is quicker than O(M).

    G.find_components();

    int num_components = G.comp_ids.size();

    vector<vector<bool>> used(
        num_components, vector<bool>(num_components, false)
    );
    vector<vector<int>> comp_adj(num_components);
    vector<int> indegree(num_components, 0);

    for(int i = 0; i < m; i++) {
        int u = edges[i].first, v = edges[i].second;
        int cu = G.comp[u], cv = G.comp[v];
        if(cu != cv && !used[cu][cv]) {
            used[cu][cv] = true;
            comp_adj[cu].push_back(cv);
            indegree[cv]++;
        }
    }

    vector<int> comp_size(num_components, 0);
    for(int i = 0; i < n; i++) {
        comp_size[G.comp[i]]++;
    }

    queue<int> q;
    vector<int> topo_order;
    for(int i = 0; i < num_components; i++) {
        if(indegree[i] == 0) {
            q.push(i);
        }
    }

    while(!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);

        for(int v: comp_adj[u]) {
            indegree[v]--;
            if(indegree[v] == 0) {
                q.push(v);
            }
        }
    }

    vector<bitset<1024>> closure(num_components);
    for(int i = 0; i < num_components; i++) {
        closure[i][i] = 1;
    }
    for(int i = num_components - 1; i >= 0; i--) {
        int u = topo_order[i];
        for(int v: comp_adj[u]) {
            closure[u] |= closure[v];
        }
    }

    int max_size = 0;
    vector<int> good_edges;

    for(int edge_idx = 0; edge_idx < m; edge_idx++) {
        int u = edges[edge_idx].first, v = edges[edge_idx].second;
        int cu = G.comp[u], cv = G.comp[v];

        vector<bool> will_merge(num_components, false);
        for(int i = 0; i < num_components; i++) {
            if(closure[cu][i] && closure[i][cv]) {
                will_merge[i] = true;
            }
        }

        vector<int> new_comp_size(num_components, 0);
        int merged_size = 0;

        for(int i = 0; i < n; i++) {
            int comp_id = G.comp[i];
            if(will_merge[comp_id]) {
                merged_size++;
            } else {
                new_comp_size[comp_id]++;
            }
        }

        int current_max = merged_size;
        for(int i = 0; i < num_components; i++) {
            current_max = max(current_max, new_comp_size[i]);
        }

        if(current_max > max_size) {
            max_size = current_max;
            good_edges = {edge_idx + 1};
        } else if(current_max == max_size) {
            good_edges.push_back(edge_idx + 1);
        }
    }

    if(m == 0) {
        max_size = 1;
        good_edges = {};
    }

    cout << max_size << endl;
    cout << good_edges.size() << endl;
    for(int i = 0; i < (int)good_edges.size(); i++) {
        if(i > 0) {
            cout << " ";
        }
        cout << good_edges[i];
    }
    if(!good_edges.empty()) {
        cout << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
