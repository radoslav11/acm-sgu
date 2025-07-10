#include <bits/stdc++.h>
// #include <coding_library/graph/eulerian_paths.hpp>

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

class EulerianPaths {
  private:
    void dfs(int u, vector<int>& path, vector<bool>& used, vector<int>& po) {
        for(; po[u] < (int)adj[u].size();) {
            int idx = po[u]++;
            if(!used[adj[u][idx].second >> 1]) {
                used[adj[u][idx].second >> 1] = true;
                dfs(adj[u][idx].first, path, used, po);
                path.push_back(adj[u][idx].second);
            }
        }
    }

  public:
    int n, m;
    vector<int> deg;
    vector<vector<pair<int, int>>> adj;
    vector<pair<int, int>> edges;

    EulerianPaths(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        m = 0;
        adj.assign(n + 1, {});
        deg.assign(n + 1, 0);
        edges.clear();
    }

    int add_edge(int u, int v) {
        adj[u].push_back({v, m * 2});
        adj[v].push_back({u, m * 2 + 1});
        edges.push_back({u, v});
        deg[u]++;
        deg[v]++;
        m++;

        return edges.size() - 1;
    }

    vector<vector<int>> find_paths() {
        vector<bool> used(m, false);
        vector<int> po(n + 1, 0);

        vector<int> odd_vertices;
        for(int i = 0; i <= n; i++) {
            if(deg[i] % 2 == 1) {
                odd_vertices.push_back(i);
            }
        }

        int total_edges = m;
        for(int i = 0; i < (int)odd_vertices.size() / 2; i++) {
            int u = odd_vertices[2 * i], v = odd_vertices[2 * i + 1];
            adj[u].push_back({v, 2 * total_edges});
            adj[v].push_back({u, 2 * total_edges + 1});
            total_edges++;
            used.push_back(false);
            edges.push_back({u, v});
        }

        vector<vector<int>> paths;
        for(int u = 0; u <= n; u++) {
            if(!adj[u].empty()) {
                vector<int> path;
                dfs(u, path, used, po);
                if(!path.empty()) {
                    // Rotate the path so that we always start with a fake edge
                    // if there is at least one.
                    auto it = find_if(path.begin(), path.end(), [&](int x) {
                        return x >= 2 * m;
                    });
                    if(it != path.end()) {
                        rotate(path.begin(), it, path.end());
                    }

                    vector<int> current_path;
                    for(int x: path) {
                        if(x < 2 * m) {
                            current_path.push_back(x);
                        } else if(!current_path.empty()) {
                            paths.push_back(current_path);
                            current_path.clear();
                        }
                    }
                    if(!current_path.empty()) {
                        paths.push_back(current_path);
                    }
                }
            }
        }

        return paths;
    }

    pair<int, int> get_edge(int edge_i) {
        if(edge_i & 1) {
            return edges[edge_i >> 1];
        } else {
            return {edges[edge_i >> 1].second, edges[edge_i >> 1].first};
        }
    }

    vector<pair<int, int>> get_path_edges(const vector<int>& path) {
        vector<pair<int, int>> result;
        for(int edge_i: path) {
            result.push_back(get_edge(edge_i));
        }
        return result;
    }

    bool is_cycle(const vector<int>& path) {
        int start = get_edge(path[0]).first;
        int end = get_edge(path.back()).second;
        return start == end;
    }
};

int n;
map<pair<int, int>, int> edge_id;
vector<vector<int>> input;
EulerianPaths ep;

void read() {
    cin >> n;
    input.resize(n);
    ep.init(n);
    for(int i = 0; i < n; i++) {
        input[i].clear();
        int x;
        cin >> x;
        while(x != 0) {
            x--;
            if(edge_id.count({i, x})) {
                input[i].push_back(edge_id[{i, x}]);
            } else {
                int edge = ep.add_edge(i, x);
                edge_id[{x, i}] = edge;
                edge_id[{i, x}] = edge;
                input[i].push_back(edge);
            }
            cin >> x;
        }
    }
}

void solve() {
    vector<int> state(ep.m, -1);
    vector<int> mask(n, 0);
    auto paths = ep.find_paths();
    for(auto& path: paths) {
        int f = 1;
        for(int edge_2x: path) {
            state[edge_2x >> 1] = f;
            auto [u, v] = ep.get_edge(edge_2x);
            mask[u] |= 1 << f;
            mask[v] |= 1 << f;
            f = 3 - f;
        }
    }

    // 110
    const int need_mask = 6;
    for(int i = 0; i < n; i++) {
        if(ep.deg[i] >= 2 && mask[i] != need_mask) {
            cout << "No solution\n";
            return;
        }
    }


    for(int i = 0; i < n; i++) {
        for(int edge: input[i]) {
            cout << state[edge] << ' ';
        }
        cout << "0\n";
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
