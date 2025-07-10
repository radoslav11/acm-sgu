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

int64_t n;

void read() { cin >> n; }

void solve() {
    int64_t answer = n / 3;
    answer *= 2;
    if(n % 3 == 2) {
        answer++;
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
