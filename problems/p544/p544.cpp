#include <bits/stdc++.h>
#include <vector>

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

const int mod = (int)1e9 + 9;

void mod_add(int& a, int b) {
    a += b;
    if(a >= mod) {
        a -= mod;
    }
}

int n, k;
vector<int> a, b;

void read() {
    cin >> n >> k;
    a.resize(n);
    b.resize(n);
    cin >> a >> b;
}

void solve() {
    // We want the number of wins by team A to be k more
    // than the number of wins of team B. We know that
    // wins(A) + wins(B) = n, so 2 * wins(B) + k = n, or
    // wins(B) = (n - k) / 2. The constraints in this problem
    // should immediately lead us to think about DP solutions.
    // Before we start with the actual idea, lets sort the individual
    // strengths (the a and b arrays), as we are anyways considering
    // permutations. In particular, we will create a joint sequence
    // of length 2 * n, that will contain all people and sort it. Then
    // we are essentially interested in ways of matching up positions
    // of this joint sequence such that we have exactly wins(B). To do
    // this we will consider the following DP state:
    //
    //    dp[prefix][wins(A) so far][wins(B) so far]
    //      - We are looking at prefix of the corresponding length.
    //      - Team A has claimed wins(A) wins from complete matches.
    //      - Team B has claimed wins(B) wins from complete matches.
    //
    // In particular, when we are at position i, we can either match
    // an existing pair, or start a new one. The state has enough
    // information for us to determine the number of unmatched
    // As and Bs - in particular we have wins(A) + wins(B) matched
    // elements of each A and B, and we know how many As and Bs there
    // are in the current prefix. Note that we multiply by the number of
    // valid candidates only when we "finish" the pair to avoid over counting.
    //
    // This has O(N^3) time complexity, but we can notice that we can only keep
    // two rows in the first dimension.

    int win_b = (n - k);
    if(win_b % 2 == 1) {
        cout << 0 << endl;
        return;
    }

    win_b /= 2;
    int win_a = win_b + k;

    vector<pair<int, int>> elements;
    for(int i = 0; i < n; i++) {
        elements.push_back({a[i], 0});
        elements.push_back({b[i], 1});
    }

    sort(elements.begin(), elements.end());

    int cnt_a = 0, cnt_b = 0;
    vector<vector<int>> dp(win_a + 1, vector<int>(win_b + 1, 0));
    dp[0][0] = 1;

    for(auto [val, type]: elements) {
        vector<vector<int>> new_dp(win_a + 1, vector<int>(win_b + 1, 0));
        for(int curr_win_a = 0; curr_win_a <= win_a; curr_win_a++) {
            for(int curr_win_b = 0; curr_win_b <= win_b; curr_win_b++) {
                if(!dp[curr_win_a][curr_win_b]) {
                    continue;
                }

                mod_add(
                    new_dp[curr_win_a][curr_win_b], dp[curr_win_a][curr_win_b]
                );

                int new_win_a = curr_win_a, new_win_b = curr_win_b;
                int candidates = -(curr_win_a + curr_win_b);
                if(type == 0) {
                    candidates += cnt_b;
                    new_win_a++;
                } else {
                    candidates += cnt_a;
                    new_win_b++;
                }

                if(candidates < 0) {
                    continue;
                }

                if(new_win_a <= win_a && new_win_b <= win_b) {
                    mod_add(
                        new_dp[new_win_a][new_win_b],
                        dp[curr_win_a][curr_win_b] * 1ll * candidates % mod
                    );
                }
            }
        }

        dp = std::move(new_dp);
        if(type == 0) {
            cnt_a++;
        } else {
            cnt_b++;
        }
    }

    cout << dp[win_a][win_b] << endl;
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
