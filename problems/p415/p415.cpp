#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n, x;
vector<int> a;

void read() {
    cin >> n >> x;
    a.resize(n);
    cin >> a;
}

const int MAXLEN = (int)1e4 + 42;

template<int bit_len = 1>
void solve(int _x) {
    if(_x > bit_len) {
        solve<std::min(bit_len * 2, MAXLEN)>(_x);
        return;
    }

    vector<bitset<bit_len>> dp_prev(n + 2);
    vector<bitset<bit_len>> dp_suff(n + 2);
    
    dp_prev[0][0] = 1;
    dp_suff[n + 1][0] = 1;
    for(int i = 1; i <= n; i++) {
        dp_prev[i] = dp_prev[i - 1] | (dp_prev[i - 1] << a[i - 1]);
    }

    for(int i = n; i >= 1; i--) {
        dp_suff[i] = dp_suff[i + 1] | (dp_suff[i + 1] << a[i - 1]);
    }

    vector<int> ans;
    for(int i = 1; i <= n; i++) {
        bool ok = false;
        for(int j = 0; j <= x; j++) {
            if(dp_prev[i - 1][j] && dp_suff[i + 1][_x - j]) {
                ok = true;
                break;
            }
        }

        if(!ok) {
            ans.push_back(a[i - 1]);
        }
    }

    cout << ans.size() << '\n';
    cout << ans << '\n';
}

void solve() { solve(x); }

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
