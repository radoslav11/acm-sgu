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

const int inf = (int)1e9 + 42;

int n;
string s;

void read() {
    cin >> n;
    cin >> s;
}

void solve() {
    // The problem fundamentally isn't hard - we can use dynamic programming
    // that tracks the position of the two legs as part of the state, if they
    // pressed the buttons, and where in the beat are we currently at. The main
    // part is all the details about the transitions. One easy to miss detail is
    // that we could make some moves even during a 'N' character.

    map<char, int> char_to_idx = {{'L', 0}, {'U', 1}, {'R', 2}, {'D', 3}};
    vector<char> idx_to_char = {'L', 'U', 'R', 'D'};

    vector<vector<vector<vector<int>>>> dp(
        n + 1, vector<vector<vector<int>>>(
                   4, vector<vector<int>>(4, vector<int>(4, inf))
               )
    );

    struct State {
        int left, right, last_pressed;
    };
    vector<vector<vector<vector<State>>>> parent(
        n + 1, vector<vector<vector<State>>>(
                   4, vector<vector<State>>(4, vector<State>(4, {-1, -1, -1}))
               )
    );

    dp[0][0][2][2] = 0;

    for(int i = 0; i < n; i++) {
        char req_char = s[i];
        int req = (req_char == 'N') ? -1 : char_to_idx[req_char];

        for(int left = 0; left < 4; left++) {
            for(int right = 0; right < 4; right++) {
                if(left == right) {
                    continue;
                }

                for(int last = 0; last < 4; last++) {
                    if(dp[i][left][right][last] == inf) {
                        continue;
                    }

                    int cur_cost = dp[i][left][right][last];

                    auto update = [&](int cost, int new_left, int new_right,
                                      int new_last) {
                        int new_cost = cur_cost + cost;
                        if(new_cost <
                           dp[i + 1][new_left][new_right][new_last]) {
                            dp[i + 1][new_left][new_right][new_last] = new_cost;
                            parent[i + 1][new_left][new_right][new_last] = {
                                left, right, last
                            };
                        }
                    };

                    if(req == -1) {
                        update(0, left, right, 2);
                    }
                    if(req == -1 || left == req) {
                        update(1, left, right, 0);
                    }
                    if(req == -1 || right == req) {
                        update(1, left, right, 1);
                    }

                    for(int new_pos = 0; new_pos < 4; new_pos++) {
                        if(new_pos == right) {
                            continue;
                        }
                        if(new_pos == 2 && right == 0) {
                            continue;
                        }
                        if(req != -1 && new_pos != req) {
                            continue;
                        }
                        update(
                            (last == 0 || last == 3) ? 9 : 3, new_pos, right, 0
                        );
                    }

                    for(int new_pos = 0; new_pos < 4; new_pos++) {
                        if(new_pos == left) {
                            continue;
                        }
                        if(left == 2 && new_pos == 0) {
                            continue;
                        }
                        if(req != -1 && new_pos != req) {
                            continue;
                        }
                        update(
                            (last == 1 || last == 3) ? 9 : 3, left, new_pos, 1
                        );
                    }

                    for(int new_left = 0; new_left < 4; new_left++) {
                        for(int new_right = 0; new_right < 4; new_right++) {
                            if(new_left == new_right) {
                                continue;
                            }
                            if(new_left == 2 && new_right == 0) {
                                continue;
                            }
                            if(req != -1 && new_left != req &&
                               new_right != req) {
                                continue;
                            }
                            update(10, new_left, new_right, 3);
                        }
                    }
                }
            }
        }
    }

    int best_cost = inf;
    int best_left = -1, best_right = -1, best_last = -1;
    for(int left = 0; left < 4; left++) {
        for(int right = 0; right < 4; right++) {
            for(int last = 0; last < 4; last++) {
                if(dp[n][left][right][last] < best_cost) {
                    best_cost = dp[n][left][right][last];
                    best_left = left;
                    best_right = right;
                    best_last = last;
                }
            }
        }
    }

    cout << best_cost << '\n';

    vector<pair<int, int>> path;
    int cur_left = best_left, cur_right = best_right, cur_last = best_last;
    for(int i = n; i > 0; i--) {
        path.push_back({cur_left, cur_right});
        State prev = parent[i][cur_left][cur_right][cur_last];
        cur_left = prev.left;
        cur_right = prev.right;
        cur_last = prev.last_pressed;
    }

    reverse(path.begin(), path.end());

    for(auto [left, right]: path) {
        cout << idx_to_char[left] << idx_to_char[right] << '\n';
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
