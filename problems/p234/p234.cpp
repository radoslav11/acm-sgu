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
        // König: alternating-BFS from unmatched left vertices marks the
        // reachable set Z; cover = (Left \ Z) on the left side together with
        // (Right in Z) on the right side. Here dist[u] != -1 means u is in Z.

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

    pair<vector<int>, vector<int>> maximum_independent_set() {
        // The complement of a minimum vertex cover is a maximum independent
        // set (König), so keep exactly the vertices not in the cover.

        auto [left_cover, right_cover] = minimum_vertex_cover();
        vector<char> in_left_cover(n), in_right_cover(m);
        for(int u: left_cover) {
            in_left_cover[u] = 1;
        }

        for(int v: right_cover) {
            in_right_cover[v] = 1;
        }

        vector<int> left_set, right_set;
        for(int u = 0; u < n; u++) {
            if(!in_left_cover[u]) {
                left_set.push_back(u);
            }
        }

        for(int v = 0; v < m; v++) {
            if(!in_right_cover[v]) {
                right_set.push_back(v);
            }
        }

        return {left_set, right_set};
    }
};

using BipartiteMatching = HopcroftKarp;

int m, n;
vector<vector<int>> grid;

void read() {
    cin >> m >> n;
    grid.assign(m, vector<int>(n));
    for(auto& row: grid) {
        cin >> row;
    }
}

void solve() {
    // A Guardian attacks its own cell and the four edge-neighbors. Two
    // Guardians may not stand on the same or neighboring cells (they would
    // kill each other), so the chosen cells form an independent set in the
    // grid graph. Every present cell must also be attacked by some Guardian,
    // i.e. the chosen set must be dominating. We want the largest such set.
    //
    // A maximum independent set is automatically dominating: if some cell
    // were neither chosen nor adjacent to a chosen cell, we could add it and
    // get a larger independent set, contradicting maximality. So the answer
    // is just the maximum independent set of the grid graph.
    //
    // The grid graph is bipartite under a checkerboard coloring, with edges
    // only between an even cell (i + j even) and an odd neighbor. By Koenig's
    // theorem the maximum independent set is the complement of a minimum
    // vertex cover, which we recover from a maximum matching: an even cell is
    // chosen iff it is not in the cover, and likewise for odd cells.

    vector<vector<int>> id(m, vector<int>(n, -1));
    int num_even = 0, num_odd = 0;
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(grid[i][j]) {
                id[i][j] = ((i + j) & 1) ? num_odd++ : num_even++;
            }
        }
    }

    HopcroftKarp hk(num_even, num_odd);
    static const int di[] = {-1, 1, 0, 0};
    static const int dj[] = {0, 0, -1, 1};
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(grid[i][j] && ((i + j) & 1) == 0) {
                for(int d = 0; d < 4; d++) {
                    int ni = i + di[d], nj = j + dj[d];
                    if(ni >= 0 && ni < m && nj >= 0 && nj < n && grid[ni][nj]) {
                        hk.add_edge(id[i][j], id[ni][nj]);
                    }
                }
            }
        }
    }

    hk.max_matching();
    auto [even_set, odd_set] = hk.maximum_independent_set();

    vector<char> even_chosen(num_even), odd_chosen(num_odd);
    for(int u: even_set) {
        even_chosen[u] = 1;
    }

    for(int v: odd_set) {
        odd_chosen[v] = 1;
    }

    vector<string> out(m, string(n, '#'));
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(!grid[i][j]) {
                continue;
            }

            bool chosen =
                ((i + j) & 1) ? odd_chosen[id[i][j]] : even_chosen[id[i][j]];
            out[i][j] = chosen ? 'G' : '.';
        }
    }

    cout << even_set.size() + odd_set.size() << '\n';
    for(auto& row: out) {
        cout << row << '\n';
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
