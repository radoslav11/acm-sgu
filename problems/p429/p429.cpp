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
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
}

int n;
vector<int> a;

static inline int apply_func(pair<int, int> fn, int x) {
    // apply_func represents the middle transform g[l,r](x) with a pair (L, R)
    // and applies it to x as described in the analysis.

    int L = fn.first, R = fn.second;
    int lo = min(L, R), hi = max(L, R);
    // If x is outside [lo, hi], return x unchanged
    if(max(lo, min(x, hi)) != x) {
        return x;
    }
    // Otherwise step toward R by +1 if L <= R else -1;
    // hitting R maps to 0
    int step;
    if(L <= R) {
        step = 1;
    } else {
        step = -1;
    }
    if(x != R) {
        return x + step;
    } else {
        return 0;
    }
}

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    // There is a direct O((n*max)^2) solution with DP, but this is too slow
    // under the given constraints. Instead, we will make a series of
    // observations and derive a O(n^2) solution.
    //
    // - With two heaps, this is Nim on 2 piles: the player to move loses
    //   exactly when the two ends are equal (a[1] == a[2]).
    //
    // - For more heaps, the middle segment acts like a transform that maps
    //   the leftmost size x to the unique rightmost size y that makes the
    //   position losing (a P-position). Call this mapping g[l,r](x).
    //
    // - To show the uniqueness of the losing y for a given x, consider the
    //   following:
    //     (1) Any q < g[l,r](x) can be reached in a single move; if such a q
    //         were losing, then g[l,r](x) would be winning - contradiction.
    //     (2) Any q > g[l,r](x) is winning because it can move to g[l,r](x),
    //         which is losing by definition.
    //
    // - By looking of the structure of g[l,r], and an argument similar to
    //   above, we can notice that we can encode each g[l,r] by a pair (L, R):
    //     (1) If x is outside the closed interval between L and R, g(x) = x.
    //         Intuitively, we can think of this as a symmetry argument, as the
    //         "inner" range is effectively losing.
    //     (2) If x lies inside that interval, g[l,r](x) moves by one toward R;
    //         for example if moving, g[l,r](R) = 0 (meaning: there is no
    //         positive y that makes the position losing).
    //
    // - We can build up g[l,r] recursively:
    //     (1) Let g[l,r] store the pair (L, R) that encodes it.
    //     (2) For intervals of length 2, the middle is empty, so g is identity,
    //         or g = (0, 0). Expanding here, the only losing states are when
    //         the second player mirrors the first (Nim on 2 heaps).
    //     (3) For length >= 3, we can reuse the already computed g[l+1,r] and
    //         g[l,r-1] to compute g[l,r]:
    //         * g[l+1,r](a[l+1]) gives us the unique losing value for a[r]
    //           assuming a[l] has been cleared. Denote it L'.
    //         * inv(g[l,r-1])(a[r-1]) gives us the unique loosing value for
    //           a[l] assuming a[r] has been cleared. Denote it R'.
    //         * WLOG, assume L' < R'. We can notice immediately that g[l,r](x)
    //           can't be L' because we can win by fully clearing a[l]. We can
    //           also notice that g[l,r](x) = x for x < L' because if a[l] =
    //           a[r] = x, the second player can mirror the first one until the
    //           game ends. This requires some time to get convinced. Let's now
    //           consider x = L'. We can show that a[r] = L'+1 is losing for
    //           the first player, because all reachable states (reduction in
    //           a[l] or a[r]) are winning. This is one of the key observations
    //           - by similar logic, we can show that g[l,r](L'+1) = L'+2, and
    //           so on, until we reach R' where we have to think a bit more.
    //           Intuitively, we should expect g[l,r](R') = R'+1 using the
    //           same "cascading" logic, but we will show that g[l,r](R') = 0,
    //           meaning there is no y that makes the position losing. This is
    //           because we know that a[l] = R' is losing if a[r] is cleared. We
    //           are now left with a[l] > R'. In this case, if a[r] = R', the
    //           second player can either immediately force the first one into
    //           one of the above-described losing states, or if the first
    //           player goes to a[l] = R', the second player can clear a[r] and
    //           win.
    //     (4) The case L' > R' is symmetric. We can notice that g[l,r] can be
    //         "inverted" for the L',R'. This is particularly useful because
    //         it gives us the function f(x) = y meaning the loosing a[l] = y
    //         when a[r] = x. We compute R' precisely this way.
    //
    // - Final decision is simple:
    //     Fedor (first player) wins unless a[n] == g[1,n](a[1]).

    if(n == 1) {
        cout << "FEDOR\n";
        return;
    }

    // g[l][r] encodes g[l,r] as a pair (L, R)
    vector<vector<pair<int, int>>> g(n, vector<pair<int, int>>(n, {0, 0}));

    for(int len = 3; len <= n; ++len) {
        for(int l = 0; l + len <= n; ++l) {
            int r = l + len - 1;
            g[l][r] = {
                // L_{l,r} = g[l+1,r](a[l+1])
                apply_func(g[l + 1][r], a[l + 1]),
                // R_{l,r} = inv(g[l,r-1]) applied to a[r-1]
                apply_func({g[l][r - 1].second, g[l][r - 1].first}, a[r - 1])
            };
        }
    }

    int target = apply_func(g[0][n - 1], a[0]);
    if(target == a[n - 1]) {
        cout << "SERGEY\n";
    } else {
        cout << "FEDOR\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
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
