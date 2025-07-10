#include <bits/stdc++.h>
// #include <coding_library/graph/hopcroft_karp.hpp>

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

// Hopcroft-Karp algorithm for bipartite matching that runs in O(E sqrt(V)).
// Motivated by this submission: https://judge.yosupo.jp/submission/52112, but
// adapted to match coding_library/graph/bipartite_matching.cpp.

class HopcroftKarp {
  private:
    int n, m;
    vector<int> dist;

    bool bfs() {
        queue<int> q;
        dist.assign(n, -1);
        for(int u = 0; u < n; u++) {
            if(inv_match[u] == -1) {
                dist[u] = 0;
                q.push(u);
            }
        }

        bool found = false;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v: adj[u]) {
                int m = match[v];
                if(m == -1) {
                    found = true;
                } else if(dist[m] == -1) {
                    dist[m] = dist[u] + 1;
                    q.push(m);
                }
            }
        }

        return found;
    }

    bool dfs(int u) {
        for(int v: adj[u]) {
            int m = match[v];
            if(m == -1 || (dist[m] == dist[u] + 1 && dfs(m))) {
                inv_match[u] = v;
                match[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

  public:
    vector<int> match, inv_match;
    vector<vector<int>> adj;

    HopcroftKarp(int _n, int _m = -1) : n(_n), m(_m == -1 ? _n : _m) {
        adj.assign(n, vector<int>());
        clear(false);
    }

    void clear(bool clear_adj = true) {
        match.assign(m, -1);
        inv_match.assign(n, -1);
        if(clear_adj) {
            adj.assign(n, vector<int>());
        }
    }

    void add_edge(int u, int v) { adj[u].push_back(v); }

    int max_matching(bool shuffle_edges = false) {
        if(shuffle_edges) {
            for(int i = 0; i < n; i++) {
                shuffle(
                    adj[i].begin(), adj[i].end(),
                    mt19937(
                        chrono::steady_clock::now().time_since_epoch().count()
                    )
                );
            }
        }

        int ans = 0;
        while(bfs()) {
            for(int u = 0; u < n; u++) {
                if(inv_match[u] == -1 && dfs(u)) {
                    ans++;
                }
            }
        }
        return ans;
    }

    vector<pair<int, int>> get_matching() {
        vector<pair<int, int>> matches;
        for(int u = 0; u < n; u++) {
            if(inv_match[u] != -1) {
                matches.emplace_back(u, inv_match[u]);
            }
        }
        return matches;
    }

    pair<vector<int>, vector<int>> minimum_vertex_cover() {
        vector<int> left_cover, right_cover;
        bfs();

        for(int u = 0; u < n; u++) {
            if(dist[u] == -1) {
                left_cover.push_back(u);
            }
        }

        for(int v = 0; v < m; v++) {
            if(match[v] != -1 && dist[match[v]] != -1) {
                right_cover.push_back(v);
            }
        }

        return {left_cover, right_cover};
    }
};

using BipartiteMatching = HopcroftKarp;

int n, p;
vector<pair<int, int>> removed;

void read() {
    cin >> n >> p;
    removed.resize(p);
    cin >> removed;
}

void solve() {
    vector<vector<bool>> board(n + 1, vector<bool>(n + 1, true));
    int total_cells = n * n;

    for(auto [x, y]: removed) {
        board[x][y] = false;
        total_cells--;
    }

    if(total_cells % 2 == 1) {
        cout << "No\n";
        return;
    }

    vector<pair<int, int>> black_cells, white_cells;
    map<pair<int, int>, int> black_id, white_id;

    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(!board[i][j]) {
                continue;
            }

            if((i + j) % 2 == 0) {
                black_id[{i, j}] = black_cells.size();
                black_cells.push_back({i, j});
            } else {
                white_id[{i, j}] = white_cells.size();
                white_cells.push_back({i, j});
            }
        }
    }

    BipartiteMatching bm(black_cells.size(), white_cells.size());

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for(int i = 0; i < (int)black_cells.size(); i++) {
        auto [x, y] = black_cells[i];

        for(int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if(nx >= 1 && nx <= n && ny >= 1 && ny <= n && board[nx][ny]) {
                if(white_id.count({nx, ny})) {
                    bm.add_edge(i, white_id[{nx, ny}]);
                }
            }
        }
    }

    int matching = bm.max_matching();
    if(matching * 2 != total_cells) {
        cout << "No\n";
        return;
    }

    cout << "Yes\n";
    vector<pair<int, int>> horizontal, vertical;
    for(int i = 0; i < (int)black_cells.size(); i++) {
        if(bm.inv_match[i] != -1) {
            auto [bx, by] = black_cells[i];
            auto [wx, wy] = white_cells[bm.inv_match[i]];

            if(bx == wx) {
                if(by < wy) {
                    horizontal.push_back({bx, by});
                } else {
                    horizontal.push_back({wx, wy});
                }
            } else {
                if(bx < wx) {
                    vertical.push_back({bx, by});
                } else {
                    vertical.push_back({wx, wy});
                }
            }
        }
    }

    cout << vertical.size() << "\n";
    for(auto [x, y]: vertical) {
        cout << x << " " << y << "\n";
    }

    cout << horizontal.size() << "\n";
    for(auto [x, y]: horizontal) {
        cout << x << " " << y << "\n";
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
