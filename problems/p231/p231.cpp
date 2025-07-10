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
bitset<MAXN> sieve;

void read() {
    cin >> n;
    sieve[0] = sieve[1] = 1;
    for(int x = 2; x <= n; x++) {
        for(int y = 2 * x; y <= n; y += x) {
            sieve[y] = 1;
        }
    }
}

void solve() {
    vector<pair<int, int> > ans;
    for(int i = 1; i + 2 <= n; i++) {
        if(!sieve[i] && !sieve[i + 2]) {
            ans.push_back(make_pair(2, i));
        }
    }

    cout << ans.size() << endl;
    for(int i = 0; i < ans.size(); i++) {
        cout << ans[i].first << " " << ans[i].second << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
