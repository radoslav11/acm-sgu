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

Point cen, A, B;
coord_t R;

void read() { cin >> cen >> R >> A >> B; }

void solve() {
    // One way to solve this is to split it into two cases - point C is "below"
    // the line AB, and another one for above. Then do a ternary search. There
    // is also a closed form solution which we describe here. By the inscribed
    // angle theorem, if C lies on a circle through A and B of radius r, then
    // sin(ACB) = |AB| / (2r). So for a fixed chord AB, the angle ACB depends
    // only on the radius of the circle through A, B, C, and on which side of AB
    // the point C lies. Because we have sin(.), we want to maximize it, or we
    // want to minimize the radius of the ACB circle. Among circles through A
    // and B that intersect the given circle, sliding the center along the
    // perpendicular bisector of AB changes the radius continuously; the
    // extremal values of the inscribed angle at the intersection point occur
    // exactly when the circle through A, B, C is tangent to the given circle.
    // So the candidates for C are the (at most two) tangency points of circles
    // through A, B with the given circle.
    //
    // Translate so that the center of the given circle is at the origin. Let
    // M = (A + B) / 2, V = (B - A).perp() so that |V| = |AB| = D. Centers of
    // circles through A and B are parameterized by P(t) = M + t V, with
    // radius:
    //
    //     r(t) = D * sqrt(t^2 + 1/4)
    //
    // (since |P(t) - A|^2 = D^2/4 + t^2 D^2 as M - A is perpendicular to V).
    // Tangency with the given circle is equivalent to |OP(t)| = |R +/- r(t)|.
    // Writing |OP(t)|^2 = |M|^2 + 2 t (M . V) + t^2 D^2 and squaring after
    // isolating the radical gives the quadratic
    //
    //     (b^2 - 4 R^2 D^2) t^2 + 2 b q t + (q^2 - R^2 D^2) = 0,
    //
    // where b = 2 (M . V) and q = |M|^2 - R^2 - D^2/4. For each real root t
    // compute P = M + t V and r = D sqrt(t^2 + 1/4); the tangency point lies
    // on line OP at distance R from O, so it is one of +/- R * P/|P|. Keep
    // the one that satisfies |C - P| ~= r, and among the valid candidates
    // pick the one with the smallest cos(ACB).

    Point Oshift = cen;
    Point Al = A - Oshift, Bl = B - Oshift;

    Point M = (Al + Bl) / 2.0;
    Point V = (Bl - Al).perp();
    coord_t D = (Bl - Al).norm();
    coord_t Dsq = D * D;

    coord_t b = 2.0 * (M * V);
    coord_t q = M.norm2() - R * R - Dsq / 4.0;

    coord_t qa = b * b - 4.0 * R * R * Dsq;
    coord_t qb = 2.0 * b * q;
    coord_t qc = q * q - R * R * Dsq;

    vector<coord_t> ts;
    if(fabs(qa) < 1e-18) {
        if(fabs(qb) > 1e-18) {
            ts.push_back(-qc / qb);
        }
    } else {
        coord_t disc = qb * qb - 4.0 * qa * qc;
        if(disc < 0) {
            disc = 0;
        }
        coord_t sq = sqrt(disc);
        ts.push_back((-qb + sq) / (2.0 * qa));
        ts.push_back((-qb - sq) / (2.0 * qa));
    }

    Point best_C;
    coord_t best_cos = 2.0;
    for(coord_t t: ts) {
        Point P = M + V * t;
        coord_t r = D * sqrt(t * t + 0.25);
        coord_t pnorm = P.norm();
        if(pnorm < Point::eps) {
            continue;
        }
        for(int sign: {1, -1}) {
            Point C = P * (sign * R / pnorm);
            if(fabs((C - P).norm() - r) > 1e-6) {
                continue;
            }
            Point ua = Al - C, ub = Bl - C;
            coord_t na = ua.norm(), nb = ub.norm();
            if(na < Point::eps || nb < Point::eps) {
                continue;
            }
            coord_t cosv = (ua * ub) / (na * nb);
            if(cosv < best_cos) {
                best_cos = cosv;
                best_C = C;
            }
        }
    }

    Point ans = best_C + Oshift;
    cout << fixed << setprecision(6) << ans.x << ' ' << ans.y << '\n';
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
