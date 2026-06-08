#include <bits/stdc++.h>
// #include <coding_library/geometry/point.hpp>

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

using coord_t = double;

struct Point {
    static constexpr coord_t eps = 1e-9;
    static inline const coord_t PI = acos((coord_t)-1.0);

    coord_t x, y;
    Point(coord_t x = 0, coord_t y = 0) : x(x), y(y) {}

    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(coord_t c) const { return Point(x * c, y * c); }
    Point operator/(coord_t c) const { return Point(x / c, y / c); }

    coord_t operator*(const Point& p) const { return x * p.x + y * p.y; }
    coord_t operator^(const Point& p) const { return x * p.y - y * p.x; }

    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point& p) const { return x != p.x || y != p.y; }
    bool operator<(const Point& p) const {
        return x != p.x ? x < p.x : y < p.y;
    }
    bool operator>(const Point& p) const {
        return x != p.x ? x > p.x : y > p.y;
    }
    bool operator<=(const Point& p) const {
        return x != p.x ? x < p.x : y <= p.y;
    }
    bool operator>=(const Point& p) const {
        return x != p.x ? x > p.x : y >= p.y;
    }

    coord_t norm2() const { return x * x + y * y; }
    coord_t norm() const { return sqrt(norm2()); }
    coord_t angle() const { return atan2(y, x); }

    Point rotate(coord_t a) const {
        return Point(x * cos(a) - y * sin(a), x * sin(a) + y * cos(a));
    }

    Point perp() const { return Point(-y, x); }
    Point unit() const { return *this / norm(); }
    Point normal() const { return perp().unit(); }
    Point project(const Point& p) const {
        return *this * (*this * p) / norm2();
    }
    Point reflect(const Point& p) const {
        return *this * 2 * (*this * p) / norm2() - p;
    }

    friend ostream& operator<<(ostream& os, const Point& p) {
        return os << p.x << ' ' << p.y;
    }
    friend istream& operator>>(istream& is, Point& p) {
        return is >> p.x >> p.y;
    }

    friend int ccw(const Point& a, const Point& b, const Point& c) {
        coord_t v = (b - a) ^ (c - a);
        if(-eps <= v && v <= eps) {
            return 0;
        } else if(v > 0) {
            return 1;
        } else {
            return -1;
        }
    }

    friend bool point_on_segment(
        const Point& a, const Point& b, const Point& p
    ) {
        return ccw(a, b, p) == 0 && p.x >= min(a.x, b.x) - eps &&
               p.x <= max(a.x, b.x) + eps && p.y >= min(a.y, b.y) - eps &&
               p.y <= max(a.y, b.y) + eps;
    }

    friend bool point_in_triangle(
        const Point& a, const Point& b, const Point& c, const Point& p
    ) {
        int d1 = ccw(a, b, p);
        int d2 = ccw(b, c, p);
        int d3 = ccw(c, a, p);
        return (d1 >= 0 && d2 >= 0 && d3 >= 0) ||
               (d1 <= 0 && d2 <= 0 && d3 <= 0);
    }

    friend Point line_line_intersection(
        const Point& a1, const Point& b1, const Point& a2, const Point& b2
    ) {
        return a1 +
               (b1 - a1) * ((a2 - a1) ^ (b2 - a2)) / ((b1 - a1) ^ (b2 - a2));
    }

    friend bool collinear(const Point& a, const Point& b) {
        return abs(a ^ b) < eps;
    }

    friend Point circumcenter(const Point& a, const Point& b, const Point& c) {
        Point mid_ab = (a + b) / 2.0;
        Point mid_ac = (a + c) / 2.0;
        Point perp_ab = (b - a).perp();
        Point perp_ac = (c - a).perp();
        return line_line_intersection(
            mid_ab, mid_ab + perp_ab, mid_ac, mid_ac + perp_ac
        );
    }

    friend coord_t arc_area(
        const Point& center, coord_t r, const Point& p1, const Point& p2
    ) {
        coord_t theta1 = (p1 - center).angle();
        coord_t theta2 = (p2 - center).angle();
        if(theta2 < theta1 - eps) {
            theta2 += 2 * PI;
        }

        coord_t d_theta = theta2 - theta1;
        coord_t cx = center.x, cy = center.y;
        coord_t area = r * cx * (sin(theta2) - sin(theta1)) -
                       r * cy * (cos(theta2) - cos(theta1)) + r * r * d_theta;
        return area / 2.0;
    }

    friend vector<Point> intersect_circles(
        const Point& c1, coord_t r1, const Point& c2, coord_t r2
    ) {
        Point d = c2 - c1;
        coord_t dist = d.norm();

        if(dist > r1 + r2 + eps || dist < abs(r1 - r2) - eps || dist < eps) {
            return {};
        }

        coord_t a = (r1 * r1 - r2 * r2 + dist * dist) / (2 * dist);
        coord_t h_sq = r1 * r1 - a * a;
        if(h_sq < -eps) {
            return {};
        }
        if(h_sq < 0) {
            h_sq = 0;
        }
        coord_t h = sqrt(h_sq);

        Point mid = c1 + d.unit() * a;
        Point perp_dir = d.perp().unit();

        if(h < eps) {
            return {mid};
        }
        return {mid + perp_dir * h, mid - perp_dir * h};
    }

    friend optional<Point> intersect_ray_segment(
        const Point& ray_start, const Point& ray_through, const Point& seg_a,
        const Point& seg_b
    ) {
        Point ray_dir = ray_through - ray_start;
        if(ray_dir.norm2() < Point::eps) {
            return {};
        }
        Point seg_dir = seg_b - seg_a;
        coord_t denom = ray_dir ^ seg_dir;
        if(fabs(denom) < eps) {
            return {};
        }
        coord_t t = ((seg_a - ray_start) ^ seg_dir) / denom;
        if(t < eps) {
            return {};
        }
        coord_t s = ((seg_a - ray_start) ^ ray_dir) / denom;
        if(s < eps || s > 1 - eps) {
            return {};
        }
        return ray_start + ray_dir * t;
    }
};

