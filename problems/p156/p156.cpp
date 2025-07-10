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
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
}

int n, m;
vector<vector<int>> adj;
vector<bool> visited;
vector<int> next_node, hcycle;

void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

int match_nodes(int u, int root) {
    int ans = 1;
    visited[u] = true;
    for(int v: adj[u]) {
        if(adj[v].size() == 2) {
            next_node[u] = v;
        } else if(adj[v].size() > 2 && !visited[v]) {
            ans += match_nodes(v, root);
        }
    }
    return ans;
}

void dfs(int u, bool in_path) {
    visited[u] = true;
    if(adj[u].size() > 2 && !visited[next_node[u]]) {
        dfs(next_node[u], false);
    }
    for(int v: adj[u]) {
        if(!visited[v]) {
            if(adj[u].size() == 2) {
                dfs(v, false);
            } else if(!in_path && adj[v].size() > 2) {
                dfs(v, true);
            }
        }
    }
    hcycle.push_back(u);
}

void read() {
    cin >> n >> m;
    adj.assign(n, {});
    visited.assign(n, false);
    next_node.assign(n, -1);

    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        add_edge(u, v);
    }
}

void solve() {
    for(int i = 0; i < n; i++) {
        if(adj[i].size() > 2 && !visited[i]) {
            if(match_nodes(i, i) & 1) {
                cout << "-1\n";
                return;
            }
        }
    }

    fill(visited.begin(), visited.end(), false);
    hcycle.clear();
    dfs(0, false);

    if(hcycle.size() != n) {
        cout << "-1\n";
    } else {
        reverse(hcycle.begin(), hcycle.end());
        for(int u: hcycle) {
            cout << u + 1 << " ";
        }
        cout << endl;
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
