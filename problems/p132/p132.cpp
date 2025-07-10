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

const int INF = 1e9;

int m, n;
vector<vector<char>> cake;

void read() {
    cin >> m >> n;
    cin.ignore();
    cake = vector<vector<char>>(m, vector<char>(n));
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            cin >> cake[i][j];
        }
        cin.ignore();
    }
}

bool check_failure(int i, int j, int mask, int windowSize) {
    if(i * n + j >= windowSize) {
        if(!(mask & (1 << windowSize)) && !(mask & (1 << (windowSize - 1))) &&
           j != n - 1) {
            return true;
        }
        if(!(mask & (1 << windowSize)) && !(mask & (1 << (windowSize - n)))) {
            return true;
        }
    }

    return false;
}

void solve() {
    int windowSize = 2 * n, full_mask = (1 << windowSize) - 1;
    vector<int> curr_dp(1 << windowSize, INF);
    vector<int> next_dp(1 << windowSize, INF);

    curr_dp[0] = 0;
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            next_dp.assign(1 << windowSize, INF);
            for(int mask = 0; mask < (1 << windowSize); mask++) {
                if(curr_dp[mask] == INF) {
                    continue;
                }

                if(cake[i][j] == '*') {
                    int new_mask = mask << 1 | 1;
                    if(!check_failure(i, j, new_mask, windowSize)) {
                        new_mask &= full_mask;
                        next_dp[new_mask] =
                            min(next_dp[new_mask], curr_dp[mask]);
                    }
                } else {
                    // Leave empty
                    {
                        int new_mask = mask << 1;
                        if(!check_failure(i, j, new_mask, windowSize)) {
                            new_mask &= full_mask;
                            next_dp[new_mask] =
                                min(next_dp[new_mask], curr_dp[mask]);
                        }
                    }

                    // Place vertically
                    if(i > 0 && !(mask & (1 << (n - 1)))) {
                        int new_mask = (mask | (1 << (n - 1))) << 1 | 1;
                        if(!check_failure(i, j, new_mask, windowSize)) {
                            new_mask &= full_mask;
                            next_dp[new_mask] =
                                min(next_dp[new_mask], curr_dp[mask] + 1);
                        }
                    }

                    // Place horizontally
                    if(j > 0 && !(mask & 1)) {
                        int new_mask = mask << 1 | 3;
                        if(!check_failure(i, j, new_mask, windowSize)) {
                            new_mask &= full_mask;
                            next_dp[new_mask] =
                                min(next_dp[new_mask], curr_dp[mask] + 1);
                        }
                    }
                }
            }

            curr_dp = std::move(next_dp);
        }
    }

    int result = INF;
    for(int mask = 0; mask < (1 << windowSize); mask++) {
        if(curr_dp[mask] == INF) {
            continue;
        }

        bool possible_tile = false;
        for(int i = 0; i < n; i++) {
            if(m != 1 && !(mask & (1 << i)) && !(mask & (1 << (i + n)))) {
                possible_tile = true;
                break;
            }

            if(i != n - 1 && !(mask & (1 << i)) && !(mask & (1 << (i + 1)))) {
                possible_tile = true;
                break;
            }

            if(i != n - 1 && m != 1 && !(mask & (1 << (i + n))) &&
               !(mask & (1 << (i + n + 1)))) {
                possible_tile = true;
                break;
            }
        }

        if(!possible_tile) {
            result = min(result, curr_dp[mask]);
        }
    }

    assert(result != INF);
    cout << result << endl;
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
