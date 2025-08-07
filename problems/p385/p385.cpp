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

int n;

void read() { cin >> n; }

void solve() {
    // Let's consider judge i has card p[i], or in other words we have a
    // permutation p[1], ..., p[n]. We can notice that in the end of the game,
    // each cycles in this permutation will be compressed into a single judge.
    // Then a judge can win if and only if it's part of a largest cycle. In
    // other words we are looking for E[positions i which are part of a largest
    // cycle], over all permutations of length n. One important constraint is
    // that we can't have cycles of length 1 - this is because a person can't
    // start with its own card. We will do this by counting all permutations but
    // never allowing size of a cycle = 1, and then re-normalizing the
    // probabilities.
    //
    // To solve this we can try dp[length][max_cycle_length][cnt], representing
    // the probability to have a permutation of given length that has cnt cycles
    // of max_cycle_length, and no cycles larger than max_cycle_length. This is
    // O(n^2 log n) states, because cnt * max_cycle_length <= length and the
    // harmonic sequence. The trick to avoid over-counting is to always select
    // the cycle of p[1] in the transitions.
    //
    // The transitions are O(n), so the total complexity is O(n^3 log n) which
    // is fast enough for n <= 100.

    vector<vector<vector<double>>> dp(n + 1, vector<vector<double>>(n + 1));
    for(int length = 0; length <= n; length++) {
        for(int max_cycle_length = 1; max_cycle_length <= n;
            max_cycle_length++) {
            int max_cnt = length / max_cycle_length;
            dp[length][max_cycle_length].assign(max_cnt + 1, 0);
        }
    }

    for(int i = 1; i <= n; i++) {
        dp[0][i][0] = 1;
    }

    for(int length = 1; length <= n; length++) {
        for(int max_cycle_length = 1; max_cycle_length <= n;
            max_cycle_length++) {
            int max_cnt = length / max_cycle_length;
            for(int cnt = 0; cnt <= max_cnt; cnt++) {
                // Look at cycle of p[1], and select the size.
                // There are C(length - 1, size - 1) ways to select the other
                // nodes in this cycle, and then we have (size-1)! permutations
                // that are valid. There are also length! permutations but
                // 1/(length-size)! is already present in dp[length - size].
                //
                // Therefore the transition probability will be:
                //
                //     (size - 1)! * C(length - 1, size - 1) / (length! /
                //     (length-size)!)
                //  =
                //     (size - 1)! * (length - 1)! * (length - size)! / ( (size
                //     - 1)! * (length - 1 - size + 1)! * length!)
                //  =
                //     1 / length

                for(int size = 2; size <= min(max_cycle_length, length);
                    size++) {
                    int new_cnt = size == max_cycle_length ? cnt - 1 : cnt;

                    if(new_cnt >= 0 &&
                       new_cnt <
                           (int)dp[length - size][max_cycle_length].size()) {
                        dp[length][max_cycle_length][cnt] +=
                            dp[length - size][max_cycle_length][new_cnt] /
                            length;
                    }
                }
            }
        }
    }

    // We didn't allow cycle length equal to 1.0, so we need to renormalize.
    double Z = 0;
    for(int max_cycle_length = 2; max_cycle_length <= n; max_cycle_length++) {
        int max_cnt = n / max_cycle_length;
        for(int cnt = 1; cnt <= max_cnt; cnt++) {
            Z += dp[n][max_cycle_length][cnt];
        }
    }

    double ans = 0;
    for(int max_cycle_length = 2; max_cycle_length <= n; max_cycle_length++) {
        int max_cnt = n / max_cycle_length;
        for(int cnt = 1; cnt <= max_cnt; cnt++) {
            ans += dp[n][max_cycle_length][cnt] / Z * cnt * max_cycle_length;
        }
    }

    cout << setprecision(9) << fixed << ans << endl;
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

/*
2 3 1 -> 3             -> 3 / 2
3 1 2 -> 3             -> 3 / 2

Total = 2.5
*/
