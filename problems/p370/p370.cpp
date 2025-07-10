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

void read() { cin >> n >> m; }

void solve() {
    int64_t ans = (n - 1) * 1ll * (m - 1);
    vector<int64_t> dp(min(n, m) + 1, 0);
    for(int d = min(n, m); d >= 2; d--) {
        dp[d] = ((n - 1) / d) * 1ll * ((m - 1) / d);
        for(int d2 = 2 * d; d2 <= min(n, m); d2 += d) {
            dp[d] -= dp[d2];
        }

        ans -= dp[d];
    }
    cout << ans + (n > 1) + (m > 1)  << '\n';
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
