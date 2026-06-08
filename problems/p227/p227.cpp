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

struct Arc {
    bool is_segment;
    Point a, b, c, center;
    coord_t r;
};

struct Crossing {
    bool infinite;
    vector<Point> pts;
};

int n;
vector<array<Point, 3>> raw_arcs;

void read() {
    cin >> n;
    raw_arcs.assign(n, {});
    for(auto& t: raw_arcs) {
        cin >> t[0] >> t[1] >> t[2];
    }
}

coord_t norm_2pi(coord_t a) {
    coord_t full = 2 * Point::PI;
    a = fmod(a, full);
    if(a < 0) {
        a += full;
    }

    return a;
}

Arc make_arc(const Point& p0, const Point& p1, const Point& p2) {
    Arc arc;
    arc.a = p0;
    arc.b = p1;
    arc.c = p2;
    if(ccw(p0, p1, p2) == 0) {
        arc.is_segment = true;
    } else {
        arc.is_segment = false;
        arc.center = circumcenter(p0, p1, p2);
        arc.r = (p0 - arc.center).norm();
    }

    return arc;
}

bool on_circle_arc(const Arc& arc, const Point& p) {
    coord_t side_c = (arc.b - arc.a) ^ (arc.c - arc.a);
    coord_t side_p = (arc.b - arc.a) ^ (p - arc.a);
    if(fabs(side_p) < Point::eps) {
        return true;
    }

    return (side_c > 0) == (side_p > 0);
}

pair<coord_t, coord_t> arc_interval(const Arc& arc) {
    coord_t ang_a = (arc.a - arc.center).angle();
    coord_t ang_b = (arc.b - arc.center).angle();
    coord_t ang_c = (arc.c - arc.center).angle();
    coord_t span_b = norm_2pi(ang_b - ang_a);
    coord_t span_c = norm_2pi(ang_c - ang_a);
    if(span_c <= span_b) {
        return {ang_a, span_b};
    }

    return {ang_b, 2 * Point::PI - span_b};
}

coord_t arc_overlap(const Arc& x, const Arc& y) {
    auto interval_x = arc_interval(x);
    auto interval_y = arc_interval(y);
    coord_t start_x = interval_x.first, len_x = interval_x.second;
    coord_t start_y = interval_y.first, len_y = interval_y.second;
    coord_t full = 2 * Point::PI;
    coord_t shift = norm_2pi(start_y - start_x);
    coord_t overlap = 0;

    auto add = [&](coord_t lo, coord_t hi) {
        coord_t l = max(lo, (coord_t)0), r = min(hi, len_x);
        if(r > l) {
            overlap += r - l;
        }
    };

    if(shift + len_y <= full) {
        add(shift, shift + len_y);
    } else {
        add(shift, full);
        add(0, shift + len_y - full);
    }

    return overlap;
}

vector<Point> line_circle(
    const Point& a, const Point& b, const Point& center, coord_t r
) {
    Point dir = (b - a).unit();
    Point foot = a + dir * ((center - a) * dir);
    coord_t dist = (center - foot).norm();
    if(dist > r + Point::eps) {
        return {};
    }

    coord_t h_sq = r * r - dist * dist;
    if(h_sq < 0) {
        h_sq = 0;
    }
    coord_t h = sqrt(h_sq);
    if(h < Point::eps) {
        return {foot};
    }

    return {foot + dir * h, foot - dir * h};
}

