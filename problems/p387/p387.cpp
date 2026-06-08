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

int n;
vector<pair<Point, Point>> segs;

void read() {
    cin >> n;
    segs.resize(n);
    for(auto& [a, b]: segs) {
        cin >> a >> b;
    }
}

Point rotate_cw(Point p, int k) {
    k = ((k % 4) + 4) % 4;
    for(int i = 0; i < k; i++) {
        p = Point(p.y, -p.x);
    }
    return p;
}

vector<Point> segment_segment_intersection(
    const Point& a, const Point& b, const Point& c, const Point& d
) {
    const coord_t eps = 1e-9;
    Point ab = b - a, cd = d - c;
    coord_t denom = ab ^ cd;
    if(fabs(denom) > eps) {
        coord_t t = ((c - a) ^ cd) / denom;
        coord_t s = ((c - a) ^ ab) / denom;
        if(t >= -eps && t <= 1 + eps && s >= -eps && s <= 1 + eps) {
            return {a + ab * t};
        }
        return {};
    }
    if(fabs((c - a) ^ ab) > eps) {
        return {};
    }
    coord_t len2 = ab.norm2();
    coord_t tc = ((c - a) * ab) / len2;
    coord_t td = ((d - a) * ab) / len2;
    if(tc > td) {
        swap(tc, td);
    }
    coord_t lo = max(0.0, tc), hi = min(1.0, td);
    if(hi < lo - eps) {
        return {};
    }
    Point P = a + ab * lo, Q = a + ab * hi;
    if(hi - lo < eps) {
        return {P};
    }
    return {P, Q};
}

