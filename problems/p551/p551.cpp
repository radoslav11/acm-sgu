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

int n, t1, t2;

void read() { cin >> n >> t1 >> t2; }

int eval(int x) { return x / t1 + x / t2; }

void solve() {
    int low = 0, high = 1e8, mid, ret;
    while(low <= high) {
        mid = (low + high) >> 1;
        if(eval(mid) >= n) {
            ret = mid, high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    int R1 = ret, bg = ret, ret2 = eval(ret);
    if(bg % t1 != 0) {
        chkmax(R1, bg + t1 - (bg % t1)), ret2++;
    }
    if(bg % t2 != 0) {
        chkmax(R1, bg + t2 - (bg % t2)), ret2++;
    }

    cout << ret2 << " " << R1 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
