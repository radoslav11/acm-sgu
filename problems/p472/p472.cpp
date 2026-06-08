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

const int dr[4] = {-1, 1, 0, 0};
const int dc[4] = {0, 0, -1, 1};
const int opp[4] = {1, 0, 3, 2};
const char low[4] = {'u', 'd', 'l', 'r'};
const char upp[4] = {'U', 'D', 'L', 'R'};

int rows, cols;
vector<string> grid;
int start_keeper, start_box, target;

void read() {
    vector<string> raw;
    string line;
    while(getline(cin, line)) {
        if(!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        raw.push_back(line);
    }

    rows = raw.size();
    cols = 0;
    for(auto& s: raw) {
        cols = max(cols, (int)s.size());
    }

    grid.assign(rows, string(cols, '#'));
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < (int)raw[r].size(); c++) {
            grid[r][c] = raw[r][c];
        }
    }

    start_keeper = start_box = target = -1;
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(grid[r][c] == '@') {
                start_keeper = r * cols + c;
                grid[r][c] = ' ';
            } else if(grid[r][c] == '$') {
                start_box = r * cols + c;
                grid[r][c] = ' ';
            } else if(grid[r][c] == '.') {
                target = r * cols + c;
                grid[r][c] = ' ';
            }
        }
    }
}

