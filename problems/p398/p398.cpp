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
const int MAXN = (1 << 6);

int n, x;
int adj[MAXN][MAXN];

void read() {
    cin >> n >> x;
    for(int i = 1; i <= n; i++) {
        int cnt;
        cin >> cnt;
        while(cnt--) {
            int x;
            cin >> x;
            adj[x][i] = 1;
            adj[i][x] = 1;
        }
    }
}

void solve() {
    vector<int> li;
    for(int i = 1; i <= n; i++) {
        bool ok = (i != x && adj[i][x] == 0);
        if(!ok) {
            continue;
        }

        ok = 0;
        for(int o = 1; o <= n; o++) {
            if(adj[x][o] && adj[o][i]) {
                ok = 1;
            }
        }

        if(ok) {
            li.push_back(i);
        }
    }

    cout << li.size() << endl;
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
