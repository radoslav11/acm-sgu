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
pair<int, int> a[MAXN];

void read() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        cin >> a[i].first >> a[i].second;
    }
}

void solve() {
    sort(a, a + n);
    int answer = 0;
    int mx = -1e9;

    for(int i = 0; i < n; i++) {
        int j = i;
        while(j < n && a[i].first == a[j].first) {
            j++;
        }

        for(int o = i; o < j; o++) {
            answer += a[o].second < mx;
        }

        for(int o = i; o < j; o++) {
            chkmax(mx, a[o].second);
        }
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
