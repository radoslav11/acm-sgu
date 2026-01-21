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
vector<vector<int>> a;

void read() {
    cin >> n >> m;
    a.assign(n, vector<int>(m));
    cin >> a;
}

void solve() {
    // The core idea here is to think of the process from the end. We know that
    // on the last operation we chose one of the "1"s, so let's try to undo that
    // step. If there was just one it's immediately clear that we can just
    // remove it, and subtract 1 from the neighbors. But what if we have
    // multiple? In that case we can first notice that there can't be two ones
    // that are adjacent to each other - otherwise in the process of "undo"-ing
    // one of them will end up going to negative. If that's not the case, we can
    // just perform all the operations at the same time. While we do this, we
    // should make sure that all non-selected cells so far still remain > 1 even
    // after doing this operation because the problem asks for a solution that
    // covers every cell exactly once.
    //
    // Implementing this naively works in O((N*M)^2), but we can implement it in
    // O(N*M) by keeping a list of positions for every value (5 lists for every
    // value, and keep an iterator in the corresponding list). The complexity is
    // O(N*M) because each move from x -> x-1 can be done in O(1) with list
    // insertions and removals, and the total sum of cells is O(N*M).

    list<pair<int, int>> by_value[6];
    vector<vector<list<pair<int, int>>::iterator>> iters(
        n, vector<list<pair<int, int>>::iterator>(m)
    );
    vector<vector<bool>> done(n, vector<bool>(m, false));

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            by_value[a[i][j]].push_back({i, j});
            iters[i][j] = prev(by_value[a[i][j]].end());
        }
    }

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    vector<pair<int, int>> answer;

    while(!by_value[1].empty()) {
        vector<pair<int, int>> ones(by_value[1].begin(), by_value[1].end());

        for(auto [x, y]: ones) {
            for(int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if(nx >= 0 && nx < n && ny >= 0 && ny < m && !done[nx][ny] &&
                   a[nx][ny] == 1) {
                    cout << "No solution\n";
                    return;
                }
            }
        }

        map<pair<int, int>, int> decr;
        for(auto [x, y]: ones) {
            for(int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if(nx >= 0 && nx < n && ny >= 0 && ny < m && !done[nx][ny]) {
                    decr[{nx, ny}]++;
                }
            }
        }

        for(auto& [pos, cnt]: decr) {
            auto [nx, ny] = pos;
            if(a[nx][ny] - cnt < 1) {
                cout << "No solution\n";
                return;
            }
        }

        for(auto [x, y]: ones) {
            answer.push_back({x, y});
            done[x][y] = true;
            by_value[1].erase(iters[x][y]);
        }

        for(auto& [pos, cnt]: decr) {
            auto [nx, ny] = pos;
            int old_val = a[nx][ny];
            by_value[old_val].erase(iters[nx][ny]);
            a[nx][ny] -= cnt;
            by_value[a[nx][ny]].push_back({nx, ny});
            iters[nx][ny] = prev(by_value[a[nx][ny]].end());
        }
    }

    if((int)answer.size() != n * m) {
        cout << "No solution\n";
        return;
    }

    reverse(answer.begin(), answer.end());
    for(auto [x, y]: answer) {
        cout << x + 1 << ' ' << y + 1 << '\n';
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
