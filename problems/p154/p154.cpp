#include <bits/stdc++.h>
#define endl '\n'

// #pragma GCC optimize ("O3")
// #pragma GCC target ("sse4")

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

int q;

void read() { cin >> q; }

int64_t eval(int64_t x) {
    int64_t ret = 0, c = 0, cc = 1, l = 5;
    while(l <= x) {
        cc++, l *= 5ll;
    }

    while(l > 1) {
        ret += cc * 1ll * ((x / l) - c);
        c += (x / l) - c;
        cc--;
        l /= 5ll;
    }

    return ret;
}

void solve() {
    int64_t low = 1, high = (int64_t)2e12, mid, ret = -1;
    while(low <= high) {
        mid = (low + high) >> 1ll;
        if(eval(mid) >= q) {
            ret = mid, high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if(eval(ret) != q) {
        cout << "No solution." << endl;
    } else {
        cout << ret << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
