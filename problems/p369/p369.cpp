#include <bits/stdc++.h>
#define endl '\n'

// #pragma GCC optimize ("O3")
// #pragma GCC target ("sse4")

using namespace std;
template<class T, class T2>
inline int chkmax(T& x, const T2& y) {
    return x < y ? x = y, 1 : 0;
}
template<class T, class T2>
inline int chkmin(T& x, const T2& y) {
    return x > y ? x = y, 1 : 0;
}
const int MAXN = (1 << 20);

int n;
vector<int> adj[MAXN];
bool used[MAXN];
map<int, int> col_last, row_last;
int x[MAXN], y[MAXN];

void read() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];

        if(col_last.count(y[i])) {
            adj[i].push_back(col_last[y[i]]);
            adj[col_last[y[i]]].push_back(i);
        } else {
            col_last[y[i]] = i;
        }

        if(row_last.count(x[i])) {
            adj[i].push_back(row_last[x[i]]);
            adj[row_last[x[i]]].push_back(i);
        } else {
            row_last[x[i]] = i;
        }
    }
}

set<int> rw, cl;

void dfs(int u) {
    used[u] = 1;
    rw.insert(x[u]);
    cl.insert(y[u]);
    for(int v: adj[u]) {
        if(!used[v]) {
            dfs(v);
        }
    }
}

void solve() {
    int64_t answer = 0;
    for(int i = 0; i < n; i++) {
        if(!used[i]) {
            rw.clear();
            cl.clear();
            dfs(i);
            answer += rw.size() * 1ll * cl.size();
        }
    }

    cout << answer << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
