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

int n, mod;

void read() { cin >> n >> mod; }

void solve() {
    // dp [pos] [dir][value] = dp[pos - 1][!dir][x]      for x in [0, value)
    //                                                   if dir == 0
    //                                                   for x in [value, pos]
    //                                                   if dir == 1

    if(n == 1) {
        cout << 1 % mod << '\n';
        return;
    }

    vector<vector<int>> last_dp(2), dp(2);
    last_dp[0] = {1 % mod};
    last_dp[1] = {1 % mod};

    for(int pos = 1; pos < n; pos++) {
        // dir = 1
        dp[1].resize(pos + 1);
        dp[1][pos] = 0; 
        for(int value = pos - 1; value >= 0; value--) {
            dp[1][value] = last_dp[0][value] + dp[1][value + 1];
            if(dp[1][value] >= mod) {
                dp[1][value] -= mod;
            }
        }
        // dir = 0
        dp[0].resize(pos + 1);
        dp[0][0] = 0;
        for(int value = 1; value <= pos; value++) {
            dp[0][value] = last_dp[1][value - 1] + dp[0][value - 1];
            if(dp[0][value] >= mod) {
                dp[0][value] -= mod;
            }
        }
        swap(last_dp, dp);
    }

    int ans = 0;
    for(int value = 0; value <= n - 1; value++) {
        ans = (ans + last_dp[0][value]) % mod;
        ans = (ans + last_dp[1][value]) % mod;
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
