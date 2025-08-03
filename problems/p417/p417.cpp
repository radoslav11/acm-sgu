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

const double pi = acos(-1);

double x, y, r;

void read() { cin >> x >> y >> r; }

void solve() {
    // Compute the mass of a disc centered at (x0, y0) with radius r, density
    // ln(x^2 + y^2). The origin is outside the disc, so s = sqrt(x0^2 + y0^2) >
    // r. The mass is the integral:
    //
    //     M = \int_{x,y \in D} \ln(x^2 + y^2) dx dy
    //
    // Use polar coordinates centered at (x0, y0). A point in the disc is:
    //
    //     x = x0 + D * cos(theta), y = y0 + D * sin(theta), 0 <= D <= r, 0 <=
    //     theta < 2pi.
    //
    // Then, x^2 + y^2 = s^2 + D^2 + 2sD * cos(theta).
    // The integral becomes:
    //
    //     M = \int_{theta=0}^{2pi} \int_{D=0}^r D * \ln(s^2 + D^2 + 2sD *
    //     cos(theta)) dD dtheta
    //
    // Integrate over theta first. For fixed D, let a = s^2 + D^2, b = 2sD.
    // Since s > r >= D:
    //
    //     \int_{0}^{2pi} \ln(a + b * cos(theta)) dtheta = 2pi * \ln(s^2).
    //
    // Thus, the mass is:
    //
    //     M = \int_{0}^r D * 2pi * \ln(s^2) dD = 2pi * \ln(s^2) * (r^2 / 2) =
    //     pi * r^2 * \ln(x0^2 + y0^2).

    double ans = pi * r * r * log(x * x + y * y);
    cout << setprecision(12) << fixed << ans << endl;
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
