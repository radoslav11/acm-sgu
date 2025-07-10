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

int n, m;
vector<int> p;

void read() {
    cin >> n >> m;
    p.resize(m);
    for(int i = 0; i < m; i++) {
        cin >> p[i];
    }
    p.push_back(1);
}

void solve() {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());

    vector<int> pos_of_state(1 << 10, -1);

    vector<int> dp(min(1 << 13, n + 1), 0);
    dp[1] = 0;
    dp[0] = 1;

    int ans = -1;
    for(int i = 2; i <= n; i++) {
        for(int x: p) {
            if(x > i) {
                break;
            }
            if(dp[i - x] == 0) {
                dp[i] = 1;
                break;
            }
        }

        int state = 0;
        for(int prv = 0; prv < 10; prv++) {
            if(i - prv < 0 || dp[i - prv] == 1) {
                state |= (1 << prv);
            }
        }

        if(pos_of_state[state] == -1) {
            pos_of_state[state] = i;
        } else {
            int cycle_length = i - pos_of_state[state];
            int need = (n - i) % cycle_length;

            ans = dp[i - cycle_length + need];
            break;
        }
    }

    if(ans == -1) {
        ans = dp[n];
    }

    cout << (ans ? "FIRST PLAYER MUST WIN" : "SECOND PLAYER MUST WIN") << '\n';
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
