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

using coord3_t = double;

const coord3_t PI = acos((coord3_t)-1.0);

int n;

void read() { cin >> n; }

void solve() {
    // We solve Game with points by dynamic programming on the optimal way
    // to grow a star-of-arms tree rooted at A0. This structure is optimal
    // because it simultaneously maximizes the current max_degree (number of
    // arms d) and the current max_distance (diameter of the two longest arms)
    // for any number of added points k.
    //
    // At step k (after adding the k-th point) with d arms:
    //   - max_degree = d
    //   - max_distance = k - max(d - 2, 0)   (achieved by placing the two
    //     longest arms opposite each other in straight lines of length 1;
    //     the remaining d-2 arms are single-point leaves)
    //
    // The contribution at step k is d * (k - max(d-2,0)).
    //
    // Transitions between steps respect the geometry and rules:
    //   - "extend" (attach new point to the end of one of the two longest
    //   arms):
    //     d stays the same, diameter increases by 1.
    //   - "new arm" (attach new point directly to A0):
    //     d increases by 1, diameter stays the same (new arm length 1).
    //
    // dp[k][d] stores the maximum total score after exactly k additions ending
    // with exactly d arms. The DP runs in O(N^2) time (N <= 1000) and selects
    // the globally optimal sequence of actions. For N=0 the score is 0.
    //
    // Why double? The problem requires the answer accurate to 1e-3, and output
    // must be printed with exactly three decimal places (fixed
    // setprecision(3)). Although the optimal score under this construction is
    // always an integer, we use double throughout for safety, precision
    // handling, and to match the required output format.
    //
    // Corner case / non-straight lines: bending an arm (non-straight) can never
    // improve the score. It either keeps diameter the same or decreases it
    // (by triangle inequality) while the max degree cannot exceed what the star
    // already achieves. Straight opposite arms strictly maximize diameter for
    // any given d and k, so any deviation is suboptimal or equal at best.
    // The construction also satisfies all geometric constraints (distinct
    // directions, no intersections, lengths <= 1, no point on segment).

    if(n == 0) {
        cout << fixed << setprecision(3) << 0.0 << '\n';
        return;
    }

    const double INF = -1e18;
    vector<vector<double>> dp(n + 1, vector<double>(n + 2, INF));
    dp[1][1] = 1.0;

    for(int k = 2; k <= n; ++k) {
        for(int d = 1; d <= k; ++d) {
            double dist = k - max(d - 2, 0);
            double p = d * dist;

            // came from extend (same d)
            if(d <= k - 1) {
                dp[k][d] = max(dp[k][d], dp[k - 1][d] + p);
            }
            // came from new arm (previous d-1)
            if(d > 1 && d - 1 <= k - 1) {
                dp[k][d] = max(dp[k][d], dp[k - 1][d - 1] + p);
            }
        }
    }

    double ans = INF;
    for(int d = 1; d <= n; ++d) {
        ans = max(ans, dp[n][d]);
    }
    cout << fixed << setprecision(3) << ans << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
