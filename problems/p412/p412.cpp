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

namespace fastin {
const int BUF = 1 << 16;
char buf[BUF];
int pos = 0, len = 0;

char gc() {
    if(pos == len) {
        len = (int)fread(buf, 1, BUF, stdin);
        pos = 0;
        if(len == 0) {
            return -1;
        }
    }
    return buf[pos++];
}

long long read_int() {
    char c = gc();
    while(c != '-' && (c < '0' || c > '9')) {
        c = gc();
    }
    bool neg = c == '-';
    if(neg) {
        c = gc();
    }
    long long x = 0;
    while(c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = gc();
    }
    return neg ? -x : x;
}
}  // namespace fastin

int n, m;
vector<Point> poly;
vector<pair<Point, Point>> shelves;

void read() {
    n = fastin::read_int();
    m = fastin::read_int();
    poly.resize(n);
    for(auto& p: poly) {
        p.x = fastin::read_int();
        p.y = fastin::read_int();
    }
    shelves.resize(m);
    for(auto& [a, b]: shelves) {
        a.x = fastin::read_int();
        a.y = fastin::read_int();
        b.x = fastin::read_int();
        b.y = fastin::read_int();
    }
}

void solve() {
    // The lamp at the origin lies strictly inside the convex polygon, so a
    // ray cast in direction theta hits the boundary in exactly one point,
    // giving a bijection between angles theta in [0, 2*pi) and wall points.
    // A wall point is shaded exactly when its ray is blocked by some shelf,
    // so the answer is the boundary length of the wall points whose angle
    // lies in the union of the angular intervals occupied by the shelves.
    //
    // Each shelf is a segment that does not pass through the origin, so as
    // seen from the origin it spans the minor arc (less than pi) between the
    // angles of its endpoints; the sign of the cross product of the two
    // endpoint vectors tells us which of the two is the counter-clockwise
    // start of that arc. We cut every arc at angle 0 so all blocked
    // intervals live in [0, 2*pi), then sort and merge them.
    //
    // To turn a blocked angular interval into a wall length we sort the
    // polygon vertices by angle. Because the polygon is convex and the
    // origin is inside, this is just a rotation of the input order, so
    // consecutive sorted vertices stay adjacent on the boundary. Walking
    // the boundary counter-clockwise from the first sorted vertex, the
    // travelled distance s(theta) to the point hit at angle theta is a
    // monotone function, precomputed via prefix sums of edge lengths plus
    // the offset of the ray-edge intersection inside the current edge. The
    // contribution of a blocked interval [a, b] is then s(b) - s(a), with
    // intervals that straddle the anchor angle split across the wrap.

    auto norm_angle = [](coord_t a) {
        if(a < 0) {
            a += 2 * Point::PI;
        }
        return a;
    };

    vector<pair<coord_t, Point>> by_angle(n);
    for(int i = 0; i < n; i++) {
        by_angle[i] = {norm_angle(poly[i].angle()), poly[i]};
    }
    sort(by_angle.begin(), by_angle.end(), [](const auto& p, const auto& q) {
        return p.first < q.first;
    });

    vector<coord_t> bound(n + 1), pref(n + 1, 0);
    for(int i = 0; i < n; i++) {
        poly[i] = by_angle[i].second;
        bound[i] = by_angle[i].first;
        if(i > 0) {
            pref[i] = pref[i - 1] + (poly[i] - poly[i - 1]).norm();
        }
    }
    bound[n] = bound[0] + 2 * Point::PI;
    coord_t perim = pref[n - 1] + (poly[0] - poly[n - 1]).norm();

    coord_t g0 = bound[0];
    auto arc_length = [&](coord_t theta) {
        int i =
            upper_bound(bound.begin(), bound.end(), theta) - bound.begin() - 1;
        i = min(i, n - 1);
        Point dir(cos(theta), sin(theta));
        Point hit = line_line_intersection(
            Point(0, 0), dir, poly[i], poly[(i + 1) % n]
        );
        return pref[i] + (hit - poly[i]).norm();
    };

    vector<pair<coord_t, coord_t>> arcs;
    auto add_arc = [&](coord_t s, coord_t e) {
        if(s <= e) {
            arcs.emplace_back(s, e);
        } else {
            arcs.emplace_back(s, 2 * Point::PI);
            arcs.emplace_back(0, e);
        }
    };

    for(auto& [a, b]: shelves) {
        coord_t cross = a ^ b;
        if(fabs(cross) < Point::eps) {
            continue;
        }
        coord_t sa = norm_angle(a.angle()), sb = norm_angle(b.angle());
        if(cross > 0) {
            add_arc(sa, sb);
        } else {
            add_arc(sb, sa);
        }
    }

    sort(arcs.begin(), arcs.end());

    coord_t ans = 0;
    coord_t cur_lo = 0, cur_hi = -1;
    auto flush = [&]() {
        if(cur_hi < cur_lo) {
            return;
        }
        if(cur_lo < g0 - Point::eps && cur_hi > g0 + Point::eps) {
            ans += (perim - arc_length(cur_lo + 2 * Point::PI)) +
                   arc_length(cur_hi);
        } else if(cur_hi <= g0 + Point::eps) {
            ans += arc_length(cur_hi + 2 * Point::PI) -
                   arc_length(cur_lo + 2 * Point::PI);
        } else {
            ans += arc_length(cur_hi) - arc_length(cur_lo);
        }
    };

    for(auto& [s, e]: arcs) {
        if(cur_hi < cur_lo) {
            cur_lo = s;
            cur_hi = e;
        } else if(s <= cur_hi + Point::eps) {
            cur_hi = max(cur_hi, e);
        } else {
            flush();
            cur_lo = s;
            cur_hi = e;
        }
    }
    flush();

    cout << fixed << setprecision(12) << ans << '\n';
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
