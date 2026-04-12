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

int n, m, k;
vector<vector<bool>> symbols;

void read() {
    cin >> n >> m >> k;
    symbols.resize(k);
    for(int i = 0; i < k; i++) {
        symbols[i] = {};
        for(int j = 0; j < n; j++) {
            string s;
            cin >> s;
            for(char c: s) {
                symbols[i].push_back(c - '0');
            }
        }
    }
}

void solve() {
    // For problems like this it's often useful to define what exactly we are
    // looking for. Here we want each of the k symbols to be distinguishable,
    // meaning that for each pair of the C(k, 2), there should be at least one
    // selected pixel where the symbols differ. The key observation is that k <=
    // 6, implies that C(k, 2) <= 15. Therefore, 2^C(k, 2) isn't very large, and
    // we can do some approach that encodes the mask of pairs that are already
    // distinguishable. To finish it off, we can do a DP[position][mask], where
    // position <= n*m is the current pixel we are considering. We have two
    // options - either include the pixel at the current position, meaning that
    // some of the relationships will become "1", or skip it, keeping the
    // current mask. Note that we can precompute for each position, the mask it
    // will "or" / "|", so that each transition is in O(1). Overall, the time
    // complexity would be O(n * m * 2^C(k, 2)) which comfortably passes the
    // given constraints.

    int total = n * m;
    int pairs = 0;
    vector<pair<int, int>> pair_list;
    for(int a = 0; a < k; a++) {
        for(int b = a + 1; b < k; b++) {
            pair_list.push_back({a, b});
            pairs++;
        }
    }

    int full_mask = (1 << pairs) - 1;

    vector<int> pixel_mask(total);
    for(int p = 0; p < total; p++) {
        for(int i = 0; i < pairs; i++) {
            auto [a, b] = pair_list[i];
            if(symbols[a][p] != symbols[b][p]) {
                pixel_mask[p] |= (1 << i);
            }
        }
    }

    vector<vector<int>> dp(total + 1, vector<int>(full_mask + 1, total + 1));
    vector<vector<int>> prev_mask_of(total + 1, vector<int>(full_mask + 1, -1));
    dp[0][0] = 0;

    for(int p = 0; p < total; p++) {
        for(int mask = 0; mask <= full_mask; mask++) {
            if(dp[p][mask] > total) {
                continue;
            }
            if(dp[p + 1][mask] > dp[p][mask]) {
                dp[p + 1][mask] = dp[p][mask];
                prev_mask_of[p + 1][mask] = mask;
            }
            int new_mask = mask | pixel_mask[p];
            if(dp[p + 1][new_mask] > dp[p][mask] + 1) {
                dp[p + 1][new_mask] = dp[p][mask] + 1;
                prev_mask_of[p + 1][new_mask] = mask;
            }
        }
    }

    vector<bool> chosen(total, false);
    int mask = full_mask;
    for(int p = total; p > 0; p--) {
        int pm = prev_mask_of[p][mask];
        if(pm != mask) {
            chosen[p - 1] = true;
        }
        mask = pm;
    }

    cout << dp[total][full_mask] << '\n';
    for(int i = 0; i < total; i++) {
        cout << (chosen[i] ? '1' : '0');
        if((i + 1) % m == 0) {
            cout << '\n';
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
