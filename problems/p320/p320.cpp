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

int n, m, k;
vector<string> grid;

vector<vector<int>> comp_id;
vector<int> comp_size;
vector<char> comp_touches_border;
vector<set<int>> comp_neighbours;

void read() {
    cin >> n >> m >> k;
    grid.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> grid[i];
    }
}

const int di[4] = {-1, 0, 1, 0};
const int dj[4] = {0, 1, 0, -1};

void label_zones() {
    comp_id.assign(n, vector<int>(m, -1));
    comp_size.clear();
    comp_touches_border.clear();
    comp_neighbours.clear();

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(comp_id[i][j] != -1) {
                continue;
            }

            int cid = comp_size.size();
            comp_size.push_back(0);
            comp_touches_border.push_back(0);
            comp_neighbours.emplace_back();

            char color = grid[i][j];
            queue<pair<int, int>> q;
            q.push({i, j});
            comp_id[i][j] = cid;

            while(!q.empty()) {
                auto [x, y] = q.front();
                q.pop();
                comp_size[cid]++;
                if(x == 0 || x == n - 1 || y == 0 || y == m - 1) {
                    comp_touches_border[cid] = 1;
                }

                for(int d = 0; d < 4; d++) {
                    int nx = x + di[d], ny = y + dj[d];
                    if(nx < 0 || nx >= n || ny < 0 || ny >= m) {
                        continue;
                    }

                    if(grid[nx][ny] == color) {
                        if(comp_id[nx][ny] == -1) {
                            comp_id[nx][ny] = cid;
                            q.push({nx, ny});
                        }
                    } else if(comp_id[nx][ny] != -1) {
                        comp_neighbours[cid].insert(comp_id[nx][ny]);
                        comp_neighbours[comp_id[nx][ny]].insert(cid);
                    }
                }
            }
        }
    }
}

vector<char> dominated_by_big_zone(const vector<char>& is_big) {
    int num_comps = comp_size.size();
    int outside = num_comps;
    int V = num_comps + 1;

    vector<vector<int>> adj(V);
    for(int c = 0; c < num_comps; c++) {
        for(int u: comp_neighbours[c]) {
            adj[c].push_back(u);
        }
        if(comp_touches_border[c]) {
            adj[c].push_back(outside);
            adj[outside].push_back(c);
        }
    }

    vector<int> disc(V, -1), low(V, 0), in_t(V, 0);
    vector<int> dom(V + 2, 0);
    vector<tuple<int, int, int>> st;
    st.reserve(V);
    int timer = 0;

    st.emplace_back(outside, 0, -1);
    disc[outside] = low[outside] = in_t[outside] = timer++;

    while(!st.empty()) {
        auto& [u, it, p] = st.back();
        if(it < (int)adj[u].size()) {
            int v = adj[u][it++];
            if(v == p) {
                continue;
            }

            if(disc[v] == -1) {
                disc[v] = low[v] = in_t[v] = timer++;
                st.emplace_back(v, 0, u);
            } else {
                low[u] = min(low[u], disc[v]);
            }
        } else {
            int u_low = low[u], u_in = in_t[u], u_out = timer - 1, parent = p;
            st.pop_back();
            if(parent == -1) {
                continue;
            }

            low[parent] = min(low[parent], u_low);
            if(u_low >= disc[parent] && parent < num_comps && is_big[parent]) {
                dom[u_in] += 1;
                dom[u_out + 1] -= 1;
            }
        }
    }

    for(int t = 1; t < timer; t++) {
        dom[t] += dom[t - 1];
    }

    vector<char> result(num_comps, 0);
    for(int c = 0; c < num_comps; c++) {
        result[c] = dom[in_t[c]] > 0;
    }
    return result;
}

void solve() {
    // If we group same-color 4-connected cells into zones, the resulting
    // structure is planar. A cell is dangerous iff it is part of a big zone
    // (size >= K) or lies strictly inside a face of some big zone, i.e.
    // there is a big zone X such that every escape path from the cell to
    // the border passes through X. Equivalently, X is an articulation point
    // separating the cell's zone from a virtual OUTSIDE node in the zone
    // adjacency graph.
    //
    // We flood fill once to label every zone with a component id and to
    // record its size, whether it touches the border, and the set of
    // neighbouring zones. Then we build the zone graph with an extra
    // OUTSIDE node joined to every border-touching zone and run Tarjan's
    // articulation point algorithm rooted at OUTSIDE. Whenever a DFS-tree
    // child v of u finishes with low[v] >= disc[u] and u is a big zone, u
    // cuts the entire subtree of v from OUTSIDE, so every zone in v's
    // subtree is dangerous. We mark each such subtree as a +1/-1 range on
    // the Euler-tour index and prefix-sum at the end.

    label_zones();

    int num_comps = comp_size.size();
    vector<char> is_big(num_comps, 0);
    for(int c = 0; c < num_comps; c++) {
        is_big[c] = comp_size[c] >= k;
    }

    auto dominated = dominated_by_big_zone(is_big);

    int ans = 0;
    for(int c = 0; c < num_comps; c++) {
        if(is_big[c] || dominated[c]) {
            ans += comp_size[c];
        }
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
        solve();
    }

    return 0;
}
