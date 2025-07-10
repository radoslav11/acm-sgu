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

int64_t C[MAXN];

void solve() {
    C[0] = 1;
    for(int i = 1; i <= n; i++) {
        for(int j = 0; j < i; j++) {
            C[i] += C[j] * C[i - j - 1];
        }
    }

    cout << C[n] << " " << n + 1 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
