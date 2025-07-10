#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

vector<int> super_primes(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;

    int cnt = 0;
    vector<int> super_primes;
    for(int i = 2; i <= n; i++) {
        if(is_prime[i]) {
            cnt++;
            if(is_prime[cnt]) {
                super_primes.push_back(i);
            }

            for(int64_t j = i * 1ll * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    return super_primes;
}

vector<int> sp;

int n;

void read() { cin >> n; }

void solve() {
    vector<int> dp(n + 1, -1);

    dp[0] = 0;
    for(int i = 0; i < (int)sp.size(); i++) {
        for(int j = sp[i]; j <= n; j++) {
            if(dp[j - sp[i]] != -1) {
                if(dp[j] == -1) {
                    dp[j] = dp[j - sp[i]] + 1;
                } else if(dp[j - sp[i]] + 1 < dp[j]) {
                    dp[j] = dp[j - sp[i]] + 1;
                }
            }
        }
    }

    if(dp[n] == -1) {
        cout << 0 << '\n';
    } else {
        vector<int> ans;
        while(n > 0) {
            for(int i = 0; i < (int)sp.size(); i++) {
                if(n >= sp[i] && dp[n - sp[i]] == dp[n] - 1) {
                    ans.push_back(sp[i]);
                    n -= sp[i];
                    break;
                }
            }
        }

        cout << ans.size() << '\n';
        sort(ans.rbegin(), ans.rend());
        cout << ans << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    sp = super_primes((int)1e6);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
