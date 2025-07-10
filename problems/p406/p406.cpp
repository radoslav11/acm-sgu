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
map<int, int> li[MAXN];
vector<int> memo[MAXN];

void read() {
    cin >> n >> m;
    for(int i = 1; i <= n; i++) {
        int c;
        cin >> c;
        while(c--) {
            int x;
            cin >> x;
            li[i][x] = 1;
            memo[i].push_back(x);
        }
    }
}

void solve() {
    while(m--) {
        vector<int> L;
        int x, c;
        cin >> c;

        while(c--) {
            cin >> x;
            L.push_back(x);
        }

        int cnt = 0;
        vector<int> answer;
        for(int i = 1; i <= n; i++) {
            bool ok = 1;
            for(int j = 0; j < L.size(); j++) {
                if(L[j] > 0 && !li[i].count(L[j])) {
                    ok = 0;
                } else if(L[j] < 0 && li[i].count(-L[j])) {
                    ok = 0;
                }
            }

            if(ok) {
                cnt += memo[i].size(), answer.push_back(i);
            }
        }

        cout << answer.size() << endl;
        for(int i = 0; i < answer.size(); i++) {
            cout << memo[answer[i]].size() << " ";
            for(int v = 0; v < memo[answer[i]].size(); v++) {
                cout << memo[answer[i]][v] << " ";
            }
            cout << endl;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
