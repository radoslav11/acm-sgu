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
vector<int64_t> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

vector<vector<int>> pref_cnt_1;

void prepare() {
    pref_cnt_1.assign(61, vector<int>(n + 1, 0));

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);

    for(int bit = 0; bit < 61; bit++) {
        vector<vector<int>> with_bit(2);
        for(int i = 0; i < n; i++) {
            int pos = order[i];
            int cbit = (a[pos] >> bit) & 1;
            with_bit[cbit].push_back(pos);
            pref_cnt_1[bit][i + 1] = pref_cnt_1[bit][i] + cbit;
        }

        order = std::move(with_bit[0]);
        order.insert(order.end(), with_bit[1].begin(), with_bit[1].end());
    }
}

vector<vector<int64_t>> dp;

int64_t rec(int bit, int carry_cnt) {
    if(bit == 61) {
        return carry_cnt == 0;
    }

    int64_t& memo = dp[bit][carry_cnt];
    if(memo != -1) {
        return memo;
    }

    memo = 0;
    for(int x = 0; x < 2; x++) {
        int xor_all = 0, new_carry_cnt = 0;
        for(int v = 0; v < 2; v++) {
            for(int c = 0; c < 2; c++) {
                int all_cnt = c ? carry_cnt : n - carry_cnt;
                int cnt = c ? pref_cnt_1[bit][carry_cnt]
                            : pref_cnt_1[bit][n] - pref_cnt_1[bit][carry_cnt];

                if(v == 0) {
                    cnt = all_cnt - cnt;
                }

                xor_all ^= (x ^ v ^ c) * (cnt & 1);
                if(v - x - c < 0) {
                    new_carry_cnt += cnt;
                }
            }
        }

        if(xor_all == 0) {
            memo += rec(bit + 1, new_carry_cnt);
        }
    }

    return memo;
}

void solve() {
    prepare();

    dp.assign(61, vector<int64_t>(n + 1, -1));
    int64_t ans = rec(0, 0);

    int64_t mn = *min_element(a.begin(), a.end()), x = 0;
    for(int i = 0; i < n; i++) {
        x ^= a[i] - mn;
    }

    if(x == 0) {
        ans--;
    }

    cout << ans << '\n';
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
