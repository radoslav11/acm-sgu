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
array<int, 4> cnt;

void read() {
    cin >> n;
    for(int i = 0; i < 4; i++) {
        cin >> cnt[i];
    }
}

array<array<int, 16>, 8> v_edge{};
array<array<int, 16>, 8> h_edge{};
unordered_map<int64_t, int64_t> memo;

int64_t active_mask(int r, int p) {
    int64_t mask = 0;
    int bit = 0;
    if(r >= 2) {
        for(int k = p / 2; k <= r - 2; k++) {
            mask |= (int64_t)(h_edge[r - 1][k] & 1) << bit++;
        }
    }
    if(p > 0) {
        mask |= (int64_t)(v_edge[r][p - 1] & 1) << bit++;
    }
    int built = (p + 1) / 2;
    for(int k = 0; k < built; k++) {
        mask |= (int64_t)(h_edge[r][k] & 1) << bit++;
    }
    return mask;
}

int64_t encode_key(int r, int p, int64_t mask) {
    int64_t key = (r - 1) * (r - 1) + p;
    key = (key << 16) | mask;
    for(int i = 0; i < 4; i++) {
        key = (key << 5) | cnt[i];
    }
    return key;
}

int64_t rec(int r, int p) {
    if(r > n) {
        return 1;
    }

    int64_t key = encode_key(r, p, active_mask(r, p));
    auto it = memo.find(key);
    if(it != memo.end()) {
        return it->second;
    }

    int next_r = r, next_p = p + 1;
    if(next_p >= 2 * r - 1) {
        next_r = r + 1;
        next_p = 0;
    }

    bool is_up = (p % 2 == 0);
    int fixed_left = (p > 0) ? v_edge[r][p - 1] : -1;
    int fixed_top = is_up ? -1 : h_edge[r - 1][(p - 1) / 2];

    int64_t total = 0;
    for(int kind = 0; kind < 4; kind++) {
        if(cnt[kind] == 0) {
            continue;
        }

        int num_black = kind;
        for(int mask = 0; mask < 8; mask++) {
            if(__builtin_popcount(mask) != num_black) {
                continue;
            }

            int e_left = (mask >> 0) & 1;
            int e_right = (mask >> 1) & 1;
            int e_third = (mask >> 2) & 1;

            if(fixed_left != -1 && fixed_left != e_left) {
                continue;
            }
            if(!is_up && fixed_top != e_third) {
                continue;
            }

            v_edge[r][p] = e_right;
            if(is_up) {
                h_edge[r][p / 2] = e_third;
            }

            cnt[kind]--;
            total += rec(next_r, next_p);
            cnt[kind]++;
        }
    }

    memo[key] = total;
    return total;
}

void solve() {
    // The big triangle has n rows; row r (1..n) contains 2r-1 unit cells at
    // positions 0..2r-2, with upward triangles at even positions and downward
    // triangles at odd positions. Edges shared between cells:
    //
    //   - v_edge[r][p] is the vertical edge between cells p and p+1 of row r
    //     (the right side of cell p, the left side of cell p+1);
    //
    //   - h_edge[r][k] is the horizontal edge between row r and row r+1, with
    //     k indexing the k-th such edge from the left; it equals the bottom
    //     side of the upward cell at (r, 2k) and the top side of the downward
    //     cell at (r+1, 2k+1).
    //
    // Kind 1+k has exactly k black edges out of 3 (k = 0..3); placing such a
    // triangle in any orientation means choosing which of (left, right, third)
    // sides receive the k black colors, where the "third" side is the bottom
    // for an upward cell and the top for a downward cell.
    //
    // We DP over cells in row-major order. At the moment we are about to
    // place cell (r, p), the future is determined by the remaining counts
    // cnt[0..3] together with a small "active border" of already-determined
    // edges that future cells will still read:
    //
    //   - h_edge[r-1][k] for k = floor(p/2) .. r-2 — bottoms of row r-1 that
    //     have not yet been consumed by row r's downward cells;
    //
    //   - v_edge[r][p-1] when p > 0 — the left side that the current cell
    //     must match;
    //
    //   - h_edge[r][k] for k = 0 .. ceil(p/2)-1 — bottoms of row r's upward
    //     cells already placed, which will be read by row r+1.
    //
    // That mask has at most n+1 bits (six bits for n=5). Memoizing on (cell
    // index, mask, cnt[0..3]) collapses the search: for each placement we
    // enumerate every (kind, orientation), keep those consistent with the
    // fixed left/top edges, decrement the kind count, and recurse.

    memo.clear();
    cout << rec(1, 0) << '\n';
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
