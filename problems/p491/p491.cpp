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

void read() { cin >> n; }

void solve() {
    // We can iterate through all values A, and try to figure out what the valid
    // values B are. Let's fix all O(N) values A. We want A*x+B*y=n. The first
    // observation is that there are only O(N/A) values for X, which means
    // overall fixing (A, x) is O(N log N) due to the sum of the harmonic
    // sequence. Afterwards, we can try to find all valid (B, y) such that
    // B*y=n-A*x. This is precisely the factors of n-A*x which in practice is
    // O(N^(1/3)), but we should precompute it in a sieve like way too.
    //
    // One thing we should be careful about is that for some A we shouldn't over
    // count the values B. We can do this directly with a global visited array,
    // where one neat way is to just use an integer marker based on A. This
    // O(N^(4/3) log N) which should be fast enough.

    vector<vector<int>> divisors(n + 1);
    for(int d = 1; d <= n; d++) {
        for(int m = d; m <= n; m += d) {
            divisors[m].push_back(d);
        }
    }

    vector<int> visited(n + 1, 0);
    int ans = 0;

    for(int a = 1; a < n; a++) {
        for(int x = 1; a * x < n; x++) {
            int remainder = n - a * x;
            for(int b: divisors[remainder]) {
                if(b > a && visited[b] != a) {
                    visited[b] = a;
                    ans++;
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
