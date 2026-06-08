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

const int64_t NEG = LLONG_MIN / 4;

int n, m;
int start_r, start_c;
int bonus_pawn, bonus_rook, bonus_knight, bonus_bishop, bonus_queen, bonus_king;
int penalty;
vector<string> grid;
vector<int> base_count;

const int knight_d[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                            {1, -2},  {1, 2},  {2, -1},  {2, 1}};

const int king_d[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                          {0, 1},   {1, -1}, {1, 0},  {1, 1}};

const int rook_d[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
const int bishop_d[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

void read() {
    cin >> n >> m;
    cin >> bonus_pawn >> bonus_rook >> bonus_knight >> bonus_bishop >>
        bonus_queen >> bonus_king >> penalty;
    grid.assign(n, string());
    for(int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    for(int r = 0; r < n; r++) {
        for(int c = 0; c < m; c++) {
            if(grid[r][c] == '@') {
                start_r = r;
                start_c = c;
            }
        }
    }
}

bool in_board(int r, int c) { return r >= 0 && r < n && c >= 0 && c < m; }

bool is_piece(char ch) {
    return ch == 'P' || ch == 'R' || ch == 'K' || ch == 'B' || ch == 'Q' ||
           ch == 'M';
}

bool blocks_ray(char ch) { return ch != '.' && ch != '@'; }

int capture_bonus(char ch) {
    switch(ch) {
        case 'P':
            return bonus_pawn;
        case 'R':
            return bonus_rook;
        case 'K':
            return bonus_knight;
        case 'B':
            return bonus_bishop;
        case 'Q':
            return bonus_queen;
        case 'M':
            return bonus_king;
    }

    return 0;
}

template<typename F>
void for_moves(char type, int r, int c, F fn) {
    bool slide = type == 'R' || type == 'B';
    const int(*dirs)[2] = type == 'K'   ? knight_d
                          : type == 'M' ? king_d
                          : type == 'R' ? rook_d
                                        : bishop_d;
    int k = slide ? 4 : 8;
    for(int i = 0; i < k; i++) {
        int nr = r + dirs[i][0], nc = c + dirs[i][1];
        if(!slide) {
            if(in_board(nr, nc)) {
                fn(nr, nc);
            }

            continue;
        }

        while(in_board(nr, nc) && !fn(nr, nc)) {
            nr += dirs[i][0];
            nc += dirs[i][1];
        }
    }
}

template<typename F>
void for_footprint(const vector<string>& g, char t, int pr, int pc, F fn) {
    auto ray = [&](int dr, int dc) {
        int r = pr + dr, c = pc + dc;
        while(in_board(r, c)) {
            fn(r * m + c);
            if(blocks_ray(g[r][c])) {
                break;
            }

            r += dr;
            c += dc;
        }
    };
    auto spot = [&](int dr, int dc) {
        int r = pr + dr, c = pc + dc;
        if(in_board(r, c)) {
            fn(r * m + c);
        }
    };

    if(t == 'R' || t == 'Q') {
        for(auto& d: rook_d) {
            ray(d[0], d[1]);
        }
    }

    if(t == 'B' || t == 'Q') {
        for(auto& d: bishop_d) {
            ray(d[0], d[1]);
        }
    }

    if(t == 'K') {
        for(auto& d: knight_d) {
            spot(d[0], d[1]);
        }
    } else if(t == 'M') {
        for(auto& d: king_d) {
            spot(d[0], d[1]);
        }
    } else if(t == 'P') {
        spot(-1, -1);
        spot(-1, 1);
    }
}

void compute_base_count() {
    base_count.assign(n * m, 0);
    vector<int> active(n * m, 0);

    auto sweep = [&](int dr, int dc, bool diagonal) {
        int r0 = dr >= 0 ? 0 : n - 1, r1 = dr >= 0 ? n : -1,
            rs = dr >= 0 ? 1 : -1;
        int c0 = dc >= 0 ? 0 : m - 1, c1 = dc >= 0 ? m : -1,
            cs = dc >= 0 ? 1 : -1;
        for(int r = r0; r != r1; r += rs) {
            for(int c = c0; c != c1; c += cs) {
                int pr = r - dr, pc = c - dc;
                int inc;
                if(!in_board(pr, pc)) {
                    inc = 0;
                } else if(blocks_ray(grid[pr][pc])) {
                    char p = grid[pr][pc];
                    inc = diagonal ? (p == 'B' || p == 'Q')
                                   : (p == 'R' || p == 'Q');
                } else {
                    inc = active[pr * m + pc];
                }

                active[r * m + c] = inc;
                if(inc) {
                    base_count[r * m + c]++;
                }
            }
        }
    };

    sweep(0, 1, false);
    sweep(0, -1, false);
    sweep(1, 0, false);
    sweep(-1, 0, false);
    sweep(1, 1, true);
    sweep(1, -1, true);
    sweep(-1, 1, true);
    sweep(-1, -1, true);

    for(int r = 0; r < n; r++) {
        for(int c = 0; c < m; c++) {
            char ch = grid[r][c];
            if(is_piece(ch) && ch != 'R' && ch != 'B' && ch != 'Q') {
                for_footprint(grid, ch, r, c, [&](int idx) {
                    base_count[idx]++;
                });
            }
        }
    }
}

void uncount_piece(vector<string>& g, vector<int>& cnt, int pr, int pc) {
    char t = g[pr][pc];
    for_footprint(g, t, pr, pc, [&](int idx) { cnt[idx]--; });
    g[pr][pc] = '.';
}

pair<int64_t, int64_t> closure(
    char type, vector<string> g, vector<int> cnt,
    const vector<pair<int, int>>& seeds
) {
    int64_t free_sum = 0;
    vector<char> reach(n * m, 0), sched(n * m, 0), risky_mark(n * m, 0);
    vector<int> work, capq, risky_cells;

    auto can_pull = [&](int x) {
        bool found = false;
        for_moves(type, x / m, x % m, [&](int nr, int nc) {
            if(reach[nr * m + nc]) {
                found = true;
                return true;
            }

            return g[nr][nc] == '#' || is_piece(g[nr][nc]);
        });
        return found;
    };

    auto on_zero = [&](int x) {
        if(g[x / m][x % m] == '#' || reach[x]) {
            return;
        }

        if(is_piece(g[x / m][x % m])) {
            if(!sched[x] && can_pull(x)) {
                sched[x] = 1;
                capq.push_back(x);
            }
        } else if(can_pull(x)) {
            reach[x] = 1;
            work.push_back(x);
        }
    };

    auto take = [&](int pr, int pc) {
        char t = g[pr][pc];
        int idx = pr * m + pc;
        free_sum += capture_bonus(t);
        reach[idx] = 1;
        g[pr][pc] = '.';
        work.push_back(idx);
        for_footprint(g, t, pr, pc, [&](int x) {
            if(--cnt[x] == 0) {
                on_zero(x);
            }
        });
    };

    auto scan = [&](int u) {
        for_moves(type, u / m, u % m, [&](int nr, int nc) {
            int idx = nr * m + nc;
            char ch = g[nr][nc];
            if(ch == '#') {
                return true;
            }

            if(is_piece(ch)) {
                if(cnt[idx] == 0) {
                    if(!sched[idx]) {
                        sched[idx] = 1;
                        capq.push_back(idx);
                    }
                } else if(!risky_mark[idx]) {
                    risky_mark[idx] = 1;
                    risky_cells.push_back(idx);
                }

                return true;
            }

            if(cnt[idx] == 0) {
                if(!reach[idx]) {
                    reach[idx] = 1;
                    work.push_back(idx);
                }

                return true;
            }

            return false;
        });
    };

    for(auto [sr, sc]: seeds) {
        if(!reach[sr * m + sc]) {
            reach[sr * m + sc] = 1;
            work.push_back(sr * m + sc);
        }
    }

    while(!work.empty() || !capq.empty()) {
        while(!work.empty()) {
            int u = work.back();
            work.pop_back();
            scan(u);
        }

        while(!capq.empty()) {
            int p = capq.back();
            capq.pop_back();
            take(p / m, p % m);
        }
    }

    int64_t best_risky = NEG;
    for(int idx: risky_cells) {
        char ch = g[idx / m][idx % m];
        if(is_piece(ch)) {
            best_risky = max(best_risky, (int64_t)capture_bonus(ch) - penalty);
        }
    }

    return {free_sum, best_risky};
}

int64_t closure_value(
    char type, const vector<string>& g, const vector<int>& cnt,
    const vector<pair<int, int>>& seeds
) {
    auto [f, rk] = closure(type, g, cnt, seeds);
    return f + (rk > NEG ? max((int64_t)0, rk) : 0);
}

void flood_component(char type, int sr, int sc, vector<char>& seen) {
    vector<pair<int, int>> st = {{sr, sc}};
    seen[sr * m + sc] = 1;
    while(!st.empty()) {
        auto [r, c] = st.back();
        st.pop_back();
        for_moves(type, r, c, [&](int nr, int nc) {
            int idx = nr * m + nc;
            char ch = grid[nr][nc];
            if(ch == '#' || is_piece(ch)) {
                return true;
            }

            if(base_count[idx] == 0) {
                if(!seen[idx]) {
                    seen[idx] = 1;
                    st.push_back({nr, nc});
                }

                return true;
            }

            return false;
        });
    }
}

int64_t eval_reversible(char type) {
    if(base_count[start_r * m + start_c] == 0) {
        return closure_value(type, grid, base_count, {{start_r, start_c}});
    }

    int64_t best = 0;
    vector<pair<int, int>> empties, capfirst;

    for_moves(type, start_r, start_c, [&](int nr, int nc) {
        int idx = nr * m + nc;
        char ch = grid[nr][nc];
        if(ch == '#') {
            return true;
        }

        if(is_piece(ch)) {
            if(base_count[idx] == 0) {
                capfirst.push_back({nr, nc});
            } else {
                best = max(best, (int64_t)capture_bonus(ch) - penalty);
            }

            return true;
        }

        if(base_count[idx] == 0) {
            empties.push_back({nr, nc});
        }

        return false;
    });

    vector<char> seen(n * m, 0);
    for(auto [r, c]: empties) {
        if(!seen[r * m + c]) {
            flood_component(type, r, c, seen);
            best = max(best, closure_value(type, grid, base_count, {{r, c}}));
        }
    }

    for(auto [pr, pc]: capfirst) {
        vector<string> g2 = grid;
        vector<int> cnt2 = base_count;
        int b = capture_bonus(grid[pr][pc]);
        uncount_piece(g2, cnt2, pr, pc);
        auto [f, rk] = closure(type, g2, cnt2, {{pr, pc}});
        best = max(best, b + f + (rk > NEG ? max((int64_t)0, rk) : 0));
    }

    return best;
}

void solve() {
    // Rules. The board has empty cells, '#' sculptures (immovable walls that
    // block and cannot be captured), colorless pieces (pawn/rook/knight/bishop/
    // queen/king, which never move), and our start '@'. Before starting we pick
    // one piece type - any but a queen - and move it by the usual chess rules.
    // Capturing a colorless piece deletes it from the board and adds its bonus.
    // If after a move our cell is attacked by any surviving colorless piece the
    // training ends and we pay `penalty`; otherwise we may keep going, and we
    // may leave for free at any time. The colorless pawns face the decreasing
    // row direction, so they attack diagonally upward while our own pawn would
    // move downward. We want the largest reachable score.
    //
    // So a run is: collect a set of "free" captures while every landing cell is
    // safe (unattacked), then optionally make one last move onto an attacked
    // cell that captures a piece, worth bonus - penalty, after which the run
    // ends. The hard part is that capturing removes a piece, which removes its
    // attacks and so changes which cells are safe as we go.
    //
    // We only evaluate the reversible pieces (rook, knight, bishop, king). The
    // pawn is left out: it walks a single strictly-downward path and can never
    // collect more than a king touring the same cells, so it is never the lone
    // best choice.
    //
    // Let base_count[cell] be how many colorless pieces attack it. We find it
    // once: eight directional line sweeps handle the sliders (each sweep
    // carries along a line whether the nearest blocker behind the current cell
    // is a matching rook/bishop/queen, so the cell just past such a blocker is
    // attacked), and the fixed footprints of the knights, kings and pawns are
    // stamped directly. A cell is safe exactly when its count is zero.
    //
    // Key lemma: a piece we can safely stand on has count zero, so it is not
    // the first blocker on any slider's ray - otherwise that slider would
    // attack it. Hence capturing a safe piece never lets a slider see further
    // (opens no new ray); it only deletes that piece's own attacks. Counts
    // therefore only shrink during free captures, safe cells stay safe, and we
    // can keep counts exact by decrementing along just the captured piece's
    // footprint, with no resweep - that is what makes the whole thing cheap.
    //
    // closure() does the work for one starting region. A reversible token can
    // roam any connected region of safe cells and always backtrack, so we flood
    // the safe cells from the seeds. Whenever a reached cell is one move from a
    // safe colorless piece we capture it: bank its bonus, mark its now-empty
    // cell reached, and decrement its footprint. A decrement that drops some
    // cell's count to zero makes that cell newly safe; if it is now adjacent to
    // the reached region we pull it in - an empty cell rejoins the flood, a
    // piece becomes capturable - and this cascades until nothing changes. A
    // piece that stays reachable only onto an attacked cell is remembered as a
    // candidate for the single risky last move worth bonus - penalty.
    //
    // The start needs care. If '@' is safe it is an ordinary region cell and
    // one closure seeded there is enough. If '@' is attacked we can stand on it
    // only at move zero - once we leave we can never come back, so different
    // first moves may strand us in different, disconnected safe regions. We
    // therefore try each first move out of '@' and keep the best: moves onto
    // empty safe cells are deduplicated by safe component (one closure per
    // component), a move that captures a safe piece runs a closure with that
    // piece pre-removed, and a move onto an attacked piece is itself a risky
    // final worth its bonus minus penalty.
    //
    // The answer is the best of the four piece types, and at least zero since
    // we may always leave at once. Each closure is O(n*m*(n+m)) in the worst
    // case.

    compute_base_count();

    int64_t ans = 0;
    for(char type: {'R', 'K', 'B', 'M'}) {
        ans = max(ans, eval_reversible(type));
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
