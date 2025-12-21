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

int q, c, n, t;
vector<pair<int, int>> xy;

void read() {
    cin >> q >> c >> n >> t;
    xy.resize(n);
    cin >> xy;
}

void solve() {
    // We can do a simple BFS solution: each culture expands from its starting
    // position. Since cultures divide in order (1, 2, 3, ...) each second, and
    // entering an occupied cell kills the incoming bacterium, we use
    // multi-source BFS. The queue naturally processes cells in order of
    // distance, and we add cultures in index order so ties go to lower-indexed
    // cultures.

    vector<vector<int>> owner(q + 1, vector<int>(c + 1, 0));
    vector<int> ans(n + 1, 0);
    queue<tuple<int, int, int, int>> bfs;

    vector<pair<int, int>> dxy = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int dy[] = {1, 0, -1, 0};

    for(int i = 0; i < n; i++) {
        int x = xy[i].first, y = xy[i].second;
        if(x >= 1 && x <= q && y >= 1 && y <= c && owner[x][y] == 0) {
            owner[x][y] = i + 1;
            bfs.push({x, y, 0, i + 1});
        }
    }

    while(!bfs.empty()) {
        auto [x, y, ti, num] = bfs.front();
        bfs.pop();

        if(ti >= t) {
            continue;
        }

        for(auto [dx, dy]: dxy) {
            int nx = x + dx;
            int ny = y + dy;
            if(nx >= 1 && nx <= q && ny >= 1 && ny <= c && owner[nx][ny] == 0) {
                owner[nx][ny] = num;
                bfs.push({nx, ny, ti + 1, num});
            }
        }
    }

    for(int x = 1; x <= q; x++) {
        for(int y = 1; y <= c; y++) {
            ans[owner[x][y]]++;
        }
    }

    for(int i = 1; i <= n; i++) {
        cout << ans[i] << '\n';
    }
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
