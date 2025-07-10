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

int64_t A, B, C;

int64_t f(int64_t x) {
    int64_t res = A;
    res *= x;
    res += x % B;
    res %= C;
    return res;
}

void read() { cin >> A >> B >> C; }

int hare_tortoise() {
    int t = 0;
    int64_t hare = f(f(1));
    int64_t tortoise = f(1);
    do {
        tortoise = f(tortoise);
        hare = f(f(hare));
        t++;
        if(t > 2000042) {
            return -1;
        }
    } while(tortoise != hare);

    int mu = 0;
    tortoise = 1;
    while(tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(hare);
        mu++;
    }

    int lambda = 1;
    hare = f(tortoise);
    while(tortoise != hare) {
        hare = f(hare);
        lambda++;
    }

    return mu + lambda;
}

void solve() {
    int ans = hare_tortoise();
    if(ans > 2000000) {
        ans = -1;
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
