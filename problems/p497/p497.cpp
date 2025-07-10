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
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
}

int64_t n;

void read() { cin >> n; }

vector<int64_t> compute_partitions_slow(int max_n) {
    vector<int64_t> dp(max_n + 1, 0);
    dp[0] = 1;
    for(int j = 1; j <= max_n; ++j) {
        for(int i = j; i <= max_n; ++i) {
            dp[i] += dp[i - j];
        }
    }
    return dp;
}

vector<int64_t> compute_partitions(int max_n) {
    vector<int64_t> p(max_n + 1, 0);
    p[0] = 1;

    for(int n = 1; n <= max_n; ++n) {
        int64_t sum = 0;
        int k = 1;
        while(true) {
            int pent1 = (k * (3 * k - 1)) / 2;
            int pent2 = ((-k) * (3 * (-k) - 1)) / 2;
            if(pent1 > n && pent2 > n) {
                break;
            }

            int sign = (k % 2 == 1) ? 1 : -1;
            if(pent1 <= n) {
                sum += sign * p[n - pent1];
            }
            if(pent2 <= n && pent2 != pent1) {
                sum += sign * p[n - pent2];
            }
            ++k;
        }
        p[n] = sum;
    }
    return p;
}

void solve() {
    vector<int64_t> partitions = compute_partitions(100);
    int64_t result = 1;
    for(int64_t p = 2; p * p <= n; ++p) {
        if(n % p == 0) {
            int e = 0;
            while(n % p == 0) {
                e++;
                n /= p;
            }
            result *= partitions[e];
        }
    }
    if(n > 1) {
        result *= partitions[1];
    }
    cout << result << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }
    return 0;
}
