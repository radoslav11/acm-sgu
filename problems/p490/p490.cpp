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

int h, w, n;

void read() { cin >> h >> w >> n; }

void solve() {
    // The solution here is fairly simple after understanding the problem
    // statement. We want to have exactly n white connected component, but
    // instead for a h x w grid, we can try to see what's the largest number of
    // components and then fill some of them down to n with black cells. The key
    // observation is that in the white connected components we don't consider
    // diagonal neigbours, meaning we can try a chess board. There are two
    // candidates for a chess board, and we choose the one with more white
    // cells. We only place on inner positions (skip the border) because a
    // white cell on the border would connect to the infinite white exterior
    // and not form a finite spot.

    static bool first = true;
    if(!first) {
        cout << '\n';
    }
    first = false;

    array<int, 2> count = {0, 0};
    for(int i = 1; i < h - 1; i++) {
        for(int j = 1; j < w - 1; j++) {
            count[(i + j) % 2]++;
        }
    }

    int max_n = max(count[0], count[1]);
    if(n > max_n) {
        cout << "Impossible\n";
        return;
    }

    int p_choose = count[0] >= count[1] ? 0 : 1;

    vector<pair<int, int>> positions;
    for(int i = 1; i < h - 1; i++) {
        for(int j = 1; j < w - 1; j++) {
            if((i + j) % 2 == p_choose) {
                positions.emplace_back(i, j);
            }
        }
    }

    vector<string> grid(h, string(w, '.'));
    for(int k = 0; k < n; k++) {
        grid[positions[k].first][positions[k].second] = '#';
    }

    for(auto& row: grid) {
        cout << row << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
