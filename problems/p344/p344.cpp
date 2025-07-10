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
vector<string> t;

void read() {
    cin >> n >> m;
    t.resize(n);
    cin >> t;
}

void solve() {
    int k = n * m;
    vector<bool> visited(k, false);
    vector<int> degree(k, 0);

    queue<int> q;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(t[i][j] == 'X') {
                q.push(i * m + j);
                visited[i * m + j] = true;
            }
        }
    }

    int ans = 0;
    while(!q.empty()) {
        int u = q.front();
        ans++;
        q.pop();
        int i = u / m;
        int j = u % m;
        for(auto [x, y]:
            vector<pair<int, int>>{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
            int ni = i + x;
            int nj = j + y;
            if(ni >= 0 && ni < n && nj >= 0 && nj < m) {
                degree[ni * m + nj]++;
                if(degree[ni * m + nj] >= 2 && !visited[ni * m + nj]) {
                    q.push(ni * m + nj);
                    visited[ni * m + nj] = true;
                }
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
