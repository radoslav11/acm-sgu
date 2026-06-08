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

int n, src_a, src_b;
vector<int> adj_mask;
vector<int64_t> popul;

void read() {
    cin >> n >> src_a >> src_b;
    src_a--;
    src_b--;

    vector<string> grid(n);
    for(auto& row: grid) {
        cin >> row;
    }

    adj_mask.assign(n, 0);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(grid[i][j] == '1') {
                adj_mask[i] |= (1 << j);
            }
        }
    }

    popul.assign(n, 0);
    cin >> popul;
}

int full_mask;
vector<int64_t> total_pop;
vector<int> neigh_mask;
vector<int> pow3_subset;
vector<int64_t> dp_a;
vector<int64_t> dp_b;
const int64_t SENT_A = INT64_MIN;
const int64_t SENT_B = INT64_MAX;

int64_t solve_state(int A, int B, int idx, int who) {
    int64_t& slot = (who == 0 ? dp_a[idx] : dp_b[idx]);
    int64_t sent = (who == 0 ? SENT_A : SENT_B);
    if(slot != sent) {
        return slot;
    }

    int free = full_mask ^ A ^ B;
    int64_t terminal = total_pop[A] - total_pop[B];
    if(free == 0) {
        return slot = terminal;
    }

    bool found_self = false;
    bool found_other = false;
    int64_t best = sent;
    for(int c = free; c; c = (c - 1) & free) {
        int64_t pa = total_pop[neigh_mask[c] & A];
        int64_t pb = total_pop[neigh_mask[c] & B];
        int64_t pc = total_pop[c];
        bool a_valid = pa > pc + pb;
        bool b_valid = pb > pc + pa;
        if(who == 0) {
            if(a_valid) {
                found_self = true;
                int64_t v = solve_state(A | c, B, idx + pow3_subset[c], 1);
                if(v > best) {
                    best = v;
                }
            }
            if(b_valid) {
                found_other = true;
            }
        } else {
            if(b_valid) {
                found_self = true;
                int64_t v = solve_state(A, B | c, idx + 2 * pow3_subset[c], 0);
                if(v < best) {
                    best = v;
                }
            }
            if(a_valid) {
                found_other = true;
            }
        }
    }

    if(found_self) {
        return slot = best;
    }
    if(!found_other) {
        return slot = terminal;
    }
    return slot = solve_state(A, B, idx, who ^ 1);
}

void solve() {
    // State of the game: (A, B, who), the disjoint sets of cities owned by
    // the two countries plus a flag for whose turn it is. The remaining
    // cities are still independent. Each city is in one of three modes
    // (free / A / B), so we encode (A, B) as a base-3 integer and keep a
    // separate memo per `who`.
    //
    // The conquest rule is at the SET level: country A can take any subset
    // C of the free cities iff popul(neigh(C) & A) > popul(C) +
    // popul(neigh(C) & B). In words, A's population around C must strictly
    // outweigh B's population around C plus the population of C itself, and
    // B's rule is symmetric. This is NOT a per-city check -- a subset of a
    // valid C can fail the inequality, so we have to enumerate all 2^|free|
    // submasks of free at every state instead of pre-collapsing to a single
    // valid mask.
    //
    // The turn alternates A, B, A, B, ... and a player is forced to act if
    // any of their moves are valid; if not, they pass to the opponent. The
    // game ends when neither player has a valid move (or there are no free
    // cities left), at which point the score is popul(A) - popul(B).
    //
    // We precompute neigh_mask[C] = (set of cities adjacent to C) \ C,
    // total_pop[mask] for the population sums, and pow3_subset[mask] =
    // sum_{i in mask} 3^i so the ternary index updates in O(1) when a whole
    // submask is added to A or B.
    //
    // The total work is sum over (A, B) of 2^|free| iterations of the
    // submask loop, doubled for the two `who` values -- bounded by O(4^n),
    // which is fine for n <= 13.

    full_mask = (1 << n) - 1;
    int total = 1 << n;

    total_pop.assign(total, 0);
    for(int m = 1; m < total; m++) {
        int lo = __builtin_ctz(m);
        total_pop[m] = total_pop[m ^ (1 << lo)] + popul[lo];
    }

    neigh_mask.assign(total, 0);
    for(int m = 0; m < total; m++) {
        int t = m;
        int nb = 0;
        while(t) {
            int i = __builtin_ctz(t);
            t &= t - 1;
            nb |= adj_mask[i];
        }
        neigh_mask[m] = nb & ~m;
    }

    vector<int> pow3(n + 1, 1);
    for(int i = 1; i <= n; i++) {
        pow3[i] = pow3[i - 1] * 3;
    }

    pow3_subset.assign(total, 0);
    for(int m = 1; m < total; m++) {
        int lo = __builtin_ctz(m);
        pow3_subset[m] = pow3[lo] + pow3_subset[m ^ (1 << lo)];
    }

    dp_a.assign(pow3[n], SENT_A);
    dp_b.assign(pow3[n], SENT_B);

    int A0 = 1 << src_a;
    int B0 = 1 << src_b;
    int idx0 = pow3[src_a] + 2 * pow3[src_b];
    cout << solve_state(A0, B0, idx0, 0) << "\n";
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
