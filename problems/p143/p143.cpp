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

int n;
vector<int> a;
vector<vector<int>> adj;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
    adj.assign(n, vector<int>());
    for(int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

int dfs(int u, int p, int &ans) {
    int res = 0;
    for(int v: adj[u]) {
        if(v == p) {
            continue;
        }
        res += dfs(v, u, ans);
    }

    ans = max(ans, res + a[u]);
    return max(0, res + a[u]);
}

void solve() {
    int ans = INT_MIN;
    dfs(0, -1, ans);
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
