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
vector<vector<int>> adj;

void read() {
    cin >> n >> m;
    adj.assign(n, {});
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

bool is_bipartite(vector<int> &color) {
    queue<int> q;
    for(int i = 0; i < n; i++) {
        if(color[i] != -1) {
            continue;
        }
        color[i] = 0;
        q.push(i);
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v: adj[u]) {
                if(color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if(color[v] == color[u]) {
                    return false;
                }
            }
        }
    }
    return true;
}

void solve() {
    vector<int> color(n, -1);
    if(is_bipartite(color)) {
        cout << "yes\n";
        vector<int> a;
        for(int i = 0; i < n; i++) {
            if(color[i] == 0) {
                a.push_back(i + 1);
            }
        }

        cout << a.size() << '\n';
        cout << a << '\n';
    } else {
        cout << "no\n";
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
