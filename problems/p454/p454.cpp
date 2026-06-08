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
vector<vector<string>> g;

void read() {
    cin >> n >> m;
    g.assign(n, vector<string>(m));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cin >> g[i][j];
        }
    }
}

void solve() {
    // The grid is tiny (n, m <= 6), so this is brute-force backtracking over
    // white cells with sudoku-style pruning. For every black cell that carries
    // a clue we precompute the horizontal and vertical "runs" it controls: the
    // clue value, the length of the run, and a per-run id so each white cell
    // knows which two runs it participates in.
    //
    // The strong pruning step is to enumerate, for every run, the full list of
    // valid digit subsets ahead of time: every subset of {1..9} of size equal
    // to the run length whose digits sum to the clue. There are very few of
    // these per run, and they encode all the real constraints of the puzzle.
    //
    // During search we keep, for every run, a bitmask of digits already placed
    // in it. The set of digits still allowed in run r is then the union of all
    // valid subsets that are supersets of the current used mask, minus that
    // mask itself. For a white cell the candidate digits are the intersection
    // of its two runs' allowed sets, which is a much tighter filter than any
    // min/max-sum bound.
    //
    // On top of that we use MRV: at each step we pick the unfilled white cell
    // with the smallest candidate set, which is the standard sudoku
    // "most-constrained variable" heuristic and collapses the search tree on
    // the harder tests.

    vector<vector<int>> h_run(n, vector<int>(m, -1));
    vector<vector<int>> v_run(n, vector<int>(m, -1));
    vector<int> h_clue, v_clue;
    vector<int> h_count, v_count;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            const string& s = g[i][j];
            if(s[0] == '.' || s == "XXXXX") {
                continue;
            }

            string aa = s.substr(0, 2);
            string bb = s.substr(3, 2);

            if(bb != "XX") {
                int rid = (int)h_clue.size();
                h_clue.push_back(stoi(bb));
                int cnt = 0;
                for(int k = j + 1; k < m && g[i][k][0] == '.'; k++) {
                    h_run[i][k] = rid;
                    cnt++;
                }
                h_count.push_back(cnt);
            }

            if(aa != "XX") {
                int rid = (int)v_clue.size();
                v_clue.push_back(stoi(aa));
                int cnt = 0;
                for(int k = i + 1; k < n && g[k][j][0] == '.'; k++) {
                    v_run[k][j] = rid;
                    cnt++;
                }
                v_count.push_back(cnt);
            }
        }
    }

    vector<vector<vector<int>>> kak(10, vector<vector<int>>(46));
    for(int mask = 0; mask < (1 << 10); mask++) {
        if(mask & 1) {
            continue;
        }
        int cnt = __builtin_popcount(mask);
        int sum = 0;
        for(int x = 1; x <= 9; x++) {
            if(mask & (1 << x)) {
                sum += x;
            }
        }
        if(cnt <= 9 && sum <= 45) {
            kak[cnt][sum].push_back(mask);
        }
    }

    int H = (int)h_clue.size();
    int V = (int)v_clue.size();
    vector<vector<int>> h_subs(H), v_subs(V);
    for(int r = 0; r < H; r++) {
        h_subs[r] = kak[h_count[r]][h_clue[r]];
    }
    for(int r = 0; r < V; r++) {
        v_subs[r] = kak[v_count[r]][v_clue[r]];
    }

    vector<pair<int, int>> cells;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(g[i][j][0] == '.') {
                cells.push_back({i, j});
            }
        }
    }

    int total = (int)cells.size();
    vector<int> h_used(H, 0), v_used(V, 0);
    vector<bool> filled(total, false);
    vector<vector<int>> ans(n, vector<int>(m, 0));

    auto allowed_for = [](const vector<int>& subs, int used) {
        int allowed = 0;
        for(int t: subs) {
            if((t & used) == used) {
                allowed |= t;
            }
        }
        return allowed & ~used;
    };

    function<bool(int)> bt = [&](int placed) -> bool {
        if(placed == total) {
            return true;
        }

        vector<int> h_allowed(H), v_allowed(V);
        for(int r = 0; r < H; r++) {
            h_allowed[r] = allowed_for(h_subs[r], h_used[r]);
        }
        for(int r = 0; r < V; r++) {
            v_allowed[r] = allowed_for(v_subs[r], v_used[r]);
        }

        int best = -1, best_mask = 0, best_cnt = 100;
        for(int idx = 0; idx < total; idx++) {
            if(filled[idx]) {
                continue;
            }
            auto [i, j] = cells[idx];
            int cand = h_allowed[h_run[i][j]] & v_allowed[v_run[i][j]];
            int cnt = __builtin_popcount(cand);
            if(cnt < best_cnt) {
                best_cnt = cnt;
                best = idx;
                best_mask = cand;
                if(cnt <= 1) {
                    break;
                }
            }
        }

        if(best_cnt == 0) {
            return false;
        }

        auto [i, j] = cells[best];
        int hr = h_run[i][j], vr = v_run[i][j];
        filled[best] = true;

        for(int mask = best_mask; mask; mask &= mask - 1) {
            int bit = mask & -mask;
            int d = __builtin_ctz(bit);
            h_used[hr] |= bit;
            v_used[vr] |= bit;
            ans[i][j] = d;

            if(bt(placed + 1)) {
                return true;
            }

            h_used[hr] ^= bit;
            v_used[vr] ^= bit;
        }

        filled[best] = false;
        return false;
    };

    bt(0);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(j) {
                cout << ' ';
            }
            if(g[i][j][0] == '.') {
                cout << ans[i][j];
            } else {
                cout << '_';
            }
        }
        cout << '\n';
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
