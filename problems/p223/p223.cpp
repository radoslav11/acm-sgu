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

int n, k;

void read() { cin >> n >> k; }

void solve() {
    vector<vector<vector<vector<int64_t>>>> dp =
        vector<vector<vector<vector<int64_t>>>>(
            2 << n,
            vector<vector<vector<int64_t>>>(
                2, vector<vector<int64_t>>(n + 1, vector<int64_t>(k + 1, 0))
            )
        );

    dp[0][0][0][k] = 1;

    for(int i = 0, pi = 0; i < n; ++i, pi ^= 1) {
        // Clear pi ^ 1
        for(int mask = 0; mask < (2 << n); ++mask) {
            for(int j = 0; j <= n; ++j) {
                for(int l = 0; l <= k; ++l) {
                    dp[mask][pi ^ 1][j][l] = 0;
                }
            }
        }

        for(int j = 0; j <= n; ++j) {
            for(int mask = 0; mask < (2 << n); ++mask) {
                for(int l = 0; l <= k; ++l) {
                    if(dp[mask][pi][j][l] == 0) {
                        continue;
                    }

                    if(j == n) {
                        dp[mask][pi ^ 1][0][l] += dp[mask][pi][j][l];
                        continue;
                    }

                    dp[(mask << 1) & ((2 << n) - 1)][pi][j + 1][l] +=
                        dp[mask][pi][j][l];

                    int bit_left = (mask & 1), bit_up = (mask >> (n - 1)) & 1,
                        bit_diag_left = (mask >> n) & 1,
                        bit_diag_right = (mask >> (n - 2)) & 1;

                    if(i == 0) {
                        bit_up = 0;
                        bit_diag_left = 0;
                        bit_diag_right = 0;
                    }

                    if(j == 0) {
                        bit_left = 0;
                        bit_diag_left = 0;
                    }

                    if(j == n - 1) {
                        bit_diag_right = 0;
                    }

                    if(bit_left == 0 && bit_up == 0 && bit_diag_left == 0 &&
                       bit_diag_right == 0 && l > 0) {
                        dp[((mask << 1) | 1) & ((2 << n) - 1)][pi][j + 1]
                          [l - 1] += dp[mask][pi][j][l];
                    }
                }
            }
        }
    }

    int64_t ans = 0;
    for(int mask = 0; mask < (2 << n); ++mask) {
        ans += dp[mask][n & 1][0][0];
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
