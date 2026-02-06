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
double t;
vector<double> a;

void read() {
    cin >> n >> t;
    a.resize(n);
    cin >> a;
}

void solve() {
    // When we see this problem a direct thought is to try doing DP. Let's have
    // the state dp[i] be the minimal expected time to complete this process if
    // we know that the first i characters are correct. Then the answer is
    // dp[0], and we know that dp[n] = 0. Let's say we know the first k
    // character are correct and try to calculate dp[k]. Let's try to fix the
    // next time q = k + p at which we will try to look at the screen to verify.
    // Overall, this would take p + t time, and then we also need to account the
    // time needed to actually press backspace. There is probability a[k + 1]
    // that we will fail at the first typed character, which will lead us back
    // to dp[k], and (1 - a[k + 1]) probability that this will succeed and we
    // would fail at one of the later ones. If we continue like this, we get
    // that:
    //
    //    dp[k] = min(dp[k],
    //                p + t +
    //                a[k + 1] * (dp[k] + p) +
    //                (1 - a[k + 1]) * a[k + 2] * (dp[k + 1] + p - 1) +
    //                     ...
    //                (1 - a[k + 1]) * ... * (1 - a[p]) * dp[k])
    //
    // And this minimized over all k + p <= n. Note that there is a cyclic term
    // in all of these p, where the common approach when we don't optimize and
    // just compute expectation is to move it on the other side of the equation.
    // Turns out that this term depending on dp[k] is always the same! This
    // means that we can subtract both sides by a[k + 1] * dp[k], making the DP
    // a DAG. The only part left is to implement this efficiently. Naively
    // performing this minimum would be O(N^3) that is a bit too slow, but we
    // can notice that we can compute the p-th transition based on temporary
    // results based on the (p-1)th transition. This way we can also get a
    // quadratic complexity.

    vector<double> dp(n + 1, 0.0);

    for(int k = n - 1; k >= 0; k--) {
        dp[k] = 1e18;
        double q = 1.0;
        double c1 = 0.0, c2 = 0.0, b_val = 0.0;
        for(int p = 1; p <= n - k; p++) {
            q *= (1.0 - a[k + p - 1]);

            double g = p * (1.0 + a[k] + c1) - c2 + t + b_val + q * dp[k + p];
            dp[k] = min(dp[k], g / (1.0 - a[k]));
            if(p < n - k) {
                double term = q * a[k + p];
                c1 += term;
                c2 += term * p;
                b_val += term * dp[k + p];
            }
        }
    }

    cout << fixed << setprecision(15) << dp[0] << '\n';
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
