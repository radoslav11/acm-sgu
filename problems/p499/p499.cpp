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
bool used[MAXN];

void read() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        int x;
        cin >> x;
        used[x] = 1;
    }
}

void solve() {
    int answer = 1;
    for(int i = 1; i < MAXN; i++) {
        int cnt = 0;
        for(int j = i; j < MAXN; j += i) {
            cnt += used[j];
        }

        if(cnt >= 2) {
            chkmax(answer, i);
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
