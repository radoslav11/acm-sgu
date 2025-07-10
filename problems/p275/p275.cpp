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

int n;
int64_t base[64];

void add(int64_t x) {
    for(int64_t l = 60; l >= 0; l--) {
        if((x & (1ll << l)) && base[l] == 0) {
            base[l] = x;
            return;
        } else if(x & (1ll << l)) {
            x ^= base[l];
        }
    }
}

int64_t max_xor() {
    int64_t x = 0;
    for(int64_t l = 60; l >= 0; l--) {
        if(!(x & (1ll << l))) {
            x ^= base[l];
        }
    }

    return x;
}

void read() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        int64_t x;
        cin >> x;
        add(x);
    }
}

void solve() { cout << max_xor() << endl; }

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
