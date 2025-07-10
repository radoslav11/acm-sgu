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
vector<vector<int>> a, b, b2;

void read() {
    cin >> n;
    b.resize(n, vector<int>(n));
    a.resize(n, vector<int>(n));
    cin >> b;
    b2 = b;
}

bool change(int i, int j, int delta) {
    bool fail = false;
    if(i > 0) {
        b2[i][j] += delta * (a[i - 1][j] > a[i][j]);
        b2[i - 1][j] += delta * (a[i - 1][j] < a[i][j]);
        if(b2[i][j] < 0 || b2[i - 1][j] < 0) {
            fail = true;
        }
    }

    if(j > 0) {
        b2[i][j] += delta * (a[i][j - 1] > a[i][j]);
        b2[i][j - 1] += delta * (a[i][j - 1] < a[i][j]);
        if(b2[i][j] < 0 || b2[i][j - 1] < 0) {
            fail = true;
        }
    }
    return fail;
}

void backtrack(int pos) {
    if(pos == n * n) {
        for(int j = 0; j < n; j++) {
            if(b2[n - 1][j] != 0) {
                return;
            }
        }

        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                cout << a[i][j] << " \n"[j == n - 1];
            }
        }
        exit(0);
    }

    int i = pos / n, j = pos % n;
    for(int val = 0; val <= 9; val++) {
        a[i][j] = val;
        if(!change(i, j, -1) && (i == 0 || b2[i - 1][j] == 0)) {
            backtrack(pos + 1);
        }
        change(i, j, 1);
    }
}

void solve() {
    backtrack(0);
    cout << "NO SOLUTION" << '\n';
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
