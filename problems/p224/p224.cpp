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

int n, k;

void read() { cin >> n >> k; }

void change_queen(int row, int col, vector<vector<int>> &attacked, int delta) {
    for(int i = 0; i < n; i++) {
        attacked[row][i] += delta;
        attacked[i][col] += delta;
        if(row + i < n && col + i < n) {
            attacked[row + i][col + i] += delta;
        }
        if(row + i < n && col - i >= 0) {
            attacked[row + i][col - i] += delta;
        }
        if(row - i >= 0 && col + i < n) {
            attacked[row - i][col + i] += delta;
        }
        if(row - i >= 0 && col - i >= 0) {
            attacked[row - i][col - i] += delta;
        }
    }
    attacked[row][col] -= 3 * delta;
}

int brute(int row, int k, vector<vector<int>> &attacked) {
    if(k > n - row) {
        return 0;
    }

    if(row == n) {
        return 1;
    }
    int ans = brute(row + 1, k, attacked);
    for(int col = 0; col < n; col++) {
        if(k && !attacked[row][col]) {
            change_queen(row, col, attacked, 1);
            ans += brute(row + 1, k - 1, attacked);
            change_queen(row, col, attacked, -1);
        }
    }
    return ans;
}

void solve() {
    vector<vector<int>> attacked(n, vector<int>(n, 0));
    cout << brute(0, k, attacked) << endl;
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
