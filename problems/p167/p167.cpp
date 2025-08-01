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

const int inf = (int)1e9 + 42;

int n, m, k;
vector<vector<int>> a;

void read() {
    cin >> n >> m >> k;
    a.resize(n, vector<int>(m));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }
}

vector<vector<int>> psum;

int get_sum(int x1, int y1, int x2, int y2) {
    int res = psum[x2][y2];
    if(x1 > 0) {
        res -= psum[x1 - 1][y2];
    }
    if(y1 > 0) {
        res -= psum[x2][y1 - 1];
    }
    if(x1 > 0 && y1 > 0) {
        res += psum[x1 - 1][y1 - 1];
    }
    return res;
}

vector<vector<vector<vector<vector<int>>>>> dp;

int rec(int row, int l, int r, int cnt, int state) {
    if(row == n) {
        return (cnt == 0) ? 0 : -inf;
    }
    if(cnt == 0) {
        return 0;
    }

    int& memo = dp[row][l][r][cnt][state];
    if(memo != -1) {
        return memo;
    }

    bool can_move_l = (state & 1) == 0;
    bool can_move_r = (state & 2) == 0;

    memo = -inf;

    for(int new_l = 0; new_l <= m; new_l++) {
        for(int new_r = new_l; new_r < m; new_r++) {
            int overlap = min(r, new_r) - max(l, new_l) + 1;
            int new_cnt = cnt - (new_r - new_l + 1);
            if(new_cnt < 0) {
                continue;
            }

            if(overlap <= 0) {
                continue;
            }

            if(!can_move_l && new_l < l) {
                continue;
            }

            if(!can_move_r && new_r > r) {
                continue;
            }

            bool we_shrunk_left = new_l > l;
            bool we_shrunk_right = new_r < r;
            int new_state =
                state | (we_shrunk_left ? 1 : 0) | (we_shrunk_right ? 2 : 0);
            int sum = get_sum(row, new_l, row, new_r);
            memo =
                max(memo, sum + rec(row + 1, new_l, new_r, new_cnt, new_state));
        }
    }

    return memo;
}

void solve() {
    if(k == 0) {
        cout << "Oil : 0\n";
        return;
    }

    psum.assign(n, vector<int>(m, 0));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            psum[i][j] = a[i][j];
            if(i > 0) {
                psum[i][j] += psum[i - 1][j];
            }
            if(j > 0) {
                psum[i][j] += psum[i][j - 1];
            }
            if(i > 0 && j > 0) {
                psum[i][j] -= psum[i - 1][j - 1];
            }
        }
    }

    dp.assign(
        n, vector<vector<vector<vector<int>>>>(
               m, vector<vector<vector<int>>>(
                      m, vector<vector<int>>(k + 1, vector<int>(4, -1))
                  )
           )
    );

    int ans = -inf;
    int start_row = -1, start_l = -1, start_r = -1;
    for(int row = 0; row < n; row++) {
        for(int l = 0; l < m; l++) {
            for(int r = l; r < m; r++) {
                int cnt = k - (r - l + 1);
                if(cnt >= 0) {
                    int sum = get_sum(row, l, row, r);
                    int res = sum + rec(row + 1, l, r, cnt, 0);
                    if(res > ans) {
                        ans = res;
                        start_l = l;
                        start_r = r;
                        start_row = row;
                    }
                }
            }
        }
    }

    cout << "Oil : " << ans << '\n';

    // Reconstruct the path using the dp table.
    vector<pair<int, int>> cells;
    for(int i = start_l; i <= start_r; i++) {
        cells.emplace_back(start_row, i);
    }

    k -= (start_r - start_l + 1);
    ans -= get_sum(start_row, start_l, start_row, start_r);
    int state = 0;
    start_row++;
    while(k > 0) {
        int best_next_l = -1, best_next_r = -1, best_next_state = -1;
        bool can_move_l = (state & 1) == 0;
        bool can_move_r = (state & 2) == 0;
        for(int new_l = 0; new_l < m; new_l++) {
            for(int new_r = new_l; new_r < m; new_r++) {
                int overlap = min(start_r, new_r) - max(start_l, new_l) + 1;
                int new_cnt = k - (new_r - new_l + 1);
                if(new_cnt < 0) {
                    continue;
                }
                if(overlap <= 0) {
                    continue;
                }

                if(!can_move_l && new_l < start_l) {
                    continue;
                }

                if(!can_move_r && new_r > start_r) {
                    continue;
                }

                bool we_shrunk_left = new_l > start_l;
                bool we_shrunk_right = new_r < start_r;
                int new_state = state | (we_shrunk_left ? 1 : 0) |
                                (we_shrunk_right ? 2 : 0);
                int sum = get_sum(start_row, new_l, start_row, new_r);
                int next_res =
                    sum + rec(start_row + 1, new_l, new_r, new_cnt, new_state);
                if(next_res == ans) {
                    best_next_l = new_l;
                    best_next_r = new_r;
                    best_next_state = new_state;
                }
            }
        }

        assert(best_next_l != -1 && best_next_r != -1 && best_next_state != -1);

        for(int i = best_next_l; i <= best_next_r; i++) {
            cells.emplace_back(start_row, i);
        }

        ans -= get_sum(start_row, best_next_l, start_row, best_next_r);
        k -= (best_next_r - best_next_l + 1);
        start_row++;
        start_l = best_next_l;
        start_r = best_next_r;
        state = best_next_state;
    }

    sort(cells.begin(), cells.end());
    for(int i = 0; i < (int)cells.size(); i++) {
        cout << cells[i].first + 1 << ' ' << cells[i].second + 1 << '\n';
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