void solve() {
    // We search over states (box cell, side), where "side" records which of
    // the four cells neighbouring the box the keeper stands on, i.e. the cell
    // the box was last pushed from. A push in direction d is possible only when
    // the keeper can walk to the cell directly behind the box (opposite to d)
    // and the cell in front of the box is free; the push then advances the box
    // one cell and moves the keeper onto the box's old cell.
    //
    // The objective is lexicographic: first minimise the number of pushes, then
    // the total number of keeper moves. This is a shortest path with cost
    // (pushes, moves) compared lexicographically, so we run Dijkstra over the
    // (box, side) states. Walking the keeper does between two consecutive
    // pushes costs no push and contributes its length in moves; for a popped
    // state we recover those lengths with a single grid BFS from the keeper's
    // cell that treats the box as a wall, yielding the distance to every
    // reachable push position at once.
    //
    // Pushing in direction d from box cell B sends the box to B + d and the
    // keeper to B, so the successor is (B + d, opposite(d)) with one extra push
    // and (walk distance to B - d) + 1 extra moves. The keeper's starting cell
    // is handled by one initial BFS that seeds every possible first push. The
    // first popped state whose box sits on the target cell is optimal.
    //
    // To print a solution we follow the predecessor chain and, for each
    // recorded push, recompute the keeper's walking path with a BFS that tracks
    // parents, emitting lower case letters for plain moves and the upper case
    // letter for the push itself. If no state with the box on the target is
    // ever popped the puzzle is unsolvable.

    if(start_box == target) {
        cout << "\n";
        return;
    }

    int cells = rows * cols;
    int states = 4 * cells;

    vector<int> walk_dist(cells), walk_seen(cells, 0);
    vector<int> bfs_queue(cells);
    int walk_timer = 0;

    auto bfs_dist = [&](int src, int obstacle) {
        walk_timer++;
        int orow = obstacle / cols, ocol = obstacle % cols;
        int need = 0;
        for(int k = 0; k < 4; k++) {
            int nr = orow + dr[k], nc = ocol + dc[k];
            if(nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
               grid[nr][nc] != '#') {
                need++;
            }
        }

        int found = 0, head = 0, tail = 0;
        walk_seen[src] = walk_timer;
        walk_dist[src] = 0;
        bfs_queue[tail++] = src;
        if(abs(src / cols - orow) + abs(src % cols - ocol) == 1) {
            found++;
        }

        while(head < tail && found < need) {
            int cur = bfs_queue[head++];
            int cr = cur / cols, cc = cur % cols, cd = walk_dist[cur];
            for(int k = 0; k < 4; k++) {
                int nr = cr + dr[k], nc = cc + dc[k];
                if(nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                    continue;
                }
                int ncell = nr * cols + nc;
                if(grid[nr][nc] == '#' || ncell == obstacle) {
                    continue;
                }
                if(walk_seen[ncell] == walk_timer) {
                    continue;
                }

                walk_seen[ncell] = walk_timer;
                walk_dist[ncell] = cd + 1;
                bfs_queue[tail++] = ncell;
                if(abs(nr - orow) + abs(nc - ocol) == 1) {
                    found++;
                }
            }
        }
    };

    const int INF = INT_MAX;
    vector<int> dist_push(states, INF), dist_move(states, INF);
    vector<int> par(states, -2), par_dir(states, -1);

    using node = tuple<int, int, int>;
    priority_queue<node, vector<node>, greater<node>> pq;

    auto relax = [&](int st, int np, int nm, int from, int dir) {
        if(np < dist_push[st] || (np == dist_push[st] && nm < dist_move[st])) {
            dist_push[st] = np;
            dist_move[st] = nm;
            par[st] = from;
            par_dir[st] = dir;
            pq.push({np, nm, st});
        }
    };

    auto try_pushes = [&](int box, int keeper, int from, int base_push,
                          int base_move) {
        bfs_dist(keeper, box);
        int brow = box / cols, bcol = box % cols;
        for(int i = 0; i < 4; i++) {
            int wr = brow - dr[i], wc = bcol - dc[i];
            int fr = brow + dr[i], fc = bcol + dc[i];
            if(wr < 0 || wr >= rows || wc < 0 || wc >= cols) {
                continue;
            }
            if(fr < 0 || fr >= rows || fc < 0 || fc >= cols) {
                continue;
            }
            if(grid[wr][wc] == '#' || grid[fr][fc] == '#') {
                continue;
            }

            int behind = wr * cols + wc;
            if(walk_seen[behind] != walk_timer) {
                continue;
            }

            int new_box = fr * cols + fc;
            int new_state = new_box * 4 + opp[i];
            relax(
                new_state, base_push + 1, base_move + walk_dist[behind] + 1,
                from, i
            );
        }
    };

    try_pushes(start_box, start_keeper, -1, 0, 0);

    int goal_state = -1;
    while(!pq.empty()) {
        auto [pp, mm, st] = pq.top();
        pq.pop();
        if(pp != dist_push[st] || mm != dist_move[st]) {
            continue;
        }

        int box = st / 4, side = st % 4;
        if(box == target) {
            goal_state = st;
            break;
        }

        int keeper = (box / cols + dr[side]) * cols + (box % cols + dc[side]);
        try_pushes(box, keeper, st, pp, mm);
    }

    if(goal_state == -1) {
        cout << "Impossible.\n";
        return;
    }

    vector<int> chain;
    for(int cur = goal_state; cur != -1; cur = par[cur]) {
        chain.push_back(cur);
    }
    reverse(chain.begin(), chain.end());

    vector<int> path_seen(cells, 0), path_par(cells), path_par_dir(cells);
    int path_timer = 0;

    auto bfs_path = [&](int src, int dst, int obstacle) {
        string moves;
        if(src == dst) {
            return moves;
        }

        path_timer++;
        int head = 0, tail = 0;
        path_seen[src] = path_timer;
        path_par[src] = -1;
        bfs_queue[tail++] = src;

        while(head < tail) {
            int cur = bfs_queue[head++];
            if(cur == dst) {
                break;
            }

            int cr = cur / cols, cc = cur % cols;
            for(int k = 0; k < 4; k++) {
                int nr = cr + dr[k], nc = cc + dc[k];
                if(nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                    continue;
                }
                int ncell = nr * cols + nc;
                if(grid[nr][nc] == '#' || ncell == obstacle) {
                    continue;
                }
                if(path_seen[ncell] == path_timer) {
                    continue;
                }

                path_seen[ncell] = path_timer;
                path_par[ncell] = cur;
                path_par_dir[ncell] = k;
                bfs_queue[tail++] = ncell;
            }
        }

        for(int cur = dst; cur != src; cur = path_par[cur]) {
            moves += low[path_par_dir[cur]];
        }
        reverse(moves.begin(), moves.end());
        return moves;
    };

    string result;
    for(int st: chain) {
        int dir = par_dir[st];
        int prev = par[st];

        int prev_box, prev_keeper;
        if(prev == -1) {
            prev_box = start_box;
            prev_keeper = start_keeper;
        } else {
            int pb = prev / 4, ps = prev % 4;
            prev_box = pb;
            prev_keeper = (pb / cols + dr[ps]) * cols + (pb % cols + dc[ps]);
        }

        int behind =
            (prev_box / cols - dr[dir]) * cols + (prev_box % cols - dc[dir]);
        result += bfs_path(prev_keeper, behind, prev_box);
        result += upp[dir];
    }

    cout << result << "\n";
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
