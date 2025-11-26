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

int n, k;
string s;

void read() {
    cin >> k >> s;
    s = s + s;
    n = s.size();
}

void solve() {
    // We can represent a substring by it's two endpoints L,R, but in this
    // problem it's more convenient to represent it with X=L, Y=(L+R+1)/2, or
    // the positions where the two halves start. We will try to keep an array
    // cnt[X][Y], being the number of positions where X and Y don't match. If we
    // have this it will be trivial to get the final answer.
    //
    // The key idea is that will iterate through all possible (i, j) that
    // correspond to distinct letters, and add +1 to the area of cnt[.][.] that
    // get's affected by this pair. This area is essentially a part of a primary
    // diagonal - a pair (i, j) will affect (x, y) when i-x = j-y, and y > i. We
    // will do prefix sums on that diagonal to get quadratic complexity.

    vector<vector<int>> cnt(n, vector<int>(n, 0));
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(s[i] != s[j]) {
                int d = j - i;
                int delta = min(d - 1, i);
                int min_x = i - delta;
                int min_y = j - delta;

                cnt[min_x][min_y]++;

                if(i + 1 < n && j + 1 < n) {
                    cnt[i + 1][j + 1]--;
                }
            }
        }
    }

    for(int i = 1; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            cnt[i][j] += cnt[i - 1][j - 1];
        }
    }

    string ans = "";
    for(int len = n / 2; len >= 1; len--) {
        if(len % 2 != 0) {
            continue;
        }
        for(int x = 0; x + len <= n; x++) {
            int y = x + len / 2;

            if(cnt[x][y] > k) {
                continue;
            }

            if(!ans.empty() && ans.size() > (size_t)len) {
                continue;
            }

            string cand = s.substr(x, len);
            if(ans.empty() || cand < ans) {
                ans = cand;
            }
        }
    }

    cout << ans << endl;
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
