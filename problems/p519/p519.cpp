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

int n, m;
vector<string> s;

void read() {
    cin >> n >> m;
    s.resize(n);
    cin >> s;
}

void solve() {
    int ans = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(s[i][j] == '0') {
                continue;
            }

            ans += 2;
            int curr = s[i][j] - '0';
            for(auto [dx, dy]:
                vector<pair<int, int>>{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}) {
                int x = i + dx;
                int y = j + dy;

                int other = 0;
                if(x < n && y < m && x >= 0 && y >= 0) {
                    other = s[x][y] - '0';
                }

                ans += max(0, curr - other);
            }
        }
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
