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

const int MAXV = 50;
const int OFFSET = MAXV;
const int MAX_SIZE = 2 * OFFSET + 1;
const int INF = (int)1e9 + 42;

struct State {
    bool possible = false;
    int sum = 0;
    vector<int> indices;
};

int n, k;
vector<pair<int, int>> ab;

void read() {
    cin >> n >> k;
    ab.resize(n);
    cin >> ab;
}

void solve() {
    // There is a direct DP solution with state dp[pos][balance][num_selected],
    // where the balance is O(K*MAXV), num_selected is O(K), and pos is O(N). At
    // every step we have 2 transitions - either select, or not. The overall
    // complexity is then O(N K^2 MAXV). This is a bit too slow, as MAXV is ~50,
    // K is 20, and N is around 60000. However, we can try speeding it up with
    // one key observation: N is too large for K = 20. We can group the numbers
    // into groups by A-B, and notice that we don't need to keep more than O(K)
    // items in each group - particularly the ones with largest value of A+B.
    // There are only O(MAXV) such groups, and this reduces N to O(K MAXV),
    // making the full complexity O(K^3 MAXV^2). This should be fast enough to
    // pass, although we might have to be a bit careful about the memory as we
    // do want to recover the solution.

    const int offset = OFFSET;
    const int max_diff = MAX_SIZE;
    const int max_sum = k * MAX_SIZE;

    vector<vector<pair<int, int>>> groups(max_diff);

    for(int i = 0; i < n; i++) {
        int a = ab[i].first;
        int b = ab[i].second;
        int diff = a - b + offset;
        int sum = a + b;

        groups[diff].push_back({sum, i + 1});
    }

    for(int i = 0; i < max_diff; i++) {
        sort(groups[i].begin(), groups[i].end(), greater<pair<int, int>>());
        if((int)groups[i].size() > k) {
            groups[i].resize(k);
        }
    }

    vector<vector<State>> dp(k, vector<State>(max_sum));

    for(int diff = 0; diff < max_diff; diff++) {
        for(auto [sum, idx]: groups[diff]) {
            for(int cnt = k - 2; cnt >= 0; cnt--) {
                for(int s = 0; s + diff <= max_sum - 1; s++) {
                    if(dp[cnt][s].possible &&
                       (!dp[cnt + 1][s + diff].possible ||
                        dp[cnt + 1][s + diff].sum < dp[cnt][s].sum + sum)) {
                        dp[cnt + 1][s + diff].possible = true;
                        dp[cnt + 1][s + diff].sum = dp[cnt][s].sum + sum;
                        dp[cnt + 1][s + diff].indices = dp[cnt][s].indices;
                        dp[cnt + 1][s + diff].indices.push_back(idx);
                    }
                }
            }

            if(diff < max_sum &&
               (!dp[0][diff].possible || dp[0][diff].sum < sum)) {
                dp[0][diff].possible = true;
                dp[0][diff].sum = sum;
                dp[0][diff].indices = {idx};
            }
        }
    }

    int best_diff = -1;
    int best_sum = -1;
    int min_balance = INF;

    for(int s = 0; s < max_sum; s++) {
        if(dp[k - 1][s].possible) {
            int balance = abs(s - offset * k);
            if(balance < min_balance ||
               (balance == min_balance && dp[k - 1][s].sum > best_sum)) {
                min_balance = balance;
                best_diff = s;
                best_sum = dp[k - 1][s].sum;
            }
        }
    }

    int a_total = (best_sum + best_diff - offset * k) / 2;
    int b_total = (best_sum - best_diff + offset * k) / 2;

    cout << a_total << ' ' << b_total << '\n';

    vector<int> result = dp[k - 1][best_diff].indices;
    sort(result.begin(), result.end());
    cout << result << '\n';
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
