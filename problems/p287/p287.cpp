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

int64_t q;
int c;

void read() { cin >> q >> c; }

void solve() {
    // Every answer arrives c queries late, so when we decide q_i we have
    // only seen the responses to q_1, ..., q_{i-c-1}. Out of the n queries
    // we make, only q_1, ..., q_{n-c} can ever yield "Correct!" within n
    // steps; the last c queries are pure padding spent waiting for the
    // answer to q_{n-c} to come back. In particular q_1, ..., q_{c+1} are
    // all committed completely blindly, and q_{c+2} is the first query
    // informed by q_1's outcome.
    //
    // Let f(n, c) be the largest range we can guarantee to crack in n
    // queries with delay c. We pick q_1 = m as the splitter and place the
    // c blind followups q_2, ..., q_{c+1} strictly below m. Once q_1's
    // answer arrives the game branches:
    //
    //   - In the "less" branch (target < m) the suffix q_2, ..., q_n is
    //     itself a delay-c game of length n-1 on the lower segment. The c
    //     values we already committed there are exactly the first c entries
    //     of that sub-game's blind prefix, and q_{c+2} closes the prefix
    //     once the branch is known. So this side covers up to f(n-1, c).
    //
    //   - In the "greater" branch (target > m) the c values placed below m
    //     are useless, as their answers will always be "less than", so only
    //     the fresh queries q_{c+2}, ..., q_n contribute. They form a clean
    //     delay-c game of length n-c-1, covering up to f(n-c-1, c).
    //
    // Any other split of the c blind followups between the two sides only
    // shrinks the achievable range, so the bound is tight:
    //
    //     f(n, c) = f(n-1, c) + f(n-c-1, c) + 1,    f(n, c) = 0 for n <= c.
    //
    // For c = 0 this collapses to f(n, 0) = 2^n - 1, the usual trichotomic
    // search. For c >= 1 the dominant root of x^{c+1} = x^c + 1 sits just
    // above 1, so the answer can grow into the millions for q = 10^15 and
    // c = 10^6. We only ever need the last c+1 entries of f, so a circular
    // buffer of size c+2 is enough. We unroll the recurrence and stop the
    // moment the running value reaches q.
    //
    // This problem is commonly known as delayed binary search, and there
    // are a fair bit of results in academia. One example is:
    //
    //     https://www.researchgate.net/publication/257428752_Delayed_
    //             Binary_Search_or_Playing_Twenty_Questions_with_a_
    //             Procrastinator

    int sz = c + 2;
    vector<int64_t> dp(sz, 0);
    for(int64_t n = 1;; n++) {
        int64_t val = 0;
        if(n > c) {
            val = dp[(n - 1) % sz] + dp[(n - c - 1) % sz] + 1;
        }
        dp[n % sz] = val;
        if(val >= q) {
            cout << n << '\n';
            return;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
