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

void read() { cin >> n >> k; }

vector<vector<vector<vector<int64_t>>>> dp;

int64_t rec(int mask, int i, int j, int k) {
    if(k < 0) {
        return 0;
    }

    if(i == n - 1 && j == n) {
        return k == 0;
    }

    int64_t &memo = dp[mask][i][j][k];
    if(memo != -1) {
        return memo;
    }

    if(j == n) {
        return memo = rec(mask, i + 1, 0, k);
    }

    memo = rec((mask << 1) & ((2 << n) - 1), i, j + 1, k);

    int bit_left = (mask & 1), bit_up = (mask >> (n - 1)) & 1,
        bit_diag_left = (mask >> n) & 1,
        bit_diag_right = (mask >> (n - 2)) & 1;

    if(i == 0) {
        bit_up = 0;
        bit_diag_left = 0;
        bit_diag_right = 0;
    }

    if(j == 0) {
        bit_left = 0;
        bit_diag_left = 0;
    }

    if(j == n - 1) {
        bit_diag_right = 0;
    }

    if(bit_left == 0 && bit_up == 0 && bit_diag_left == 0 &&
       bit_diag_right == 0) {
        memo += rec(((mask << 1) | 1) & ((2 << n) - 1), i, j + 1, k - 1);
    }

    return memo;
}

void solve() {
    dp = vector<vector<vector<vector<int64_t>>>>(
        2 << n, vector<vector<vector<int64_t>>>(
                           n, vector<vector<int64_t>>(n + 1, vector<int64_t>(k + 1, -1))));

    cout << rec(0, 0, 0, k) << '\n';
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
