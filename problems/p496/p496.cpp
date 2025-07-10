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
vector<array<int, 4>> a;

void read() {
    cin >> n;
    a.resize(n);
    for(auto& x: a) {
        cin >> x[0] >> x[1] >> x[2] >> x[3];
    }
}

void solve() {
    map<pair<int, int>, map<pair<int, int>, int>> slopes_per_point;

    for(auto i = 0; i < n; i++) {
        int dx = a[i][2] - a[i][0];
        int dy = a[i][3] - a[i][1];
        int g = gcd(dx, dy);
        dx /= g;
        dy /= g;

        if(dx < 0) {
            dx = -dx;
            dy = -dy;
        }

        if(dx == 0) {
            dy = 1;
        }

        if(dy == 0) {
            dx = 1;
        }

        slopes_per_point[{a[i][0], a[i][1]}][{dx, dy}]++;
        slopes_per_point[{a[i][2], a[i][3]}][{dx, dy}]++;
    }

    int ans = 0;
    for(auto& [_, slopes]: slopes_per_point) {
        int cur = 0;
        for(auto& [slope, cnt]: slopes) {
            auto [dx, dy] = slope;
            swap(dx, dy);
            dx *= -1;
            if(dx < 0) {
                dx = -dx;
                dy = -dy;
            }
            if(slopes.count({dx, dy})) {
                cur += cnt * slopes[{dx, dy}];
            }
        }
        ans += cur;
    }

    cout << ans / 2 << '\n';
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
