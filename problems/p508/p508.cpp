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

const long double eps = 1e-15;

int n, l1, l2;
long double p;

void read() {
    cin >> n >> l1 >> l2 >> p;
    p /= 100;
}

void solve() {
    // We can solve the problem with a fairly simple bayesian approach:
    //     Prior: uniform over k = 0..n, P(k) = 1/(n+1)
    //     Likelihood: P(data | k) = C(k, l1) * C(n-k, l2) / C(n, l1+l2).
    //
    // Then we have:
    //     P(k | data) = (P(data | k) * P(k)) / P(data)
    //
    // All terms apart from P(data | k) are constant w.r.t k, so we have:
    //     P(k | data) ~ C(k, l1) * C(n-k, l2).
    //
    // We know that sum_{k=0 to n} P(k | data) = 1, so we can directly calculate
    // the P(k | data) by demeaning.
    //
    // We need the smallest-length contiguous interval [a, b] such that:
    //    sum_{k=a to b} P(k | data) >= p
    //
    // Among all such minimal-length intervals, choose the one with least a.
    // This can be done by iterating over all intervals and using prefix sums
    // for the probabilities. We should be careful about what epsilon we use, as
    // the probabilities might differ by very small amounts. Using 1e-15 works.

    vector<vector<long double>> C(n + 1, vector<long double>(n + 1, 0));
    for(int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for(int j = 1; j < i; j++) {
            C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
    }

    vector<long double> weight(n + 1, 0.0);
    long double total = 0.0;
    for(int k = 0; k <= n; k++) {
        int64_t c1 = (l1 <= k ? C[k][l1] : 0);
        int64_t c2 = (l2 <= n - k ? C[n - k][l2] : 0);
        weight[k] = (long double)c1 * c2;
        total += weight[k];
    }

    vector<long double> post(n + 1);
    for(int k = 0; k <= n; k++) {
        post[k] = weight[k] / total;
    }

    vector<long double> pref_p(n + 2, 0.0);
    for(int k = 0; k <= n; k++) {
        pref_p[k + 1] = pref_p[k] + post[k];
    }

    int best_len = n + 1;
    pair<int, int> ans = {0, n};
    for(int len = 0; len <= n; len++) {
        for(int a = 0; a + len <= n; a++) {
            int b = a + len;
            long double prob = pref_p[b + 1] - pref_p[a];
            if(prob + eps >= p) {
                if(len < best_len || (len == best_len && a < ans.first)) {
                    best_len = len;
                    ans = {a, b};
                }
            }
        }
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
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
