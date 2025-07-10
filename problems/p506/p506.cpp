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

int n, m;
string s, t;

void read() {
    cin >> s >> t;
    n = s.size();
    m = t.size();
}

int nxt[MAXN][26];

void solve() {
    for(int i = 0; i < 26; i++) {
        nxt[n][i] = n;
    }

    for(int i = n - 1; i >= 0; i--) {
        for(int c = 0; c < 26; c++) {
            nxt[i][c] = nxt[i + 1][c];
        }
        nxt[i][s[i] - 'a'] = i;
    }

    int64_t answer = 0;
    for(int i = 0; i < n; i++) {
        int en = i - 1;
        for(char c: t) {
            en = nxt[en + 1][c - 'a'];
            if(en == n) {
                break;
            }
        }

        answer += n - en;
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
