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
const int MAXN = (1 << 10);

int n;

void read() { cin >> n; }

int64_t f[MAXN], sum = 0;

void solve() {
    f[0] = f[1] = f[2] = 1;
    for(int i = 3; i <= n; i++) {
        f[i] = f[i - 1] + f[i - 2], sum += f[i];
    }

    if(n >= 1) {
        sum += 1;
    }
    if(n >= 2) {
        sum += 1;
    }

    cout << sum << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
