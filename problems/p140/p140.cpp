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

int n, p, b;
vector<int> a;

void read() {
    cin >> n >> p >> b;
    a.resize(n);
    cin >> a;

    for(auto& ai: a) {
        ai %= p;
    }
}

int64_t extended_euclid(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if(b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t d = extended_euclid(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

pair<vector<int64_t>, int64_t> inductive_solve(const vector<int>& a) {
    vector<int64_t> x(a.size());
    int64_t g = extended_euclid(
        a[a.size() - 2], a[a.size() - 1], x[a.size() - 2], x[a.size() - 1]
    );

    for(int i = n - 2; i >= 0; i--) {
        int64_t prv_g = g, mult;
        g = extended_euclid(a[i], prv_g, x[i], mult);
        for(int j = i + 1; j < n; j++) {
            x[j] = x[j] * mult;
        }
    }

    return {x, g};
}

void solve() {
    a.push_back(p);
    auto [x, g] = inductive_solve(a);

    if(b % g != 0) {
        cout << "NO\n";
        return;
    }

    cout << "YES\n";
    x.pop_back();

    int64_t mult = b / g;
    for(auto& xi: x) {
        xi = xi * mult % p;
        if(xi < 0) {
            xi += p;
        }
    }

    cout << x << '\n';
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
