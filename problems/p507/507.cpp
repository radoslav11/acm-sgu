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

const int INF = 2147483647;

int n, m;
vector<int> a;
vector<vector<int>> adj;

void read() {
    cin >> n >> m;
    a.resize(n);
    adj.assign(n, vector<int>());
    for(int i = 1; i < n; i++) {
        int par;
        cin >> par;
        adj[par - 1].push_back(i);
    }

    for(int i = 0; i < m; i++) {
        cin >> a[n - m + i];
    }
}

set<int> dfs(int u, vector<int> &answer) {
    if(adj[u].empty()) {
        return {a[u]};
    }

    set<int> s;
    for(auto v: adj[u]) {
        set<int> t = dfs(v, answer);

        answer[u] = min(answer[u], answer[v]);
        if(s.size() < t.size()) {
            swap(s, t);
        }

        for(auto x: t) {
            auto it = s.lower_bound(x);
            if(it != s.end()) {
                answer[u] = min(answer[u], *it - x);
            } 
            if(it != s.begin()) {
                answer[u] = min(answer[u], x - *prev(it));
            }
            s.insert(x);
        }
    }

    return s;
}

void solve() {
    vector<int> answer(n, INF);
    dfs(0, answer);
    for(int i = 0; i < n - m; i++) {
        cout << answer[i] << ' ';
    }
    cout << '\n';
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
