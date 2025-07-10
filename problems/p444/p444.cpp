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
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
}

int64_t phi(int64_t n) {
    int64_t result = n;
    for(int64_t i = 2; i * i <= n; ++i) {
        if(n % i == 0) {
            while(n % i == 0) {
                n /= i;
            }
            result -= result / i;
        }
    }
    if(n > 1) {
        result -= result / n;
    }
    return result;
}

int64_t mod_pow(int64_t a, int64_t p, int64_t m) {
    int64_t result = 1;
    a %= m;
    while(p) {
        if(p & 1) {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        p >>= 1;
    }
    return result;
}

pair<int64_t, int64_t> find_decimal_info(int64_t x, int64_t y) {
    vector<int> visited(y, -1);
    int64_t rem = x % y;
    int64_t digits = 0;
    while(rem != 0 && visited[rem] == -1) {
        visited[rem] = digits;
        rem = (rem * 10) % y;
        digits++;
    }
    if(rem == 0) {
        return {digits, 0};
    }
    int64_t cycle_length = digits - visited[rem];
    int64_t non_recurring = digits - cycle_length;
    return {non_recurring, cycle_length};
}

int64_t x, y;

void read() { cin >> x >> y; }

void solve() {
    auto [non_recurring, cycle_length] = find_decimal_info(x, y);
    cout << non_recurring << ' ' << cycle_length << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
