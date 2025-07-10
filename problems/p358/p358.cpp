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

int a[3][3];

void read() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            cin >> a[i][j];
        }
    }
}

void solve() {
    for(int i = 0; i < 3; i++) {
        sort(a[i], a[i] + 3);
    }

    vector<int> li;
    li.push_back(a[0][1]);
    li.push_back(a[1][1]);
    li.push_back(a[2][1]);
    sort(li.begin(), li.end());

    cout << li[1] << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
