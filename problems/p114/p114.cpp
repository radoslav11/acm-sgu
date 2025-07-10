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

int n;
vector<int> x, a; 

void read() {
    cin >> n;
    x.resize(n);
    a.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> x[i] >> a[i];
    }
}

double f(double p) {
    double res = 0;
    for(int i = 0; i < n; i++) {
        res += a[i] * abs(x[i] - p);
    }
    return res;
}

void solve() {
    double l = *min_element(x.begin(), x.end()), r = *max_element(x.begin(), x.end()), m1, m2;
    for(int steps = 0; steps < 70; steps++) {
        m1 = l + (r - l) / 3;
        m2 = r - (r - l) / 3;
        if(f(m1) <= f(m2)) {
            r = m2;
        } else {
            l = m1;
        }
    }

    cout << setprecision(5) << fixed << r << endl;
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
