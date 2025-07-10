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
const int MAXN = (1 << 20);

int n;

void read() { cin >> n; }

int answer[MAXN];
vector<int> adj[MAXN];
bool used[MAXN];

void solve() {
    for(int i = 1; i <= n; i++) {
        answer[i] = 1;
    }
    for(int i = 1; i <= n; i++) {
        for(int j = i * 2; j <= n; j += i) {
            adj[j].pb(i);
        }
    }

    for(int i = 1; i <= n; i++) {
        for(int v: adj[i]) {
            used[answer[v]] = 1;
        }
        while(used[answer[i]]) {
            answer[i]++;
        }
        for(int v: adj[i]) {
            used[answer[v]] = 0;
        }
    }

    int mx_col = 1;
    for(int i = 1; i <= n; i++) {
        chkmax(mx_col, answer[i]);
    }

    cout << mx_col << endl;
    for(int i = 1; i <= n; i++) {
        cout << answer[i] << " ";
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
