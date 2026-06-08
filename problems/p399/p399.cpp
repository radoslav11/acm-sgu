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

int N, M;
vector<string> grid;

void read() {
    cin >> N >> M;
    grid.assign(N, "");
    for(int i = 0; i < N; i++) {
        cin >> grid[i];
    }
}

void solve() {
    // The grid is tiny (N, M <= 15), so we can afford to enumerate "where
    // pipes meet" directly. It's very important to note that there are only two
    // craters which is mentioned in the problem statement. A pipe set is a
    // collection of '.' cells; any optimal solution falls into one of two
    // shapes:
    //
    //     (a) two independent paths, each connecting one crater to the ocean;
    //     (b) a single connected '*' component that touches the ocean and
    //         at least two craters (the Steiner / branched shape).
    //
    // We label craters by 4-connected flood fill of the 'X' cells, and for
    // each crater i collect its ports = the '.' cells edge-adjacent to it.
    // Every '.' cell on the border is an ocean port (the area around the isle
    // is ocean). Then we run two flavors of multi-source BFS over '.' cells:
    //
    //     distO[r][c]   = min cells from (r,c) to any ocean port (1 at the
    //                     port itself, +1 per step), with parent pointers;
    //     distC[i][r][c] = same thing but sourced from crater i's ports.
    //
    // For shape (a), the cheapest single-crater connection has cost
    // cost[i] = min over ports p of crater i of distO[p]; pick the two
    // craters with the smallest cost[], reconstruct each path via parentO,
    // and take the union (the two paths may share cells, so we count by set
    // union, not sum).
    //
    // For shape (b), the optimal Steiner tree on three terminals (ocean +
    // two craters) in an unweighted graph is always realized by three
    // shortest paths meeting at a single vertex m. Iterate over every pair
    // i < j of craters and every candidate meeting cell m; the union of the
    // three paths uses
    //
    //     distO[m] + (distC[i][m] - 1) + (distC[j][m] - 1)
    //
    // cells (the -1's account for m being shared). Covering >= 3 craters
    // with one component cannot beat the best pair, so enumerating pairs is
    // enough.
    //
    // Compare the two candidates' actual cell counts -- for shape (b) we do
    // one extra BFS from the chosen m to recover the in-tree paths to the
    // best port of crater i and crater j, then union with the ocean path --
    // mark those cells with '*', and print the grid. Total time is
    // O(K^2 * N * M), trivially within limits.

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    const int INF = INT_MAX;

    auto in_bounds = [&](int r, int c) {
        return r >= 0 && r < N && c >= 0 && c < M;
    };

    vector<vector<int>> crater_id(N, vector<int>(M, -1));
    int K = 0;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            if(grid[i][j] != 'X' || crater_id[i][j] != -1) {
                continue;
            }
            queue<pair<int, int>> q;
            q.push({i, j});
            crater_id[i][j] = K;
            while(!q.empty()) {
                auto [r, c] = q.front();
                q.pop();
                for(int d = 0; d < 4; d++) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if(in_bounds(nr, nc) && grid[nr][nc] == 'X' &&
                       crater_id[nr][nc] == -1) {
                        crater_id[nr][nc] = K;
                        q.push({nr, nc});
                    }
                }
            }
            K++;
        }
    }

    vector<vector<pair<int, int>>> ports(K);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            if(grid[i][j] != '.') {
                continue;
            }
            set<int> seen;
            for(int d = 0; d < 4; d++) {
                int ni = i + dr[d], nj = j + dc[d];
                if(in_bounds(ni, nj) && grid[ni][nj] == 'X') {
                    int cid = crater_id[ni][nj];
                    if(seen.insert(cid).second) {
                        ports[cid].push_back({i, j});
                    }
                }
            }
        }
    }

    auto bfs = [&](const vector<pair<int, int>>& sources, int start_dist) {
        vector<vector<int>> dist(N, vector<int>(M, INF));
        vector<vector<pair<int, int>>> par(
            N, vector<pair<int, int>>(M, {-1, -1})
        );
        queue<pair<int, int>> q;
        for(auto [r, c]: sources) {
            if(dist[r][c] == INF) {
                dist[r][c] = start_dist;
                q.push({r, c});
            }
        }
        while(!q.empty()) {
            auto [r, c] = q.front();
            q.pop();
            for(int d = 0; d < 4; d++) {
                int nr = r + dr[d], nc = c + dc[d];
                if(in_bounds(nr, nc) && grid[nr][nc] == '.' &&
                   dist[nr][nc] == INF) {
                    dist[nr][nc] = dist[r][c] + 1;
                    par[nr][nc] = {r, c};
                    q.push({nr, nc});
                }
            }
        }
        return make_pair(dist, par);
    };

    vector<pair<int, int>> ocean_sources;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            if(grid[i][j] == '.' &&
               (i == 0 || i == N - 1 || j == 0 || j == M - 1)) {
                ocean_sources.push_back({i, j});
            }
        }
    }
    auto [dist_o, par_o] = bfs(ocean_sources, 1);

    vector<vector<vector<int>>> dist_c(K);
    for(int k = 0; k < K; k++) {
        dist_c[k] = bfs(ports[k], 1).first;
    }

    auto best_port = [&](int k, const vector<vector<int>>& d) {
        pair<int, int> best = {-1, -1};
        int bd = INF;
        for(auto [r, c]: ports[k]) {
            if(d[r][c] < bd) {
                bd = d[r][c];
                best = {r, c};
            }
        }
        return best;
    };

    auto trace_path = [&](pair<int, int> start,
                          const vector<vector<pair<int, int>>>& par) {
        set<pair<int, int>> path;
        auto cur = start;
        while(cur.first != -1) {
            path.insert(cur);
            cur = par[cur.first][cur.second];
        }
        return path;
    };

    vector<int> cost(K, INF);
    for(int k = 0; k < K; k++) {
        for(auto [r, c]: ports[k]) {
            cost[k] = min(cost[k], dist_o[r][c]);
        }
    }
    vector<int> order(K);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return cost[a] < cost[b];
    });

    set<pair<int, int>> S_sep;
    int cost_sep = INF;
    if(K >= 2 && cost[order[0]] != INF && cost[order[1]] != INF) {
        auto pa = best_port(order[0], dist_o);
        auto pb = best_port(order[1], dist_o);
        auto sa = trace_path(pa, par_o);
        auto sb = trace_path(pb, par_o);
        S_sep.insert(sa.begin(), sa.end());
        S_sep.insert(sb.begin(), sb.end());
        cost_sep = (int)S_sep.size();
    }

    int min_multi = INF;
    int best_i = -1, best_j = -1;
    pair<int, int> best_m = {-1, -1};
    for(int i = 0; i < K; i++) {
        for(int j = i + 1; j < K; j++) {
            for(int r = 0; r < N; r++) {
                for(int c = 0; c < M; c++) {
                    if(grid[r][c] != '.') {
                        continue;
                    }
                    if(dist_o[r][c] == INF || dist_c[i][r][c] == INF ||
                       dist_c[j][r][c] == INF) {
                        continue;
                    }
                    int v = dist_o[r][c] + dist_c[i][r][c] - 1 +
                            dist_c[j][r][c] - 1;
                    if(v < min_multi) {
                        min_multi = v;
                        best_i = i;
                        best_j = j;
                        best_m = {r, c};
                    }
                }
            }
        }
    }

    set<pair<int, int>> S_multi;
    int cost_multi = INF;
    if(best_i != -1) {
        auto ocean_path = trace_path(best_m, par_o);
        auto [dist_m, par_m] = bfs({best_m}, 0);
        auto pi = best_port(best_i, dist_m);
        auto pj = best_port(best_j, dist_m);
        auto path_i = trace_path(pi, par_m);
        auto path_j = trace_path(pj, par_m);
        S_multi.insert(ocean_path.begin(), ocean_path.end());
        S_multi.insert(path_i.begin(), path_i.end());
        S_multi.insert(path_j.begin(), path_j.end());
        cost_multi = (int)S_multi.size();
    }

    set<pair<int, int>>& chosen = (cost_sep <= cost_multi) ? S_sep : S_multi;
    for(auto [r, c]: chosen) {
        grid[r][c] = '*';
    }
    for(auto& row: grid) {
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
