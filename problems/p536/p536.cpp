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

int n, m;
vector<string> tbl;
vector<pair<int, int>> pieces;
vector<char> ptype;
int kpos;

void read() {
    cin >> n >> m;
    tbl.resize(n);
    cin >> tbl;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(tbl[i][j] == '*') {
                kpos = i * m + j;
            } else if(tbl[i][j] == 'K' || tbl[i][j] == 'B' || tbl[i][j] == 'R') {
                pieces.emplace_back(i, j);
                ptype.push_back(tbl[i][j]);
            }
        }
    }
}

void solve() {
    // We can solve this with a fairly simple BFS by extending the state. As
    // black pieces don't move, the state is fully defined by the position of
    // the king, and the state of figures that have been taken (which can be
    // kept as a 3-bit mask). Initially we will just cover the cells that are
    // blocked.

    int k = pieces.size();
    if(k == 0) {
        cout << 0 << "\n";
        return;
    }

    vector<vector<set<int>>> attack(n, vector<set<int>>(m));
    for(int idx = 0; idx < k; idx++) {
        auto [ci, cj] = pieces[idx];
        char type = ptype[idx];
        if(type == 'K') {
            for(int di = -2; di <= 2; di++) {
                for(int dj = -2; dj <= 2; dj++) {
                    if(abs(di) + abs(dj) == 3) {
                        int ni = ci + di, nj = cj + dj;
                        if(ni >= 0 && ni < n && nj >= 0 && nj < m) {
                            attack[ni][nj].insert(idx);
                        }
                    }
                }
            }
        } else if(type == 'B') {
            for(int d = 0; d < 4; d++) {
                int di = (d == 0 || d == 1) ? 1 : -1;
                int dj = (d == 0 || d == 2) ? 1 : -1;
                for(int step = 1;; step++) {
                    int ni = ci + di * step, nj = cj + dj * step;
                    if(ni < 0 || ni >= n || nj < 0 || nj >= m) {
                        break;
                    }
                    attack[ni][nj].insert(idx);
                    if(tbl[ni][nj] != '.') {
                        break;
                    }
                }
            }
        } else if(type == 'R') {
            for(int d = 0; d < 4; d++) {
                int di = (d == 0) ? 1 : ((d == 1) ? -1 : 0);
                int dj = (d == 2) ? 1 : ((d == 3) ? -1 : 0);
                for(int step = 1;; step++) {
                    int ni = ci + di * step, nj = cj + dj * step;
                    if(ni < 0 || ni >= n || nj < 0 || nj >= m) {
                        break;
                    }
                    attack[ni][nj].insert(idx);
                    if(tbl[ni][nj] != '.') {
                        break;
                    }
                }
            }
        }
    }

    queue<array<int, 3>> q;
    vector<vector<vector<int>>> dist(
        n, vector<vector<int>>(m, vector<int>(1 << k, -1))
    );
    int start_mask = 0;
    dist[kpos / m][kpos % m][start_mask] = 0;
    q.push({kpos / m, kpos % m, start_mask});

    int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    while(!q.empty()) {
        auto [ci, cj, mask] = q.front();
        q.pop();

        if(mask == (1 << k) - 1) {
            cout << dist[ci][cj][mask] << "\n";
            return;
        }

        for(int d = 0; d < 8; d++) {
            int ni = ci + dr[d];
            int nj = cj + dc[d];
            if(ni < 0 || ni >= n || nj < 0 || nj >= m) {
                continue;
            }

            bool attacked = false;
            for(int idx: attack[ni][nj]) {
                if(!(mask & (1 << idx))) {
                    attacked = true;
                    break;
                }
            }
            if(attacked) {
                continue;
            }

            int nmask = mask;
            char cell = tbl[ni][nj];
            if(cell == 'K' || cell == 'B' || cell == 'R') {
                for(int idx = 0; idx < k; idx++) {
                    if(pieces[idx] == make_pair(ni, nj)) {
                        nmask |= (1 << idx);
                        break;
                    }
                }
            }

            if(dist[ni][nj][nmask] == -1) {
                dist[ni][nj][nmask] = dist[ci][cj][mask] + 1;
                q.push({ni, nj, nmask});
            }
        }
    }

    cout << -1 << "\n";
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
