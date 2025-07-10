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

int n, m, y;
vector<int> x;

void read() {
    cin >> n >> m >> y;
    x.resize(n);
    cin >> x;
}

void solve() {
    // x[i] / y = a[i] / m

    vector<int> a_floor(n), a_ceil(n);
    vector<double> delta(n);

    int sum_floor = 0;
    for(int i = 0; i < n; i++) {
        a_floor[i] = (x[i] * m) / y;
        a_ceil[i] = (x[i] * m + y - 1) / y;

        sum_floor += a_floor[i];
        if(a_floor[i] == a_ceil[i]) {
            continue;
        }

        double a_exact = (double)x[i] * m / y;
        delta[i] = -(a_exact - a_floor[i]) + (a_ceil[i] - a_exact);
    }

    int buffer = m - sum_floor;

    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    sort(perm.begin(), perm.end(), [&](int i, int j) {
        return delta[i] < delta[j];
    });

    assert(buffer <= n);
    for(int i = 0; i < buffer; i++) {
        a_floor[perm[i]] = a_ceil[perm[i]];
    }

    cout << a_floor << '\n';
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
