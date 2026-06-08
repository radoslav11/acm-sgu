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

int n;
string s1, s2;
array<int, 27> a{}, b{};
vector<int> move_costs;

void read() { cin >> n >> s1 >> s2; }

int push_step(int from, int to) {
    int d = (to - from + n + 1) % (n + 1);
    move_costs.push_back(d);
    return d;
}

int compute_cost() {
    move_costs.clear();
    int total = 0;
    array<int, 27> pos_in_b{}, perm{};
    for(int i = 0; i <= n; i++) {
        pos_in_b[b[i]] = i;
    }

    for(int i = 0; i <= n; i++) {
        perm[i] = pos_in_b[a[i]];
    }

    array<bool, 27> visited{};
    for(int i = 0; i <= n; i++) {
        if(visited[i]) {
            continue;
        }

        visited[i] = true;
        if(perm[i] == i) {
            continue;
        }

        vector<int> cycle = {i};
        for(int x = perm[i]; x != i; x = perm[x]) {
            cycle.push_back(x);
            visited[x] = true;
        }

        int cnt = (int)cycle.size();
        if(i == 0) {
            reverse(cycle.begin() + 1, cycle.end());
            for(int j = 1; j < cnt; j++) {
                total += push_step(cycle[j - 1], cycle[j]);
            }
        } else {
            int start = 0;
            for(int j = 0; j < cnt; j++) {
                int x = cycle[j], y = cycle[(j + 1) % cnt];
                bool zero_outside = x < y && (perm[0] < x || perm[0] > y);
                bool zero_inside_wrap = y < perm[0] && perm[0] < x;
                if(zero_outside || zero_inside_wrap) {
                    start = j;
                }
            }

            rotate(cycle.begin(), cycle.begin() + start, cycle.end());
            reverse(cycle.begin() + 1, cycle.end());
            total += push_step(perm[0], cycle[0]);
            for(int j = 1; j < cnt; j++) {
                total += push_step(cycle[j - 1], cycle[j]);
            }

            total += push_step(cycle.back(), perm[0]);
        }
    }

    return total;
}

void apply_move(int p) {
    array<int, 27> next_a{};
    for(int i = p + 1; i <= n; i++) {
        next_a[i - p] = a[i];
    }
    next_a[n - p + 1] = a[p];
    for(int i = 1; i < p; i++) {
        next_a[n - p + 1 + i] = a[i];
    }
    for(int i = 1; i <= n; i++) {
        a[i] = next_a[i];
    }
}

