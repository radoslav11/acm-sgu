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
int a[MAXN];

void read() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        cin >> a[i];
    }
}

void solve() {
    for(int i = 0; i < n; i++) {
        int c = 0;
        for(int d = 2; d * 1ll * d <= a[i]; d++) {
            while(a[i] % d == 0) {
                c++;
                a[i] /= d;
            }
        }

        if(a[i] != 1) {
            c++;
        }

        if(c == 2) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
