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

class EulerianPaths {
  private:
    int n, m;
    vector<vector<pair<int, int>>> adj;
    vector<pair<int, int>> edges;
    vector<int> deg;

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
    EulerianPaths(int _n) : n(_n), m(0) {
        adj.assign(n + 1, {});
        deg.assign(n + 1, 0);
    }

    void add_edge(int u, int v) {
        adj[u].push_back({v, m * 2});
        adj[v].push_back({u, m * 2 + 1});
        edges.push_back({u, v});
        deg[u]++;
        deg[v]++;
        m++;
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
};

int m;
vector<pair<int, int>> dominos;

void read() {
    cin >> m;
    dominos.resize(m);
    cin >> dominos;
}

void solve() {
    EulerianPaths ep(6);
    for(int i = 0; i < m; i++) {
        ep.add_edge(dominos[i].first, dominos[i].second);
    }

    auto paths = ep.find_paths();
    if(paths.size() == 1) {
        for(int edge_i: paths[0]) {
            cout << (edge_i >> 1) + 1 << ' '
                 << (ep.get_edge(edge_i) == dominos[edge_i >> 1] ? '+' : '-')
                 << '\n';
        }
    } else {
        cout << "No solution\n";
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
