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

int n, a, k, m;

int pw(int x, int p) {
    int r = 1 % k;
    while(p) {
        if(p & 1) {
            r = r * 1ll * x % k;
        }
        x = x * 1ll * x % k;
        p >>= 1;
    }

    return r;
}

void read() { cin >> n >> m >> k; }

void solve() {
    int answer = 0;
    while(n--) {
        int v;
        cin >> v;
        answer += pw(v, m) == 0;
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
