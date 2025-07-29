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

int p;

void solve() {
    // The trick in this problem is that we can compute the answer in advance and
    // then answer everything in O(1). This should be obvious by realizing that there
    // are only 100 possible values for p, and we can compute the answer for each one.
    //
    // The other observation is that the answer is small, and certainly less than 4. 
    // 1) For cnt = 1, we can cover the whole board with 1 laser. Assuming that the board 
    //    is 1x1, the diameter is sqrt(2) and the radius of the circle is 1/2. Then the
    //    area is pi * (1/2)^2 = pi/4, or roughly 78.5%.
    // 2) For cnt = 2 and cnt = 3, we can simply brute force all positions of the laser,
    //    up to some precision. We can notice that for 3 lasers, it's quite easy to find
    //    a solution with 99% coverage, and for 2 lasers, we can simply use precision that
    //    is roughly 500 and find a solution with 95% coverage.
    // 4) We can show that to get 100% coverage, we need at 3 circles of radius at least
    //    sqrt(65)/16, which is a bit over the radius of 1/2 we have. A relevant page for 
    //    this is: https://www.quora.com/A-unit-square-is-completely-covered-by-three-
    //             identical-circles-Find-the-smallest-possible-diameter-of-the-circles
    //    Hence, the answer of 99% is satisfactory for 3 lasers too, and we don't have to
    //    use too much compute.

    if(p == 0) {
        cout << 0 << '\n';
    } else if(p <= 78) {
        cout << 1 << '\n';
    } else if(p <= 95) {
        cout << 2 << '\n';
    } else if(p <= 99) {
        cout << 3 << '\n';
    } else {
        cout << 4 << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    for(int test = 1; cin >> p; test++) {
        cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
