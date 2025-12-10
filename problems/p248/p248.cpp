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
vector<int> c;
int V;

void read() {
    cin >> n;
    c.resize(n);
    cin >> c;
    cin >> V;
}

void solve() {
    // This solution uses dynamic programming to solve a coin change variant
    // where we need to find the minimum number of variables(coins) to make
    // exactly the target value V.The array dp[v] stores the minimum sum of
    // variables needed to achieve value v, starting with dp[0] = 0. For each
    // value from 1 to V, we try adding each coefficient c[i] and update dp[v]
    // with the minimum count, returning - 1 if V is unreachable. The complexity
    // is trivially O(nV). The main observation here is that both n and V are
    // reasonably small.

    vector<int> dp(V + 1, INT_MAX);
    dp[0] = 0;

    for(int v = 1; v <= V; v++) {
        for(int i = 0; i < n; i++) {
            if(v >= c[i] && dp[v - c[i]] != INT_MAX) {
                dp[v] = min(dp[v], dp[v - c[i]] + 1);
            }
        }
    }

    if(dp[V] == INT_MAX) {
        cout << -1 << endl;
    } else {
        cout << dp[V] << endl;
    }
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
