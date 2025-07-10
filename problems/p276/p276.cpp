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

double s, t;

void read() {
    cin >> s >> t;
    t -= s;
    t /= 60.0;
}

void solve() {
    int ans = 4;
    if(t < 30) {
        ans = 3;
    }
    if(t < 15) {
        ans = 2;
    }
    if(t < 5) {
        ans = 1;
    }
    if(t <= 0) {
        ans = 0;
    }
    cout << ans << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
