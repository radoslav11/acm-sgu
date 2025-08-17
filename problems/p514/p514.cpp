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

double area(int a, int b, int c) {
    double s = (a + b + c) / 2.0;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

double rec(int i, int x, int y, int z) {
    if(i == n) {
        if(x + y < z || y + z < x || z + x < y) {
            return 1e18;
        }
        return area(abs(x), abs(y), abs(z));
    }
    double res = 1e18;
    res = min(res, rec(i + 1, x + a[i], y, z));
    res = min(res, rec(i + 1, x, y + a[i], z));
    res = min(res, rec(i + 1, x, y, z + a[i]));
    res = min(res, rec(i + 1, x - a[i], y, z));
    res = min(res, rec(i + 1, x, y - a[i], z));
    res = min(res, rec(i + 1, x, y, z - a[i]));
    return res;
}

void solve() {
    // Fundamentally the solution is simple - we can notice n <= 10, so this
    // immediately suggests that we can use a brute force solution. The main
    // observations is that we should always aim for an area 0.0 polygon (e.g.
    // say if we have 5, 5, 5, and 5). If this is impossible, we should make a
    // triangle. We can note that we should always be able to make on of these
    // two. This idea generalizes - instead of sequentially attaching the sides,
    // we can make an area 0.0 zone as part of a side of the triangle, and
    // essentially get a "smaller" triangle. This leads to the idea of brute
    // forcing the sides of a triangle but we can move the sides in both
    // directions, positive meaning we extend the side, negative meaning we
    // create a zone with almost no area.

    cout << setprecision(10) << fixed << rec(0, 0, 0, 0) << endl;
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
