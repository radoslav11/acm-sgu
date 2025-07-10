#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n, m, k;

void read() {
    cin >> n >> m >> k;
}

void solve() {
    vector<vector<int64_t>> dp(n - m + 1, vector<int64_t>(1 << m, 0));

    for(int mask = 0; mask < (1 << m); mask++) {
        int cnt = __builtin_popcount(mask);
        if(cnt < k) {
            continue;
        }

        dp[0][mask] = 1; 
    }

    for(int i = 1; i < n - m + 1; i++) {
        for(int mask = 0; mask < (1 << m); mask++) {
            if(dp[i - 1][mask] == 0) {
                continue;
            }

            for(int bit = 0; bit < 2; bit++) {
                int new_mask = (mask << 1) | bit;
                new_mask &= (1 << m) - 1;

                int cnt = __builtin_popcount(new_mask);
                if(cnt < k) {
                    continue;
                }

                dp[i][new_mask] += dp[i - 1][mask];
            }
        }
    }

    int64_t ans = 0;
    for(int mask = 0; mask < (1 << m); mask++) {
        ans += dp[n - m][mask];
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
