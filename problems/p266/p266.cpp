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

using coord3_t = double;

const coord3_t PI = acos((coord3_t)-1.0);

struct Point3 {
    coord3_t x, y, z;

    coord3_t dot(const Point3& p) const { return x * p.x + y * p.y + z * p.z; }
    coord3_t norm() const { return sqrt(dot(*this)); }

    friend istream& operator>>(istream& in, Point3& p) {
        return in >> p.x >> p.y >> p.z;
    }
};

coord3_t R;
Point3 p1, p2;

void read() { cin >> R >> p1 >> p2; }

void solve() {
    // A scanner at distance d from the center sees a spherical cap. The
    // tangent points form a circle lying in the plane perpendicular to the
    // line center-scanner at distance R^2 / d from the center, so in terms
    // of the unit vector u toward the scanner the cap is
    //
    //     { x on sphere : (x / R) . u >= R / d },
    //
    // i.e. a cap of half-angle a = acos(R / d) and area 2 pi R^2 (1 - R/d).
    //
    // By inclusion-exclusion,
    //
    //     answer = area(cap_1) + area(cap_2) - area(cap_1 cap cap_2),
    //
    // so we only need the intersection. On the sphere, the intersection of
    // two caps is a "lens" bounded by two small-circle arcs meeting at the
    // two tangent-circle crossings. Let a, b be the cap half-angles and t
    // the angle between their axes u_a, u_b.
    //
    //   1) Each arc spans a half-angle psi at its pole. Restricting cap a's
    //      boundary (x . u_a = cos a) to also lie in cap b gives
    //
    //          cos(psi_a) = (cos b - cos a cos t) / (sin a sin t),
    //          cos(psi_b) = (cos a - cos b cos t) / (sin b sin t).
    //
    //   2) Each lens corner P is the apex of the spherical triangle
    //      (P, u_a, u_b), so the spherical law of cosines gives the angle g
    //      between the two boundary tangents at P:
    //
    //          cos(g) = (cos t - cos a cos b) / (sin a sin b).
    //
    //      The interior angle of the lens at P is then pi - g.
    //
    //   3) Apply Gauss-Bonnet on the unit sphere (K = 1). A small circle of
    //      half-angle r has geodesic curvature cot r, and an arc spanning
    //      2 psi at its pole has length 2 psi sin r, so the integral of
    //      k_g ds over each arc is 2 psi cos r. With two corners of
    //      interior angle pi - g (i.e. exterior angle g) the theorem reads
    //
    //          area + 2 psi_a cos a + 2 psi_b cos b + 2 g = 2 pi,
    //
    //      hence on a sphere of radius R
    //
    //          area(lens) = R^2 (2(pi - g) - 2 psi_a cos a - 2 psi_b cos b).
    //
    // The disjoint / nested cases are handled explicitly up front; otherwise
    // we clamp the cosines into [-1, 1] before acos for numerical safety.

    coord3_t d1 = p1.norm(), d2 = p2.norm();
    coord3_t cos_a = R / d1, cos_b = R / d2;
    coord3_t sin_a = sqrt(max(0.0, 1.0 - cos_a * cos_a));
    coord3_t sin_b = sqrt(max(0.0, 1.0 - cos_b * cos_b));
    coord3_t alpha = acos(cos_a), beta = acos(cos_b);

    coord3_t cos_t = p1.dot(p2) / (d1 * d2);
    cos_t = max((coord3_t)-1.0, min((coord3_t)1.0, cos_t));
    coord3_t sin_t = sqrt(max(0.0, 1.0 - cos_t * cos_t));
    coord3_t theta = acos(cos_t);

    coord3_t area1 = 2 * PI * R * R * (1 - cos_a);
    coord3_t area2 = 2 * PI * R * R * (1 - cos_b);

    coord3_t intersection = 0.0;
    if(theta >= alpha + beta) {
        intersection = 0.0;
    } else if(theta + min(alpha, beta) <= max(alpha, beta)) {
        intersection = 2 * PI * R * R * (1 - (alpha < beta ? cos_a : cos_b));
    } else {
        auto clamp_cos = [](coord3_t c) {
            return max((coord3_t)-1.0, min((coord3_t)1.0, c));
        };
        coord3_t psi_a =
            acos(clamp_cos((cos_b - cos_a * cos_t) / (sin_a * sin_t)));
        coord3_t psi_b =
            acos(clamp_cos((cos_a - cos_b * cos_t) / (sin_b * sin_t)));
        coord3_t gamma =
            acos(clamp_cos((cos_t - cos_a * cos_b) / (sin_a * sin_b)));
        intersection =
            R * R * (2 * (PI - gamma) - 2 * psi_a * cos_a - 2 * psi_b * cos_b);
    }

    cout << fixed << setprecision(3) << area1 + area2 - intersection << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
