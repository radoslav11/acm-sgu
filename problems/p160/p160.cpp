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

int n, m;
vector<int> a;

void read() {
    cin >> n >> m;
    a.resize(n);
    cin >> a;
}

void solve() {
    vector<vector<bool>> dp(n + 1, vector<bool>(m, false));
    vector<vector<bool>> take(n + 1, vector<bool>(m, false));
    dp[0][1] = true;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(dp[i][j]) {
                dp[i + 1][j] = true;
                dp[i + 1][(j * a[i]) % m] = true;
                take[i + 1][(j * a[i]) % m] = true;
            }
        }
    }

    int max_score = 0;
    for(int j = m - 1; j >= 0; j--) {
        if(dp[n][j]) {
            max_score = j;
            break;
        }
    }

    vector<int> levers;
    int curr = max_score;
    for(int i = n; i > 0; i--) {
        if(take[i][curr]) {
            levers.push_back(i);
            for(int j = 0; j < m; j++) {
                if(dp[i - 1][j] && (j * a[i - 1]) % m == curr) {
                    curr = j;
                    break;
                }
            }
        }
    }

    reverse(levers.begin(), levers.end());
    cout << max_score << '\n';
    cout << levers << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    read();
    solve();
    return 0;
}
