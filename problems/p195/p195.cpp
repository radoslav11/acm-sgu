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
vector<vector<int>> adj;

void read() {
    cin >> n;
    adj.assign(n, {});
    for(int i = 1; i < n; i++) {
        int p;
        cin >> p;
        adj[p - 1].push_back(i);
    }
}

void solve() {
    vector<vector<int>> dp(n, vector<int>(2, 0));
    for(int u = n - 1; u >= 0; u--) {
        dp[u][1] = 1;
        for(int v: adj[u]) {
            dp[u][1] += dp[v][0];
        }

        int sum_dp_0 = 0;
        for(int v: adj[u]) {
            sum_dp_0 += dp[v][0];
        }

        for(int v: adj[u]) {
            dp[u][0] = max(dp[u][0], sum_dp_0 - dp[v][0] + dp[v][1]);
        }
        dp[u][1] = max(dp[u][0], dp[u][1]);
    }

    vector<int> ans;
    vector<bool> take(n, false);
    for(int u = 0; u < n; u++) {
        if(take[u] && dp[u][1] == dp[u][0]) {
            take[u] = false;
        }

        if(take[u]) {
            ans.push_back(u + 1);
            for(int v: adj[u]) {
                take[v] = false;
            }
        } else {
            int sum_dp_0 = 0;
            for(int v: adj[u]) {
                sum_dp_0 += dp[v][0];
            }

            int take_child = -1;
            for(int v: adj[u]) {
                if(dp[u][0] == sum_dp_0 - dp[v][0] + dp[v][1]) {
                    take_child = v;
                    take[v] = true;
                    break;
                }
            }

            for(int v: adj[u]) {
                if(v != take_child) {
                    take[v] = false;
                }
            }
        }
    }

    cout << dp[0][0] * 1000 << '\n';
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
