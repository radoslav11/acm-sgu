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
}

int n;
vector<int> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    vector<int> up_left(n, 0), up_right(n, 0);
    vector<int> down_left(n, 0), down_right(n, 0);

    for(int i = 1; i < n; i++) {
        if(a[i] > a[i - 1]) {
            up_left[i] = up_left[i - 1] + 1;
        }
        if(a[i] < a[i - 1]) {
            down_left[i] = down_left[i - 1] + 1;
        }
    }

    for(int i = n - 2; i >= 0; i--) {
        if(a[i] > a[i + 1]) {
            up_right[i] = up_right[i + 1] + 1;
        }
        if(a[i] < a[i + 1]) {
            down_right[i] = down_right[i + 1] + 1;
        }
    }

    int max_hill = 0, max_dale = 0;

    for(int i = 0; i < n; i++) {
        if(up_left[i] > 0 && up_right[i] > 0) {
            max_hill = max(max_hill, min(up_left[i], up_right[i]));
        }
        if(down_left[i] > 0 && down_right[i] > 0) {
            max_dale = max(max_dale, min(down_left[i], down_right[i]));
        }
    }

    cout << max_hill << ' ' << max_dale << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }
    return 0;
}
