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
}

int n;
vector<int> vals;

void read() {
    vals.resize(3 * n);
    cin >> vals;
}

int dp[1 << 25];
int8_t popcnt[1 << 25];

void precompute() {
    popcnt[0] = 0;
    for(int mask = 1; mask < (1 << 25); mask++) {
        popcnt[mask] = popcnt[mask >> 1] + (mask & 1);
    }
}

void solve() {
    vector<pair<int, int>> v;
    for(int i = 0; i < 3 * n; i++) {
        v.emplace_back(vals[i], i);
    }

    sort(v.begin(), v.end());

    vector<pair<int, int>> B, AC;
    // It's always optimal to take the first n elements for C
    B.insert(B.end(), v.begin(), v.begin() + n);
    AC.insert(AC.end(), v.begin() + n, v.end());

    // We maintain a rolling window starting from the end of AC. The
    // mask represents the elements we have taken. Note that it's always
    // best to put the A[cnt] at the end of the mask, or the last zero bit.
    // For the C, we can put it anywhere before. After this step, we need to
    // "recycle" the mask by shifting it left and filling the last zero.
    // Without the recycling, the naive solution would be O*(4^n), but the
    // core observation that allows us to do this recycling idea is that it
    // never makes sense to put the C with the corresponding A very far from
    // each other.

    memset(dp, 0, (1 << n) * sizeof(int));
    int top_zero = n;
    for(int mask = 0; mask < (1 << n); mask++) {
        int cnt = popcnt[mask];
        int next_mask = mask << 1;
        if((next_mask >> top_zero) & 1) {
            top_zero--;
        }

        next_mask |= (1 << top_zero);
        next_mask &= (1 << n) - 1;

        for(int i = 0; i < n; i++) {
            if((next_mask >> i) & 1) {
                break;
            }

            int Bj = B[cnt].first;
            int Aj = AC[n - cnt + top_zero - 1].first;
            int Cj = AC[n - cnt + i - 1].first;

            dp[next_mask | (1 << i)] =
                max(dp[next_mask | (1 << i)], dp[mask] + (Aj - Bj) * Cj);
        }
    }

    cout << dp[(1 << n) - 1] << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    precompute();

    int T = 1;
    cin >> T >> n;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
