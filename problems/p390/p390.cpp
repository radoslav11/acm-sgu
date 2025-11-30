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

const int POS = 19;
const int SUM_MAX = 171;

int64_t L, R;
int K;

vector<int> dig_l(POS), dig_r(POS);
vector<vector<vector<pair<int64_t, int>>>> dp;

pair<int64_t, int> dfs(
    int pos, int carry, int sum_dig, bool tight_low, bool tight_high
) {
    if(pos == -1) {
        if(carry + sum_dig >= K) {
            return {1, 0};
        }
        return {0, carry + sum_dig};
    }
    if(!tight_low && !tight_high) {
        auto& cell = dp[pos][sum_dig][carry];
        if(cell.first != -1) {
            return cell;
        }
        pair<int64_t, int> res = {0, carry};
        for(int d = 0; d <= 9; d++) {
            auto tmp = dfs(pos - 1, res.second, sum_dig + d, false, false);
            res.first += tmp.first;
            res.second = tmp.second;
        }
        cell = res;
        return cell;
    } else {
        pair<int64_t, int> res = {0, carry};
        int lo = tight_low ? dig_l[pos] : 0;
        int hi = tight_high ? dig_r[pos] : 9;
        for(int d = lo; d <= hi; d++) {
            bool nL = tight_low && (d == lo);
            bool nH = tight_high && (d == hi);
            auto tmp = dfs(pos - 1, res.second, sum_dig + d, nL, nH);
            res.first += tmp.first;
            res.second = tmp.second;
        }
        return res;
    }
}

void prepare(int64_t x, vector<int>& d) {
    string s = to_string(x);
    reverse(s.begin(), s.end());
    d.assign(POS, 0);
    for(int i = 0; i < (int)s.size() && i < POS; i++) {
        d[i] = s[i] - '0';
    }
}

void read() { cin >> L >> R >> K; }

void solve() {
    // The immediate solution of doing floor(total_digit_sum/K) is unfortunately
    // incorrect as the remainders get discarded at passenger boundaries.
    // The right way to solve this is to use digit DP. We will simulate the
    // greedy process over L..R with digit DP: walk digits from MSB to LSB with
    // maintaining the K we have so far, and how much the digits we have seen so
    // far contribute to the digit sum. In a terminating state, we decide based
    // on whether the carry + digit_sum >= k. If it is, we zero out the carry
    // and add 1 to the answer, otherwise we have a new carry. This essentially
    // represent an automaton that we can memoize over. We just need to keep a
    // pair with the carry we are ending at, and the actual answer.

    prepare(L, dig_l);
    prepare(R, dig_r);
    dp.assign(
        POS, vector<vector<pair<int64_t, int>>>(
                 SUM_MAX + 1, vector<pair<int64_t, int>>(K + 1, {-1, -1})
             )
    );
    auto ans = dfs(POS - 1, 0, 0, true, true);
    cout << ans.first << '\n';
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
