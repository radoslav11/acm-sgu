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

void read() { cin >> n >> m; }

void solve() {
    // Try all 3 possible diagonal patterns (offset 0, 1, 2)
    // Each pattern creates diagonal lines with period 3
    int min_count = INT_MAX;
    int best_offset = 0;

    // Test each of the 3 possible diagonal offsets
    for(int offset = 0; offset < 3; offset++) {
        int count = 0;

        // Count how many '#' symbols this offset would create
        for(int i = 0; i < n; i++) {
            // For row i, place '#' at columns: (i + offset) % 3, (i + offset) %
            // 3 + 3, (i + offset) % 3 + 6, ... This creates a diagonal pattern
            // with period 3
            for(int j = (i + offset) % 3; j < m; j += 3) {
                count++;
            }
        }

        // Keep track of the offset that minimizes the number of '#' symbols
        if(count < min_count) {
            min_count = count;
            best_offset = offset;
        }
    }

    // Create the grid using the best offset pattern
    // Use vector of strings for cleaner memory management
    vector<string> grid(n, string(m, '0'));

    // Fill the grid with '#' symbols using the optimal diagonal pattern
    for(int i = 0; i < n; i++) {
        // Place '#' at positions following the diagonal pattern
        for(int j = (i + best_offset) % 3; j < m; j += 3) {
            grid[i][j] = '#';
        }
    }

    // Output the resulting grid
    for(int i = 0; i < n; i++) {
        cout << grid[i] << endl;
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
