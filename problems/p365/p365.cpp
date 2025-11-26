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

int S;

void read() { cin >> S; }

vector<vector<vector<int64_t>>> dp;

int64_t rec(int pos, int state, int last_digit) {
    if(pos == S) {
        return 1;
    }

    int64_t& memo = dp[pos][state][last_digit];
    if(memo != -1) {
        return memo;
    }

    memo = 0;

    if(state == 0) {
        for(int d = last_digit; d <= 9; d++) {
            memo += rec(pos + 1, 0, d);
        }
        for(int d = 0; d < last_digit; d++) {
            memo += rec(pos + 1, 1, d);
        }
    } else {
        for(int d = 0; d <= last_digit; d++) {
            memo += rec(pos + 1, 1, d);
        }
    }

    return memo;
}

void solve() {
    // This is a classical problem on digit DP. The state we want to keep is:
    //    dp[pos][state][last_digit].
    //        - pos is the position, starting from S - 1.
    //        - state = 0 means that we are building the increasing prefix,
    //          while state = 1 means that we are building the decreasing. We
    //          start from 0 state.
    //        - last_digit is the last digit we placed (0 at the beginning).

    dp.assign(S, vector<vector<int64_t>>(2, vector<int64_t>(10, -1)));
    cout << rec(0, 0, 0) << '\n';
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
