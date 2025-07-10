#include <bits/stdc++.h>
#define endl '\n'

// #pragma GCC optimize ("O3")
// #pragma GCC target ("sse4")

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T2>
inline int chkmax(T& x, const T2& y) {
    return x < y ? x = y, 1 : 0;
}
template<class T, class T2>
inline int chkmin(T& x, const T2& y) {
    return x > y ? x = y, 1 : 0;
}
const int MAXN = (1 << 10);

int n, m;
int a[MAXN][MAXN];

void read() {
    cin >> n >> m;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }
}

int best[MAXN][MAXN];
int dp[MAXN][MAXN];

int rec(int i, int j) {
    if(i == n) {
        return 0;
    }
    if(j == m) {
        return -1e9;
    }

    int& memo = dp[i][j];
    if(memo != -1) {
        return memo;
    }

    best[i][j] = 0;
    memo = rec(i, j + 1);

    if(chkmax(memo, a[i][j] + rec(i + 1, j + 1))) {
        best[i][j] = 1;
    }

    return memo;
}

int ans[MAXN];

void solve() {
    memset(dp, -1, sizeof(dp));

    int i = 0, j = 0;
    cout << rec(i, j) << endl;
    while(i != n) {
        rec(i, j);
        if(best[i][j] == 0) {
            j++;
        } else {
            ans[i] = j, i++, j++;
        }
    }

    for(int i = 0; i < n; i++) {
        cout << ans[i] + 1 << " ";
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
