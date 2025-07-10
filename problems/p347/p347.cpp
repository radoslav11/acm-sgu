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
string s[MAXN];

void read() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        cin >> s[i];
    }
}

bool cmp(string a, string b) { return a + b < b + a; }

void solve() {
    sort(s, s + n, cmp);
    for(int i = 0; i < n; i++) {
        cout << s[i];
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
