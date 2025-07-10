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
const int MAXN = (1 << 20);

int64_t a1, b1, c1;
int64_t a2, b2, c2;

void read() {
    cin >> a1 >> b1 >> c1;
    cin >> a2 >> b2 >> c2;
}

void solve() {
    int64_t answer = a1 / a2;
    chkmin(answer, c1 / c2);
    chkmin(answer, b1 / b2);
    cout << answer << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
