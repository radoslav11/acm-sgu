#include <bits/stdc++.h>
#define endl '\n'

// #pragma GCC optimize ("O3")
// #pragma GCC target ("sse4")

using namespace std;
template<class T, class T2>
inline void chkmax(T& x, const T2& y) {
    if(x < y) {
        x = y;
    }
}
template<class T, class T2>
inline void chkmin(T& x, const T2& y) {
    if(x > y) {
        x = y;
    }
}
const int MAXN = (1 << 16);

int n;
vector<int> adj[MAXN];

void read() {
    cin >> n;
    for(int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

int tr_sz[MAXN];

void dfs(int u, int pr) {
    tr_sz[u] = 1;
    for(int v: adj[u]) {
        if(v != pr) {
            dfs(v, u);
            tr_sz[u] += tr_sz[v];
        }
    }
}

int val[MAXN];

void solve() {
    dfs(1, 1);

    vector<int> li;
    int mn = n;
    for(int u = 1; u <= n; u++) {
        val[u] = n - tr_sz[u];
        for(int v: adj[u]) {
            if(tr_sz[v] < tr_sz[u]) {
                chkmax(val[u], tr_sz[v]);
            }
        }

        chkmin(mn, val[u]);
    }

    for(int i = 1; i <= n; i++) {
        if(val[i] == mn) {
            li.push_back(i);
        }
    }

    cout << mn << " " << li.size() << endl;
    for(int x: li) {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
