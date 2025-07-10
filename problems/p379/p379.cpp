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

int n, c, p, t;
int cnt[MAXN];

void read() {
    cin >> n >> c >> p >> t;
    for(int i = 0; i < n; i++) {
        cin >> cnt[i];
    }
}

bool check(int x) {
    vector<int> li;
    for(int i = 0; i < n; i++) {
        li.push_back(min(x, cnt[i]));
        x -= cnt[i];
        if(x <= 0) {
            break;
        }
    }

    if(x > 0) {
        return false;
    }

    int64_t ret = 0;
    for(int i = 0; i < (int)li.size(); i++) {
        int whole = li[i] / c;
        if(whole * 1ll * p > t) {
            return false;
        }
        ret += whole * 1ll * (i + 1) * 2ll * p;
        if(ret > t) {
            return false;
        }
        li[i] %= c;
    }

    while(!li.empty() && li.back() == 0) {
        li.pop_back();
    }

    while(!li.empty()) {
        ret += li.size() * 1ll * p * 2ll;
        if(ret > t) {
            return false;
        }

        int cap = 0;
        while(!li.empty()) {
            int curr = min(c - cap, li.back());
            cap += curr;

            if(curr == li.back()) {
                li.pop_back();
            } else {
                li[li.size() - 1] -= curr;
                break;
            }
        }
    }

    return ret <= t;
}

void solve() {
    int low = 1, high = (int)1e9, mid, ret = 0;

    while(low <= high) {
        mid = (low + high) >> 1;
        if(check(mid)) {
            ret = mid, low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    cout << ret << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
 
