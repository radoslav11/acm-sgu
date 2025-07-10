#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n, m;
vector<string> tbl;

void read() {
    cin >> n >> m;
    tbl.resize(n);
    cin >> tbl;
}

void solve() {
    vector<int> mask(1 << n, 0);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(tbl[i][j] == '*') {
                mask[1 << i] |= 1 << j;
            }
        }
    }

    for(int i = 1; i < (1 << n); i++) {
        int l = i & -i;
        mask[i] = mask[l] | mask[i ^ l];
    }

    int ans = n + m;
    for(int i = 0; i < (1 << n); i++) {
        int mask_m = mask[((1 << n) - 1) ^ i];
        ans = min(ans, max(__builtin_popcount(i), __builtin_popcount(mask_m)));
    }

    cout << ans << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