void solve() {
    // Assume WLOG the output string T starts with S1 (we also try the
    // S2-first ordering and pick the shorter). The first N characters of T
    // are S1. Every window of length N+1 in T must be almost-permutative
    // (one duplicate, one missing), so each new character is forced to be
    // the one that just left the previous window of length N, until the
    // window becomes a full permutation again.
    //
    // Concretely, if the current full-permutation window is W = w_1..w_n,
    // the next "stable" window is produced by choosing a split point
    // p in {1..n-1}, appending w_p, w_1, w_2, ..., w_{p-1} (cost = p new
    // characters), and the new window becomes w_{p+1}..w_n w_p w_1..w_{p-1}.
    //
    // Prepend a virtual symbol 0 at position 0 and treat positions
    // 0..n as a cycle of length n+1. A move with split p is equivalent to
    // swapping the element at position p with the 0 (the cost p is exactly
    // the clockwise distance from 0's current position to p). The goal is
    // to transform the permutation of S1 into some cyclic rotation of the
    // permutation of S2 with minimum total swap distance.
    //
    // perm[i] is the position where the value currently at position i in a
    // needs to land in b. We can show that we should fix cycles independently,
    // as otherwise we will just waste more operations that would merge the
    // cycles, increasing the overall cost. Now let's consider the cycles
    // themselves:
    //
    //   - First the cycle containing 0. Write it in perm-order as
    //     C_0 = (0 -> p_1 -> p_2 -> ... -> p_t -> 0). We resolve it by
    //     walking 0 through positions q_0 = 0, q_1, q_2, ..., q_m, where
    //     each step is a swap with 0. Such a walk has a fixed value-transport
    //     effect: the value originally at q_i ends up at q_{i-1}, and 0 ends
    //     at q_m. We need the value at each q_i to land at perm[q_i], so we
    //     need perm[q_i] = q_{i-1}, i.e., q_i must come AFTER q_{i-1} in the
    //     cycle's perm-order. So we walk C_0 in REVERSE perm-order:
    //
    //         q_0 = 0, q_1 = p_t, q_2 = p_{t-1}, ..., q_t = p_1.
    //
    //     The cost is the sum of clockwise step distances along that reversed
    //     walk, and after this phase 0 sits at p_1 = perm[0]. (If perm[0] = 0
    //     the 0-cycle is trivial and 0 stays at position 0.)
    //
    //   - Then every other cycle. Let C = (c_0 -> c_1 -> ... -> c_{k-1} -> c_0)
    //     be a non-zero cycle, and let z := perm[0] be the position where 0
    //     currently sits (either 0, or the spot left by the 0-cycle phase).
    //     z is not in C (distinct cycles are disjoint), so 0 has to take a
    //     round trip: leave z, traverse C, return to z. Same reverse-walk
    //     reasoning as above: starting at q_0 = z, pick an entry index s and
    //     walk
    //
    //         z -> c_s -> c_{s-1} -> ... -> c_{s+1} -> z   (indices mod k)
    //
    //     The k middle swaps transport the cycle's values one perm-step
    //     forward, and the final swap z <-> last position brings z's value
    //     (which had been displaced by the first swap) back home. So z ends
    //     up holding what it started with, and every value in C lands where
    //     perm sends it.
    //
    //     Let W = sum_{j} d(c_j, c_{(j+1) mod k}) be the total clockwise
    //     length of C walked in cycle-order (an invariant of C, not of s).
    //     Telescoping the seven-term walk above, its cost is
    //
    //         d(z, c_s)  +  (W - d(c_{s+1}, c_s))  +  d(c_{s+1}, z),
    //
    //     and this collapses to exactly W iff z lies on the clockwise arc
    //     from c_{s+1} back to c_s. Equivalently, z is NOT on the open
    //     clockwise arc c_s -> c_{s+1}. Such an s always exists: take c_s as
    //     the first cycle vertex met going clockwise from z. By construction
    //     no cycle vertex lies in the open arc (z, c_s), so c_{s+1} (which is
    //     != c_s since k >= 2) must sit clockwise past c_s, and the edge
    //     c_s -> c_{s+1} cannot wrap back around to contain z.
    //
    // Try every cyclic rotation of b (where 0 ends up sitting), both
    // S1-first and S2-first, take the cheapest, then replay the swaps to
    // emit the answer.

    a[0] = b[0] = 0;
    for(int i = 0; i < n; i++) {
        a[i + 1] = s1[i] - 'A' + 1;
        b[i + 1] = s2[i] - 'A' + 1;
    }

    int best_dir = 0, best_rot = 0, best_cost = INT_MAX;
    for(int dir = 0; dir < 2; dir++) {
        for(int r = 0; r <= n; r++) {
            int c = compute_cost();
            if(c < best_cost) {
                best_cost = c;
                best_dir = dir;
                best_rot = r;
            }
            rotate(b.begin(), b.begin() + 1, b.begin() + n + 1);
        }

        swap(a, b);
    }

    if(best_dir == 1) {
        swap(a, b);
    }

    rotate(b.begin(), b.begin() + best_rot, b.begin() + n + 1);
    compute_cost();

    string result;
    for(int i = 1; i <= n; i++) {
        result += (char)('A' + a[i] - 1);
    }

    for(int p: move_costs) {
        if(p == 0) {
            continue;
        }
        result += (char)('A' + a[p] - 1);
        for(int i = 1; i < p; i++) {
            result += (char)('A' + a[i] - 1);
        }
        apply_move(p);
    }

    cout << result << '\n';
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
