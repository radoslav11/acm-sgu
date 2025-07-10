#include <bits/stdc++.h>
#include <bitset>

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
    // vector<vector<int64_t>> can_cover(1 << m, vector<int64_t>(1 << m,
    // false));

    // can_cover[0][0] = 1;
    // for(int mask = 0; mask < (1 << m); mask++) {
    //     for(int nmask = 0; nmask < (1 << m); nmask++) {
    //         int i = m - 1;
    //         while(i >= 0 && !(mask & (1 << i)) && !(nmask & (1 << i))) {
    //             i--;
    //         }

    //         if(i == -1) {
    //             continue;
    //         }

    //         if((mask & (1 << i)) && (nmask & (1 << i))) {
    //             can_cover[mask][nmask] +=
    //                 can_cover[mask ^ (1 << i)][nmask ^ (1 << i)];
    //         }

    //         if(i > 0 && (mask & (1 << i)) && (mask & (1 << (i - 1)))) {
    //             can_cover[mask][nmask] +=
    //                 can_cover[mask ^ (1 << i) ^ (1 << (i - 1))][nmask];
    //         }

    //         if(i > 0 && (nmask & (1 << i)) && (nmask & (1 << (i - 1)))) {
    //             can_cover[mask][nmask] +=
    //                 can_cover[mask][nmask ^ (1 << i) ^ (1 << (i - 1))];
    //         }

    //         if(i > 0 && (mask & (1 << i)) && (nmask & (1 << (i - 1))) &&
    //            (nmask & (1 << i))) {
    //             can_cover[mask][nmask] +=
    //                 can_cover[mask ^ (1 << i)]
    //                          [nmask ^ (1 << i) ^ (1 << (i - 1))];
    //         }

    //         if(i > 0 && (mask & (1 << (i - 1))) && (nmask & (1 << i)) &&
    //            (nmask & (1 << (i - 1)))) {
    //             can_cover[mask][nmask] +=
    //                 can_cover[mask ^ (1 << (i - 1))]
    //                          [nmask ^ (1 << i) ^ (1 << (i - 1))];
    //         }

    //         if(i > 0 && (nmask & (1 << i)) && (mask & (1 << (i - 1))) &&
    //            (nmask & (1 << i))) {
    //             can_cover[mask][nmask] +=
    //                 can_cover[mask ^ (1 << (i - 1))]
    //                          [nmask ^ (1 << i) ^ (1 << (i - 1))];
    //         }

    //         if(i > 0 && (nmask & (1 << i)) && (mask & (1 << (i - 1))) &&
    //            (mask & (1 << (i - 1)))) {
    //             can_cover[mask][nmask] +=
    //                 can_cover[mask ^ (1 << (i - 1))]
    //                          [nmask ^ (1 << i) ^ (1 << (i - 1))];
    //         }

    //         cout << bitset<3>(mask) << '\n'
    //              << bitset<3>(nmask) << '\n'
    //              << can_cover[mask][nmask] << '\n';
    //         cout << ">>>> " << i << '\n';
    //         cout << '\n';
    //     }
    // }

    // vector<vector<int64_t>> dp(n + 1, vector<int64_t>(1 << m, 0));
    // dp[0][(1 << m) - 1] = 1;
    // for(int i = 1; i <= n; i++) {
    //     for(int mask = 0; mask < (1 << m); mask++) {
    //         for(int nmask = 0; nmask < (1 << m); nmask++) {
    //             dp[i][nmask] +=
    //                 can_cover[mask ^ ((1 << m) - 1)][nmask] * dp[i -
    //                 1][mask];
    //         }
    //     }
    // }

    if(n == 1 || m == 1) {
        cout << (n + m) % 2 << endl;
        return;
    }

    vector<vector<int64_t>> dp(n * m + 1, vector<int64_t>(1 << (m + 1), 0));
    int total_mask = (1 << (m + 1)) - 1;

    dp[0][0] = 1;
    for(int p = 0; p < n * m; p++) {
        for(int mask = 0; mask < (1 << (m + 1)); mask++) {
            if(!dp[p][mask]) {
                continue;
            }

            int i = p / m, j = p % m;
            if((p > m && (mask & (1 << m))) || p <= m) {
                dp[p + 1][(mask << 1) & total_mask] += dp[p][mask];
            }

            //  XXXXXX
            // X..
            if(j > 0 && !(mask & 1)) {
                if((p > m && (mask & (1 << m))) || p <= m) {
                    dp[p + 1][((mask << 1) | 3) & total_mask] += dp[p][mask];
                }
            }

            //  X.XXXX
            // XX.
            if(i > 0 && !(mask & (1 << (m - 1)))) {
                if((p > m && (mask & (1 << m))) || p <= m) {
                    dp[p + 1][((mask << 1) | 1 | (1 << m)) & total_mask] +=
                        dp[p][mask];
                }
            }

            //  X.XXXX
            // X..
            if(i > 0 && j > 0 && !(mask & (1 << (m - 1))) && !(mask & 1)) {
                if((p > m && (mask & (1 << m))) || p <= m) {
                    dp[p + 1][((mask << 1) | 3 | (1 << m)) & total_mask] +=
                        dp[p][mask];
                }
            }

            //  ..XXXX
            // XX.
            if(i > 0 && j > 0 && !(mask & (1 << (m - 1))) &&
               !(mask & (1 << m))) {
                dp[p + 1][((mask << 1) | 1 | (1 << m)) & total_mask] +=
                    dp[p][mask];
            }

            //  .XXXXX
            // X..
            if(i > 0 && j > 0 && !(mask & (1 << m)) && !(mask & 1)) {
                dp[p + 1][((mask << 1) | 3) & total_mask] += dp[p][mask];
            }

            //  X..XXX
            // XX.
            if(i > 0 && j + 1 < m && !(mask & (1 << (m - 1))) &&
               !(mask & (1 << (m - 2)))) {
                if((p > m && (mask & (1 << m))) || p <= m) {
                    dp[p + 1]
                      [((mask << 1) | 1 | (1 << (m - 1)) | (1 << m)) &
                       total_mask] += dp[p][mask];
                }
            }
        }
    }

    cout << dp[n * m][total_mask] << endl;
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
