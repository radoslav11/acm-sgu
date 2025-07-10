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

int n, m;
vector<int> a;

void read() {
    cin >> n >> m;
    a.resize(n);
    cin >> a;
}

void solve() {
    vector<vector<int>> dp(n, vector<int>(m + 1, (int)1e9));
    for(int i = 0; i < m - 1; i++) {
        dp[i][i + 1] = a[i];
    }

    int ans = (int)1e9;
    for(int i = 0; i < n; i++) {
        for(int prv_dist = 1; prv_dist <= m; prv_dist++) {
            for(int dist = 1; dist <= m; dist++) {
                int nxt = i + dist;
                if(nxt < n && prv_dist + dist <= m) {
                    dp[nxt][dist] =
                        min(dp[nxt][dist], dp[i][prv_dist] + a[nxt]);
                }
            }

            int left_to_end = n - i;
            if(prv_dist + left_to_end <= m) {
                ans = min(ans, dp[i][prv_dist]);
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