Crossing intersect_arcs(const Arc& x, const Arc& y) {
    Crossing res;
    res.infinite = false;

    if(x.is_segment && y.is_segment) {
        Point d1 = x.b - x.a, d2 = y.b - y.a;
        if(fabs(d1 ^ d2) < Point::eps) {
            if(ccw(x.a, x.b, y.a) != 0) {
                return res;
            }

            Point u = d1.unit();
            coord_t len = (x.b - x.a) * u;
            coord_t pos_a = (y.a - x.a) * u;
            coord_t pos_b = (y.b - x.a) * u;
            coord_t lo = max((coord_t)0, min(pos_a, pos_b));
            coord_t hi = min(len, max(pos_a, pos_b));
            if(hi > lo + Point::eps) {
                res.infinite = true;
            } else if(hi > lo - Point::eps) {
                res.pts.push_back(x.a + u * ((lo + hi) / 2));
            }

            return res;
        }

        Point p = line_line_intersection(x.a, x.b, y.a, y.b);
        if(point_on_segment(x.a, x.b, p) && point_on_segment(y.a, y.b, p)) {
            res.pts.push_back(p);
        }

        return res;
    }

    if(x.is_segment != y.is_segment) {
        const Arc& seg = x.is_segment ? x : y;
        const Arc& cir = x.is_segment ? y : x;
        for(const Point& p: line_circle(seg.a, seg.b, cir.center, cir.r)) {
            if(point_on_segment(seg.a, seg.b, p) && on_circle_arc(cir, p)) {
                res.pts.push_back(p);
            }
        }

        return res;
    }

    if((x.center - y.center).norm() < 1e-6 && fabs(x.r - y.r) < 1e-6) {
        if(arc_overlap(x, y) > 1e-7) {
            res.infinite = true;
            return res;
        }

        for(const Point& p: {x.a, x.b, y.a, y.b}) {
            if(on_circle_arc(x, p) && on_circle_arc(y, p)) {
                res.pts.push_back(p);
            }
        }

        return res;
    }

    for(const Point& p: intersect_circles(x.center, x.r, y.center, y.r)) {
        if(on_circle_arc(x, p) && on_circle_arc(y, p)) {
            res.pts.push_back(p);
        }
    }

    return res;
}

void solve() {
    // Each arc is the circle through its three points, restricted to the side
    // of the chord (first two points) that contains the third, intermediate
    // point. So a point lying on the supporting circle belongs to the arc iff
    // it is on the same side of the chord as the intermediate point, with the
    // two endpoints included. If the three points are collinear the supporting
    // circle degenerates and the arc is just the segment between its endpoints,
    // which we treat as a separate case.
    //
    // We gather all crossing points by looking at every pair of arcs. For two
    // arcs on different circles we take the (at most two) circle-circle
    // intersection points and keep those that lie on both arcs. A segment and
    // an arc reduce to line-circle intersection filtered the same way, and two
    // segments reduce to segment-segment intersection.
    //
    // The number of crossings is infinite exactly when two arcs share a whole
    // sub-arc: two segments overlapping along a common sub-segment, or two
    // co-circular arcs whose angular ranges overlap on more than isolated
    // points. For co-circular arcs we build the CCW angular interval of each
    // (the one passing through the intermediate point) and measure their
    // overlap length on the circle; a positive overlap means infinitely many
    // crossings, otherwise the only crossings are shared endpoints.
    //
    // Finally we drop duplicate points (the same crossing is found by several
    // pairs, and distinct crossings are guaranteed to be at least 0.005 apart),
    // then sort by x and then y for output.

    vector<Arc> arcs;
    for(auto& t: raw_arcs) {
        arcs.push_back(make_arc(t[0], t[1], t[2]));
    }

    bool infinite = false;
    vector<Point> pts;
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            Crossing res = intersect_arcs(arcs[i], arcs[j]);
            if(res.infinite) {
                infinite = true;
            }
            for(const Point& p: res.pts) {
                pts.push_back(p);
            }
        }
    }

    if(infinite) {
        cout << "Infinity\n";
        return;
    }

    vector<Point> uniq;
    for(const Point& p: pts) {
        bool dup = false;
        for(const Point& q: uniq) {
            if((p - q).norm() < 1e-3) {
                dup = true;
                break;
            }
        }
        if(!dup) {
            uniq.push_back(p);
        }
    }

    sort(uniq.begin(), uniq.end(), [](const Point& p, const Point& q) {
        if(fabs(p.x - q.x) > 1e-6) {
            return p.x < q.x;
        }

        return p.y < q.y;
    });

    cout << uniq.size() << '\n';
    cout << fixed << setprecision(3);
    for(const Point& p: uniq) {
        coord_t ox = fabs(p.x) < 5e-4 ? 0 : p.x;
        coord_t oy = fabs(p.y) < 5e-4 ? 0 : p.y;
        cout << ox << ' ' << oy << '\n';
    }
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
