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

int f(int x) {
    int r = 0;
    while(x) {
        r += x % 10;
        x /= 10;
    }

    return r;
}

int g(int x) {
    if(x <= 9) {
        return x;
    }
    return g(f(x));
}

int a[MAXN];

void read() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        cin >> a[i];
    }
}

void solve() {
    if(a[0] == 0) {
        cout << 0 << endl;
        return;
    }

    int ans = 0, s = 1;
    for(int i = 0; i < n; i++) {
        a[i] %= 9;
        s = s * a[i] % 9;
        ans = (ans + s) % 9;
    }

    if(ans == 0) {
        cout << 9 << endl;
    } else {
        cout << ans << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K;
    cin >> K;

    while(K--) {
        read();
        solve();
    }

    return 0;
}
