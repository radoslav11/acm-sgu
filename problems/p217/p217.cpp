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

double r1, r2;

void read() { cin >> r1 >> r2; }

void solve() {
    // Let's have r = min(r1, r2), and R = max(r1, r2). The rough idea is that
    // we can put the origin of the coordinate system where the two axes
    // intersect. Let's place the axis of the first cylinder (radius r) along
    // the x-axis, and the second (radius R) along the y-axis. Then we are
    // interested in points x,y,z satisfying:
    //
    //     y^2 + z^2 <= r^2
    //     x^2 + z^2 <= R^2
    //
    // The z coordinate is common to both, and it's clear that |z| <= r. We also
    // have the symmetry between z < 0 and z > 0, so we can just integrate over
    // z from 0 to r. For a fixed z, we have:
    //
    //     |y| <= sqrt(r^2 - z^2)
    //     |x| <= sqrt(R^2 - z^2)
    //
    // We again have the 2x2 symmetries here, but otherwise we have a
    // rectangular range of sqrt(r^2 - z^2) x sqrt(R^2 - z^2). Putting
    // everything together, and having the 2x2x2 = 8 symmetries on the front, we
    // simply want to integrate the below:
    //
    //     V = 8 * integral from 0 to r of sqrt((r^2 - z^2)(R^2 - z^2)) dz
    //
    // Afterwards, the integral can be numerically approximated using Simpson's
    // rule or some other approach, or we can go further by evaluating it to the
    // closed form expression involving complete elliptic integrals of the first
    // kind K(.) and second kind E(.). Then we can evaluate it quicker. The full
    // closed form would be:
    //
    //     V = 8/3 R^3 [ (1 + (r/R)^2) E(r/R) - (1 - (r/R)^2) K(r/R)]
    //
    // However, in this problem we have a single test case so going for
    // Simpson's rule is totally fine. As a reminder, Simpson's approximation is
    // essentially choosing N points at equal distances, and then interpolating
    // a quadratic based on f(l), f((l+r)/2), and f(r). After integrating the
    // quadratic, the approximation is that:
    //
    // integral from l to r f(x) dx = [(r - l) / 6] (f(l) + 4 f((l+r)/2) + f(r))
    //
    // In this problem 1M operations are plenty.

    if(r1 > r2) {
        swap(r1, r2);
    }

    auto f = [](double y) {
        return sqrt((r1 * r1 - y * y) * (r2 * r2 - y * y));
    };

    int n = 1000000;
    double h = r1 / n;
    double sum = f(0) + f(r1);

    for(int i = 1; i < n; i++) {
        double y = i * h;
        if(i % 2 == 1) {
            sum += 4 * f(y);
        } else {
            sum += 2 * f(y);
        }
    }

    double vol = 8 * sum * h / 3;
    cout << fixed << setprecision(4) << vol << endl;
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
