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
vector<int> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    // This problem is about minesweeper, but we are given a N x 2 grid.
    // Column 1 is hidden and may contain mines. Column 2 is fully visible and
    // mine-free, and each cell shows how many mines are among its neighbors in
    // column 1. Since column 2 has no mines, cell i in column 2 only looks at
    // cells i-1, i, i+1 in column 1 (where they exist):
    //   a[0] = m[0] + m[1]  (top row, 2 neighbors)
    //   a[i] = m[i-1] + m[i] + m[i+1]  (middle rows, 3 neighbors)
    //   a[n-1] = m[n-2] + m[n-1]  (bottom row, 2 neighbors)
    // We need to count how many binary assignments of mines to column 1
    // produce the given numbers. The key insight is that once we fix m[0],
    // every other value is forced by the equations, so the answer is at most
    // 2. We just try both and count the valid ones.

    auto check = [&](int first) -> bool {
        vector<int> m(n);
        m[0] = first;
        if(n == 1) {
            return a[0] == m[0];
        }
        m[1] = a[0] - m[0];
        if(m[1] < 0 || m[1] > 1) {
            return false;
        }
        for(int i = 1; i < n - 1; i++) {
            m[i + 1] = a[i] - m[i] - m[i - 1];
            if(m[i + 1] < 0 || m[i + 1] > 1) {
                return false;
            }
        }
        return a[n - 1] == m[n - 1] + m[n - 2];
    };

    int ans = 0;
    if(check(0)) {
        ans++;
    }
    if(check(1)) {
        ans++;
    }
    cout << ans << "\n";
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
