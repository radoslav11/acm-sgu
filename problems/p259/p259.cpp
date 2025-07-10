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
vector<pair<int, int>> a;

void read() {
    cin >> n;
    a.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> a[i].first;
    }
    for(int i = 0; i < n; i++) {
        cin >> a[i].second;
    }
}

void solve() {
    // Current time: max(a1 + b1, a1 + a2 + b2)
    // Alternative time: max(a2 + b2, a2 + a1 + b1)
    //      -= a1 + a2
    // Current time: max(b1 - a2, b2)
    // Alternative time: max(b2 - a1, b1)
    //       If b1 - a2 > b2
    //             Current time: b1 - a2
    //             Alternative time: b1         -> Alternative time is longer
    //       If b1 - a2 < b2
    //             Current time: b2
    //             Alternative time: max(b2 - a1, b1)
    //                                          -> Alternative time is better
    //                                          only
    //                                             if b1 < b2

    // Current time: max(a1 + b1, a1 + a2 + b2)
    // Alternative time: max(a2 + b2, a2 + a1 + b1)
    // -> b1 < b2
    //    Current time: a1 + b1 + b2
    //    Alternative time: max(a1 + a2 + b1, a2 + b2) < a1 + b1 + b2
    // -> b1 > b2
    //    Current time: max(a1 + b1, a1 + a2 + b2)
    //    Alternative time: a1 + a2 + b1 > Current time

    stable_sort(
        a.begin(), a.end(),
        [](const pair<int, int> &x, const pair<int, int> &y) {
            return max(x.first + x.second, x.first + y.first + y.second) <
                   max(y.first + y.second, y.first + x.first + x.second);
        }
    );

    int ans = 0, sum = 0;
    for(int i = 0; i < n; i++) {
        sum += a[i].first;
        ans = max(ans, sum + a[i].second);
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
