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
vector<string> grid;
string path;

void read() {
    cin >> n >> m;
    grid.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> grid[i];
    }
    cin >> path;
}

void solve() {
    // The solution to this problem is standard implementation. We essentially
    // just want to keep the current position (x, y) and direction dir, and
    // move along the grid. We will also keep the visited cells so far so that
    // we know if we need to divide by 2.

    int x = 0, y = 0;
    int dir = 0;
    int satisfaction = 0;

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    vector<vector<int>> visited(n, vector<int>(m, 0));
    for(char c: path) {
        if(c == 'L') {
            dir = (dir + 3) % 4;
        } else if(c == 'R') {
            dir = (dir + 1) % 4;
        } else {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            // Determine the blocks we pass when moving from (x, y) to (nx, ny).
            if(dir == 0) {
                // Blocks above and below this segment.
                if(y > 0 && x < m) {
                    int beauty = grid[y - 1][x] - '0';
                    satisfaction +=
                        (visited[y - 1][x] == 0) ? beauty : beauty / 2;
                    visited[y - 1][x]++;
                }
                if(y < n && x < m) {
                    int beauty = grid[y][x] - '0';
                    satisfaction += (visited[y][x] == 0) ? beauty : beauty / 2;
                    visited[y][x]++;
                }
            } else if(dir == 1) {
                // Blocks left and right of this segment.
                if(y < n && x > 0) {
                    int beauty = grid[y][x - 1] - '0';
                    satisfaction +=
                        (visited[y][x - 1] == 0) ? beauty : beauty / 2;
                    visited[y][x - 1]++;
                }
                if(y < n && x < m) {
                    int beauty = grid[y][x] - '0';
                    satisfaction += (visited[y][x] == 0) ? beauty : beauty / 2;
                    visited[y][x]++;
                }
            } else if(dir == 2) {
                // Blocks above and below this segment.
                if(y > 0 && x > 0) {
                    int beauty = grid[y - 1][x - 1] - '0';
                    satisfaction +=
                        (visited[y - 1][x - 1] == 0) ? beauty : beauty / 2;
                    visited[y - 1][x - 1]++;
                }
                if(y < n && x > 0) {
                    int beauty = grid[y][x - 1] - '0';
                    satisfaction +=
                        (visited[y][x - 1] == 0) ? beauty : beauty / 2;
                    visited[y][x - 1]++;
                }
            } else {
                // Blocks left and right of this segment.
                if(y > 0 && x > 0) {
                    int beauty = grid[y - 1][x - 1] - '0';
                    satisfaction +=
                        (visited[y - 1][x - 1] == 0) ? beauty : beauty / 2;
                    visited[y - 1][x - 1]++;
                }
                if(y > 0 && x < m) {
                    int beauty = grid[y - 1][x] - '0';
                    satisfaction +=
                        (visited[y - 1][x] == 0) ? beauty : beauty / 2;
                    visited[y - 1][x]++;
                }
            }

            x = nx;
            y = ny;
        }
    }

    cout << satisfaction << '\n';
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
