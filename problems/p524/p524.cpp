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
vector<int> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

long double f(long double l, long double d) {
    long double ans = 0;
    for(int i = 0; i < n; i++) {
        ans += abs(a[i] - (l + d * i));
    }
    return ans;
}

pair<long double, long double> f(long double d) {
    long double l = -1e12, r = 1e12, m1, m2, ans_l = 0;
    for(int steps = 0; steps < 100; steps++) {
        m1 = l + (r - l) / 3;
        m2 = r - (r - l) / 3;
        if(f(m1, d) < f(m2, d)) {
            r = m2;
            ans_l = m1;
        } else {
            l = m1;
            ans_l = m2;
        }
    }

    return {f(ans_l, d), ans_l};
}

void solve() {
    sort(a.begin(), a.end());

    long double l = 0, r = 1e7, m1, m2, ans_d = 1;
    for(int steps = 0; steps < 100; steps++) {
        m1 = l + (r - l) / 3;
        m2 = r - (r - l) / 3;
        if(f(m1).first < f(m2).first) {
            r = m2;
            ans_d = m1;
        } else {
            l = m1;
            ans_d = m2;
        }
    }

    pair<long double, long double> ans = f(ans_d);
    cout << setprecision(9) << fixed << ans.first << '\n';
    for(int i = 0; i < n; i++) {
        cout << setprecision(9) << fixed << ans.second + ans_d * i << ' ';
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
