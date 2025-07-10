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
const int MAXN = (1 << 16);

int n;

void read() { cin >> n; }

void solve() {
    if(n < 9) {
        cout << 0 << endl;
    } else if(n == 9) {
        cout << 8 << endl;
    } else {
        cout << 72;
        n -= 10;
        while(n--) {
            cout << 0;
        }
        cout << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
