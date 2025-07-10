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
int a[MAXN + 42];

void read() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        cin >> a[i];
    }
}

vector<int> li;

int sz;
int tr[MAXN + 42];

void init() {
    sz = li.size() + 1;
    memset(tr, 0, sizeof(tr));
}

void add(int x) {
    for(; x <= sz; x += (x & -x)) {
        tr[x]++;
    }
}

int query(int x) {
    int ret = 0;
    for(; x > 0; x -= (x & -x)) {
        ret += tr[x];
    }

    return ret;
}

void solve() {
    for(int i = 0; i < n; i++) {
        li.push_back(a[i]);
    }

    sort(li.begin(), li.end());
    li.erase(unique(li.begin(), li.end()), li.end());

    for(int i = 0; i < n; i++) {
        a[i] = lower_bound(li.begin(), li.end(), a[i]) - li.begin() + 1;
    }

    init();

    uint32_t answer = 0;
    for(int i = 0; i < n; i++) {
        add(a[i]);
        answer += i + 1 - query(a[i]);
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
