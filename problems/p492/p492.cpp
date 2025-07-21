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

int64_t n;

void read() { cin >> n; }

vector<vector<vector<int64_t>>> dp;

string bound_s;

int64_t rec(int pos, int last_1, int is_smaller) {
    if(pos == bound_s.size()) {
        return 1;
    }

    int64_t& memo = dp[pos][last_1][is_smaller];
    if(memo != -1) {
        return memo;
    }

    memo = 0;
    int up = bound_s[pos] - '0';
    int high = is_smaller ? 9 : up;
    for(int d = 0; d <= high; d++) {
        if(last_1 && (d == 3)) {
            continue;
        }

        memo += rec(pos + 1, d == 1, is_smaller | (d < up));
    }

    return memo;
}

int64_t cnt_below(int64_t bound) {
    bound_s = to_string(bound);
    dp.assign(bound_s.size(), {{-1, -1}, {-1, -1}});
    return rec(0, 0, 0);
}

void solve() {
    int64_t low = 0, high = 2 * n, mid, ans;
    while(low <= high) {
        mid = low + (high - low) / 2;
        // -1 as we shouldn't count 0.
        if(cnt_below(mid) - 1 >= n) {
            high = mid - 1;
            ans = mid;
        } else {
            low = mid + 1;
        }
    }

    cout << ans << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
