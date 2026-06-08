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

int rule_mask[4][4];
string row;

int color_id(char c) { return c == 'b' ? 0 : c == 'r' ? 1 : c == 'y' ? 2 : 3; }

void read() {
    int cnt[4];
    for(int c = 0; c < 4; c++) {
        cin >> cnt[c];
    }

    for(int c = 0; c < 4; c++) {
        for(int i = 0; i < cnt[c]; i++) {
            string rule;
            cin >> rule;
            rule_mask[color_id(rule[0])][color_id(rule[1])] |= 1 << c;
        }
    }

    cin >> row;
}

void solve() {
    // dp[l][r] is a 4-bit mask whose set bits are the colors that the segment
    // row[l..r] can be collapsed into as a single stone. Any collapse of a
    // segment ends with one final merge of two adjacent stones, and those two
    // stones are the independent collapses of a prefix row[l..mid] into some
    // color a and a suffix row[mid+1..r] into some color b. So for every split
    // point mid and every reachable pair (a, b) we add all colors that the
    // replacement rules allow (a, b) to turn into. rule_mask[a][b] already
    // holds that set as a mask, including the case where one ordered pair is
    // listed under several target colors.
    //
    // The base case is a single stone, which can only stay its own color. The
    // colors Sasha can win with are exactly the bits of dp[0][n-1], since
    // winning means reducing the whole row to one stone of his color.

    int n = (int)row.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));
    for(int i = 0; i < n; i++) {
        dp[i][i] = 1 << color_id(row[i]);
    }

    for(int len = 2; len <= n; len++) {
        for(int l = 0, r = len - 1; r < n; l++, r++) {
            int mask = 0;
            for(int mid = l; mid < r; mid++) {
                int left = dp[l][mid], right = dp[mid + 1][r];
                for(int a = 0; a < 4; a++) {
                    for(int b = 0; b < 4; b++) {
                        if((left >> a & 1) && (right >> b & 1)) {
                            mask |= rule_mask[a][b];
                        }
                    }
                }
            }

            dp[l][r] = mask;
        }
    }

    string colors = "bryw", ans;
    for(int c = 0; c < 4; c++) {
        if(dp[0][n - 1] >> c & 1) {
            ans += colors[c];
        }
    }

    cout << (ans.empty() ? "Nobody" : ans) << '\n';
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
