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

Point pos_a, vel_a, pos_b, vel_b;
coord_t r_a, m_a, r_b, m_b, t;

void read() {
    cin >> pos_a >> vel_a >> r_a >> m_a;
    cin >> pos_b >> vel_b >> r_b >> m_b;
    cin >> t;
}

void solve() {
    // The two disks move on straight lines until (if ever) they touch, and a
    // touch is the only event that changes a velocity. Working in the frame of
    // disk B, the relative position p = A - B drifts with relative velocity
    // v = vel_a - vel_b, so contact happens when |p + v*s| equals the sum of
    // radii R. Squaring gives the quadratic (v*v)s^2 + 2(p*v)s + (p*p - R^2),
    // whose value is the signed squared gap: it is negative exactly while the
    // disks overlap, so its two roots s1 <= s2 bound the overlap interval and
    // the first contact is the smaller root. The constant term equals the gap
    // at s = 0, which the problem guarantees is positive (no initial overlap),
    // so s = 0 lies outside [s1, s2]: the roots are either both negative (the
    // overlap is entirely in the past, no future hit) or both positive (first
    // contact at s1). The mixed case s1 < 0 < s2 would mean the disks already
    // overlap at s = 0 and is thus excluded; this is also why s2 never matters,
    // as it is only the exit time of a pass-through that the bounce prevents.
    // That bounce reverses the relative velocity along the line of centers,
    // pushing them apart again, so at most one collision can occur in [0, t].
    //
    // If that root lies in (0, t] we slide both disks to the contact instant
    // and resolve the impact along the unit normal n = unit(A - B). Momentum
    // and kinetic energy are conserved by exchanging only the velocity
    // components along n, which for masses m_a, m_b gives the standard
    // vel_a -= 2*m_b/(m_a+m_b) * ((vel_a - vel_b)*n) n and the symmetric
    // update for B. We then coast for the remaining time with the new speeds;
    // if no contact occurs we simply coast for the full duration t.

    coord_t R = r_a + r_b;
    Point p = pos_a - pos_b;
    Point v = vel_a - vel_b;

    coord_t a = v * v;
    coord_t b = 2 * (p * v);
    coord_t c = p * p - R * R;

    coord_t hit = -1;
    if(a > Point::eps) {
        coord_t disc = b * b - 4 * a * c;
        if(disc >= 0) {
            coord_t s = (-b - sqrt(disc)) / (2 * a);
            if(s > Point::eps && s <= t + Point::eps) {
                hit = min(s, t);
            }
        }
    }

    if(hit < 0) {
        pos_a = pos_a + vel_a * t;
        pos_b = pos_b + vel_b * t;
    } else {
        pos_a = pos_a + vel_a * hit;
        pos_b = pos_b + vel_b * hit;

        Point n = (pos_a - pos_b).unit();
        coord_t rel = (vel_a - vel_b) * n;
        vel_a = vel_a - n * (2 * m_b / (m_a + m_b) * rel);
        vel_b = vel_b + n * (2 * m_a / (m_a + m_b) * rel);

        coord_t rest = t - hit;
        pos_a = pos_a + vel_a * rest;
        pos_b = pos_b + vel_b * rest;
    }

    cout << fixed << setprecision(6);
    cout << pos_a << ' ' << vel_a << '\n';
    cout << pos_b << ' ' << vel_b << '\n';
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