Point A, B, center;
coord_t R, D;

void read() { cin >> A >> B >> center >> R >> D; }

struct Part {
    char type;
    Point a, b;
};

void solve() {
    // We travel from A to B; the disc may be entered, but the total length of
    // route lying inside it must not exceed the fuel budget D. If the straight
    // segment A-B stays outside the disc, or the chord it cuts through the disc
    // has length at most D, then that single segment is already the answer.
    //
    // Otherwise the optimum is a taut path that detours around the crater on
    // one of the two sides. Around a convex obstacle a taut path leaves A along
    // a tangent, hugs the boundary, and reaches B along a tangent. The fuel
    // lets us replace one sub-arc of the boundary by a straight chord. A chord
    // of length D always subtends the same central angle 2*asin(D/2R), so the
    // saving (arc length minus the chord) is identical wherever the chord sits,
    // and one chord is never worse than splitting the budget into several. Thus
    // while the cut-out arc still fits inside the tangent-to-tangent arc Phi,
    // the route is tangent, chord of length D, remaining arc, tangent, and we
    // are free to place the chord right after A's tangent point.
    //
    // When D is large enough that the chord would subtend more than Phi, the
    // boundary no longer pins the touch points: the path degenerates to
    // A -> P -> Q -> B where P and Q sit on the circle exactly the chord angle
    // apart and the two outer segments must not re-enter the disc. There we
    // minimise |A-P| + D + |Q-B| over the single free angle by a dense scan
    // refined with ternary search, treating angles whose outer segments clip
    // the disc as infeasible.
    //
    // We build all three shapes (pure detour, tangent+chord+arc, and the free
    // two-tangent chord) for both sides and keep the shortest. Each emitted arc
    // spans less than 180 degrees, so naming only its endpoints is unambiguous.

    const coord_t INF = 1e18;

    auto on_circle = [&](coord_t ang) {
        return center + Point(cos(ang), sin(ang)) * R;
    };

    auto seg_ok = [&](const Point& p0, const Point& p1) {
        Point d = p1 - p0;
        coord_t dn = d.norm2();
        coord_t t = 0;
        if(dn > Point::eps) {
            t = ((center - p0) * d) / dn;
            t = max((coord_t)0, min((coord_t)1, t));
        }
        Point cl = p0 + d * t;
        return (cl - center).norm() >= R - 1e-7;
    };

    auto print = [&](const vector<Part>& parts) {
        vector<Part> p;
        for(const auto& q: parts) {
            if((q.a - q.b).norm() > 1e-9) {
                p.push_back(q);
            }
        }

        cout << fixed << setprecision(8);
        cout << p.size() << '\n';
        for(const auto& q: p) {
            cout << q.type << ' ' << q.a.x << ' ' << q.a.y << ' ' << q.b.x
                 << ' ' << q.b.y << '\n';
        }
    };

    if(seg_ok(A, B)) {
        print({{'S', A, B}});
        return;
    }

    coord_t h = fabs((B - A) ^ (center - A)) / (B - A).norm();
    coord_t chord_ab = 2 * sqrt(max((coord_t)0, R * R - h * h));
    if(chord_ab <= D + 1e-9) {
        print({{'S', A, B}});
        return;
    }

    coord_t dA = (A - center).norm(), dB = (B - center).norm();
    coord_t tA = sqrt(dA * dA - R * R), tB = sqrt(dB * dB - R * R);
    coord_t psiA = (A - center).angle(), psiB = (B - center).angle();
    coord_t gA = acos(R / dA), gB = acos(R / dB);
    coord_t theta_d = 2 * asin(min((coord_t)1, D / (2 * R)));

    coord_t best_len = INF;
    vector<Part> best;
    auto consider = [&](coord_t len, const vector<Part>& parts) {
        if(len < best_len) {
            best_len = len;
            best = parts;
        }
    };

    for(int s: {1, -1}) {
        coord_t a1 = psiA + s * gA;
        coord_t a2 = psiB - s * gB;
        coord_t Phi = fmod(s * (a2 - a1), 2 * Point::PI);
        if(Phi < 0) {
            Phi += 2 * Point::PI;
        }

        Point Ta = on_circle(a1), Tb = on_circle(a2);

        consider(
            tA + tB + R * Phi, {{'S', A, Ta}, {'A', Ta, Tb}, {'S', Tb, B}}
        );

        if(theta_d <= Phi + 1e-12) {
            Point P2 = on_circle(a1 + s * theta_d);
            consider(
                tA + tB + R * (Phi - theta_d) + D,
                {{'S', A, Ta}, {'S', Ta, P2}, {'A', P2, Tb}, {'S', Tb, B}}
            );
        }

        auto g = [&](coord_t al) -> coord_t {
            Point P = on_circle(al), Q = on_circle(al + s * theta_d);
            if(!seg_ok(A, P) || !seg_ok(Q, B)) {
                return INF;
            }
            return (A - P).norm() + D + (Q - B).norm();
        };

        coord_t cB = psiB - s * theta_d;
        coord_t d = atan2(sin(cB - psiA), cos(cB - psiA));
        coord_t lo = max(-gA, d - gB), hi = min(gA, d + gB);
        if(lo <= hi) {
            lo += psiA;
            hi += psiA;
            for(int it = 0; it < 200; it++) {
                coord_t m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
                if(g(m1) < g(m2)) {
                    hi = m2;
                } else {
                    lo = m1;
                }
            }

            coord_t al = (lo + hi) / 2, c = g(al);
            if(c < INF / 2) {
                Point P = on_circle(al), Q = on_circle(al + s * theta_d);
                consider(c, {{'S', A, P}, {'S', P, Q}, {'S', Q, B}});
            }
        }
    }

    print(best);
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
