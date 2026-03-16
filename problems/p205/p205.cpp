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

const int64_t inf = (int64_t)1e18 + 42;

int n, m, s;
vector<int> x;
vector<vector<int>> L;

void read() {
    cin >> n;
    x.resize(n);
    cin >> x;
    cin >> m >> s;
    L.resize(m, vector<int>(s));
    for(auto& row: L) {
        cin >> row;
    }
}

void solve() {
    // Given n values x[0..n-1], m=2^p level sets each of size s=2^q, where
    // L[i][k] is the k-th level in set i. We must replace each x[j] with some
    // level L[set][k] from the current set, minimizing sum |x[j] - L[set][k]|.
    // The first value uses set 0. The chosen index k's p LSBs
    // determine the next set: next_set = k & (m-1). So picking a suboptimal
    // level now can lead to a better set (and lower total error) later.
    //
    // We solve this with DP backwards. dp[j][set] is the min total deviation
    // for x[j..n-1] when position j uses level set `set`. For each (j, set), we
    // try all s level indices k, pay |x[j] - L[set][k]| and transition to
    // dp[j+1][k & (m-1)]. We track choices for reconstruction. The complexity
    // is O(n * m * s), with n=1000, m=128, s=128 that's ~16M ops, fast enough
    // for the 0.25s time limit.

    vector<vector<int64_t>> dp(n + 1, vector<int64_t>(m, 0));
    vector<vector<int>> choice(n, vector<int>(m, -1));

    for(int j = n - 1; j >= 0; j--) {
        for(int set = 0; set < m; set++) {
            dp[j][set] = inf;
            for(int k = 0; k < s; k++) {
                int next_set = k & (m - 1);
                int64_t cost = abs(x[j] - L[set][k]) + dp[j + 1][next_set];
                if(cost < dp[j][set]) {
                    dp[j][set] = cost;
                    choice[j][set] = k;
                }
            }
        }
    }

    cout << dp[0][0] << '\n';
    int cur_set = 0;
    for(int j = 0; j < n; j++) {
        int k = choice[j][cur_set];
        cout << k << " \n"[j == n - 1];
        cur_set = k & (m - 1);
    }
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
