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

int m, n;
int B[MAXN], a0;

/// 2 * m = (n - 1) * n
/// n * n - n - 2 * m = 0
/// D = 1 + 8 * m
/// n = (1 + sqrt(1 + 8 * m)) / 2

void read() {
    cin >> m;
    for(int i = 0; i < m; i++) {
        cin >> B[i];
    }
    n = (1 + sqrt(1 + 8 * m)) / 2;
}

map<int, bool> L, O;

void solve() {
    for(int i = 0; i < m; i++) {
        O[B[i]] = 1;
    }

    vector<int> ans, pedo;
    ans.push_back(0);
    pedo.push_back(0);
    L[0] = 1;

    for(int i = 0; i < m; i++) {
        if(!L.count(B[i])) {
            bool ok = 1;
            for(int x = 0; x < ans.size(); x++) {
                if(!O.count(B[i] ^ ans[x])) {
                    ok = 0;
                }
            }

            if(ok) {
                for(int x = 0, SZ = pedo.size(); x < SZ; x++) {
                    L[B[i] ^ pedo[x]] = 1, pedo.push_back(B[i] ^ pedo[x]);
                }
                ans.push_back(B[i]);
            }
        }
    }

    for(int i = 0; i < ans.size(); i++) {
        cout << ans[i] << " ";
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
