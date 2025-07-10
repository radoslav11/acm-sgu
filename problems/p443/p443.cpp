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

vector<int> prime_factors(int n) {
    vector<int> res;
    for(int i = 2; i * i <= n; i++) {
        if(n % i == 0) {
            res.push_back(i);
            while(n % i == 0) {
                n /= i;
            }
        }
    }
    if(n > 1) {
        res.push_back(n);
    }
    return res;
}

int f(int x) {
    vector<int> p = prime_factors(x);
    int64_t sum = accumulate(p.begin(), p.end(), 0LL);
    int mx = *max_element(p.begin(), p.end());
    return 2 * mx - sum;
}

int a, b;

void read() {
    cin >> a >> b;
}

void solve() {
    if(f(a) > f(b)) {
        cout << "a\n";
    } else {
        cout << "b\n";
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
