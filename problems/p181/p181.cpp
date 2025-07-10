#include <bits/stdc++.h>
// #include <coding_library/math/combinatorics.cpp>
#define beta wawawa

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

int A, alpha, beta, gamma, M, k;

void read() { cin >> A >> alpha >> beta >> gamma >> M >> k; }

int f(int x) { return (alpha * x * x + beta * x + gamma) % M; }

void solve() {
    if(k == 0) {
        cout << A << '\n';
        return;
    }

    vector<int> period(M, -1);
    int pos = 0, x = A % M;

    while(k > 0 && period[x] == -1) {
        period[x] = pos++;
        x = f(x);
        k--;
    }

    if(k == 0) {
        cout << x << '\n';
    } else {
        int cycle = pos - period[x];
        k %= cycle;
        while(k--) {
            x = f(x);
        }
        cout << x << '\n';
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