void solve() {
    // The field is a square centered at the origin with first base at some
    // point P = (x, y). The other three bases are R(P), R^2(P), R^3(P) where
    // R(x, y) = (-y, x) is the 90-degree CCW rotation around the origin. The
    // diagonal of the square is 2*|P|, so its area is 2*(x^2 + y^2). Let U be
    // the union of all n input segments; P is valid iff P, R(P), R^2(P),
    // R^3(P) all lie in U, which is the same as
    //
    //     P \in V = U \cap R^{-1}(U) \cap R^{-2}(U) \cap R^{-3}(U).
    //
    // We need the expected area of the square when P is drawn uniformly from
    // V. In general V is either a 1-dimensional set (integrate by arc length)
    // or a finite set of isolated points (average over the points).
    //
    // Per-segment reduction. Since V \subseteq U and the input segments don't
    // overlap, process each S_i = [A, B] independently with parameter t in [0,
    // 1], P(t) = A + t*(B - A), arc-length element L_i dt. For each k = 1, 2, 3
    // the condition P(t) in R^{-k}(U) is equivalent to P(t) lying on
    // R^{-k}(S_j) for some j. For each j compute the segment-segment
    // intersection of S_i with R^{-k}(S_j), project it back onto the parameter
    // of S_i, and get either nothing, a single t, or a subinterval
    // [t_lo, t_hi] (the latter only in the collinear-overlap case). Union all
    // such pieces across j and merge into disjoint intervals E_k \subseteq [0,
    // 1].
    //
    // Sweep. Take all interval endpoints from E_1, E_2, E_3 plus {0, 1}, sort
    // and dedupe into candidate values t_0 < t_1 < ... < t_m. Between two
    // consecutive candidates the membership in each E_k is constant, so test
    // the midpoint against all three lists; if it lies in all of them the
    // whole open interval lies in V. For the 1D accumulator write
    //
    //     r^2(t) = q_a t^2 + q_b t + q_c
    //
    // with q_a = dx^2 + dy^2, q_b = 2(A_x dx + A_y dy), q_c = A_x^2 + A_y^2,
    // and add
    //
    //     length     += (t_b - t_a) * L_i
    //     integral   += 2 * L_i * [q_a/3 * t^3 + q_b/2 * t^2 + q_c * t]
    //                   from t_a to t_b
    //
    // (area density is 2 * r^2). For the 0D accumulator, for each candidate t
    // itself test membership in all three E_k; if yes record P(t).
    //
    // Final answer. If the accumulated length is positive, output
    // integral / length. Otherwise dedupe the collected points (Euclidean
    // tolerance), compute 2 * (x^2 + y^2) for each, and output the arithmetic
    // mean.

    coord_t total_length = 0, total_integral = 0;
    vector<Point> discrete_points;

    for(auto& [a, b]: segs) {
        Point ab = b - a;
        coord_t L = ab.norm();
        if(L < 1e-12) {
            continue;
        }
        coord_t len2 = ab.norm2();

        vector<vector<pair<coord_t, coord_t>>> E(3);
        for(int k = 1; k <= 3; k++) {
            vector<pair<coord_t, coord_t>> raw;
            for(auto& [c, d]: segs) {
                Point c2 = rotate_cw(c, k);
                Point d2 = rotate_cw(d, k);
                auto pts = segment_segment_intersection(a, b, c2, d2);
                if(pts.empty()) {
                    continue;
                }
                coord_t t1 = ((pts[0] - a) * ab) / len2;
                t1 = max(0.0, min(1.0, t1));
                if(pts.size() == 1) {
                    raw.push_back({t1, t1});
                } else {
                    coord_t t2 = ((pts[1] - a) * ab) / len2;
                    t2 = max(0.0, min(1.0, t2));
                    if(t1 > t2) {
                        swap(t1, t2);
                    }
                    raw.push_back({t1, t2});
                }
            }
            sort(raw.begin(), raw.end());
            for(auto& p: raw) {
                if(!E[k - 1].empty() &&
                   p.first <= E[k - 1].back().second + 1e-12) {
                    E[k - 1].back().second =
                        max(E[k - 1].back().second, p.second);
                } else {
                    E[k - 1].push_back(p);
                }
            }
        }

        vector<coord_t> ts = {0.0, 1.0};
        for(int k = 0; k < 3; k++) {
            for(auto& [lo, hi]: E[k]) {
                ts.push_back(lo);
                ts.push_back(hi);
            }
        }
        sort(ts.begin(), ts.end());
        vector<coord_t> cand;
        for(coord_t t: ts) {
            t = max(0.0, min(1.0, t));
            if(cand.empty() || fabs(t - cand.back()) > 1e-12) {
                cand.push_back(t);
            }
        }

        auto in_E = [&](coord_t t, int k) {
            for(auto& [lo, hi]: E[k]) {
                if(t >= lo - Point::eps && t <= hi + Point::eps) {
                    return true;
                }
            }
            return false;
        };

        coord_t dx = ab.x, dy = ab.y;
        coord_t qa = dx * dx + dy * dy;
        coord_t qb = 2 * (a.x * dx + a.y * dy);
        coord_t qc = a.x * a.x + a.y * a.y;
        auto F = [&](coord_t t) {
            return qa / 3 * t * t * t + qb / 2 * t * t + qc * t;
        };

        for(int i = 0; i + 1 < (int)cand.size(); i++) {
            coord_t tlo = cand[i], thi = cand[i + 1];
            if(thi - tlo < 1e-12) {
                continue;
            }
            coord_t mid = (tlo + thi) / 2;
            if(in_E(mid, 0) && in_E(mid, 1) && in_E(mid, 2)) {
                total_length += (thi - tlo) * L;
                total_integral += 2 * L * (F(thi) - F(tlo));
            }
        }

        for(coord_t t: cand) {
            if(in_E(t, 0) && in_E(t, 1) && in_E(t, 2)) {
                discrete_points.push_back(a + ab * t);
            }
        }
    }

    cout << fixed << setprecision(10);
    if(total_length > 1e-9) {
        cout << total_integral / total_length << '\n';
    } else {
        vector<Point> uniq;
        for(auto& p: discrete_points) {
            bool found = false;
            for(auto& q: uniq) {
                if((p - q).norm() < 1e-6) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                uniq.push_back(p);
            }
        }
        coord_t sum_area = 0;
        for(auto& p: uniq) {
            sum_area += 2 * p.norm2();
        }
        cout << sum_area / (coord_t)uniq.size() << '\n';
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
