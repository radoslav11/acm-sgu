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

long double b1, t1, b2, t2, l, ds, df, g;

bool read() {
    cin >> b1 >> t1 >> b2 >> t2 >> l >> ds >> df >> g;
    return !cin.fail();
}

void solve() {
    // The free variables are the landing point p in (0, l) between the
    // walls and the two hole-crossing heights h_1 in [b1, t1] and h_2 in
    // [b2, t2]. The jumper has a single max-speed v that must cover both
    // jumps, so for a given p the smallest feasible v is max(v1(p), v2(p))
    // with v_i(p) the minimum speed for the i-th jump. For a fixed p the
    // two jumps are independent and we minimise each v_i over its h_i in
    // closed form; that leaves p, which couples the jumps (moving p right
    // lengthens jump 1 and shortens jump 2) and we handle by ternary
    // searching
    //
    //     min over p of max(v1(p), v2(p)).
    //
    // Consider a jump from (0, 0) over the hole at (d1, h) to (d1 + d2, 0).
    // We pick coordinates so launch and landing both have y = 0, so the
    // trajectory is a parabola with roots at x = 0 and x = R := d1 + d2.
    // Any such parabola has the form
    //
    //     y(x) = a * x * (R - x),
    //
    // which already bakes in the two ground crossings; only the single
    // shape parameter a is left to fix. We fix it from the hole point:
    // y(d1) = a * d1 * d2 = h, so a = h / (d1 * d2).
    //
    // To turn this geometric parabola into a physical trajectory, write
    // the projectile motion x(t) = v_x * t, y(t) = v_y * t - g * t^2 / 2,
    // eliminate t via t = x / v_x:
    //
    //     y(x) = (v_y / v_x) * x  -  (g / (2 * v_x^2)) * x^2.
    //
    // Matching the x^2 coefficient with a * x * (R - x) = a * R * x - a * x^2
    // gives a = g / (2 * v_x^2), i.e. v_x^2 = g / (2 * a). And matching the
    // linear coefficient gives v_y / v_x = a * R, i.e. tan(theta) = a * R.
    // Then
    //
    //     v^2 = v_x^2 + v_y^2 = v_x^2 * (1 + tan^2 theta)
    //         = g / (2 * a) + g * a * R^2 / 2
    //         = A / h + B * h,
    //
    //     A = g * d1 * d2 / 2,   B = g * (d1 + d2)^2 / (2 * d1 * d2).
    //
    // f(h) = A / h + B * h is convex on h > 0 with unconstrained minimum
    // at h_star = sqrt(A / B) = d1 * d2 / (d1 + d2) and minimum value
    // g * (d1 + d2). The hole forces h in [b, t], so the optimal h is
    // clamp(h_star, b, t) and v_i(p)^2 = f at that h.
    //
    // Each v_i(p) is U-shaped in p (constraint-bound for extreme p,
    // unconstrained = g * (d1 + d2) in the middle), and v1, v2 move in
    // opposite directions with p, so max(v1, v2) is unimodal and ternary
    // search on p in (0, l) finds the optimum. -1 is never needed: every
    // jump is feasible for sufficiently large v since b > 0.

    auto min_v2_jump = [](long double d1, long double d2, long double b,
                          long double t) {
        long double A = g * d1 * d2 / 2.0L;
        long double B = g * (d1 + d2) * (d1 + d2) / (2.0L * d1 * d2);
        long double h = max(b, min(t, d1 * d2 / (d1 + d2)));
        return A / h + B * h;
    };

    auto cost = [&](long double p) {
        return max(min_v2_jump(ds, p, b1, t1), min_v2_jump(l - p, df, b2, t2));
    };

    long double lo = 0.0L, hi = l;
    for(int iter = 0; iter < 300; iter++) {
        long double m1 = lo + (hi - lo) / 3.0L;
        long double m2 = hi - (hi - lo) / 3.0L;
        if(cost(m1) < cost(m2)) {
            hi = m2;
        } else {
            lo = m1;
        }
    }

    cout << sqrtl(cost((lo + hi) / 2.0L)) << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(6);

    while(read()) {
        solve();
    }

    return 0;
}
