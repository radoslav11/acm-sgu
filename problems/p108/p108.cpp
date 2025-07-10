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

int n, k;
vector<int> a;

void read() {
    cin >> n >> k;
    a.resize(k);
    cin >> a;
}

const int MAXLEN = (int)1e7 + 42;

template<int len = 1>
void solve_fixed_len() {
    if(len <= n) {
        solve_fixed_len<min(len * 2, MAXLEN)>();
        return;
    }

    function<int(int)> nxt = [&](int x) {
        int res = x;
        while(x) {
            res += x % 10;
            x /= 10;
        }
        return res;
    };

    bitset<len> dp;
    bitset<len> useful_indices;
    for(int i: a) {
        useful_indices[i] = true;
    }

    for(int i = 1; i <= n; i++) {
        int nxt_i = nxt(i);
        if(nxt_i >= len) {
            continue;
        }
        dp[nxt_i] = true;
    }

    vector<int> ans;

    vector<int> compressed = a;
    sort(compressed.begin(), compressed.end());
    compressed.erase(
        unique(compressed.begin(), compressed.end()), compressed.end()
    );

    int cnt = 0;
    for(int i = 1; i <= n; i++) {
        if(!dp[i]) {
            cnt++;
            if(useful_indices[cnt]) {
                ans.push_back(i);
            }
        }
    }

    cout << cnt << endl;
    for(int i: a) {
        int real_i = lower_bound(compressed.begin(), compressed.end(), i) -
                     compressed.begin();
        cout << ans[real_i] << ' ';
    }
    cout << endl;
}

void solve() { solve_fixed_len(); }

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
