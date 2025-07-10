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

string s;

void read() { cin >> s; }

void solve() {
    if(s == "0") {
        cout << s << '\n';
        return;
    }

    int n = s.size();
    vector<vector<int64_t>> dp(
        1 << n, vector<int64_t>(17, numeric_limits<int64_t>::max())
    );
    dp[0][0] = 0;
    for(int mask = 0; mask < (1 << n); mask++) {
        int cnt_non_zero = 0;
        for(int i = 0; i < n; i++) {
            if((mask & (1 << i)) && s[i] != '0') {
                cnt_non_zero++;
            }
        }

        for(int i = 0; i < n; i++) {
            if((s[i] == '0' && cnt_non_zero == 0) || (mask & (1 << i))) {
                continue;
            }

            for(int j = 0; j < 17; j++) {
                if(dp[mask][j] == numeric_limits<int64_t>::max()) {
                    continue;
                }
                int nmask = mask | (1 << i);
                int nval = (j * 10 + s[i] - '0') % 17;
                dp[nmask][nval] =
                    min(dp[nmask][nval], dp[mask][j] * 10 + s[i] - '0');
            }
        }
    }

    int64_t ans = dp[(1 << n) - 1][0];
    if(ans == numeric_limits<int64_t>::max()) {
        cout << "-1\n";
        return;
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
