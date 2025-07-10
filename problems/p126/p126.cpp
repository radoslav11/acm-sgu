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

int64_t a, b;

void read() {
    cin >> a >> b;
}

void solve() {
    // (a, b), (a > b)
    // (a - b, 2b)
    // (a - 3b, 4b)
    // (a - 7b, 8b)
    // ...

    for(int steps = 0; steps <= 60; steps++) {
        if(a > b) {
            swap(a, b);
        }

        if(a == 0) {
            cout << steps << '\n';
            return;
        }

        b -= a;
        a *= 2;
    }

    cout << -1 << '\n';
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
